#include "ResManagementPanelFileListHandler.h"
#include <QFileIconProvider>
#include <QListView>
#include <QMimeDatabase>
#include <QMimeType>
#include <QStackedLayout>
#include <QtConcurrent>
#include "ResManagementPanelDirTreeHandler.h"
#include "src/model/FileItem.h"
#include "src/model/FileTreeModel.h"
#include "src/utils/FunctionPerformer.h"
#include "ui_ResManagementPanel.h"

class ResManagementPanelFileListHandler::FileTreeIconProvider : public QFileIconProvider
															  , public std::enable_shared_from_this<FileTreeIconProvider>
{
public:
	FileTreeIconProvider(ResManagementPanelFileListHandler* handler) 
		: handler_(handler)
	{}

	virtual QIcon icon(const QFileInfo& file_info) const
	{
		QMimeDatabase db;
		QMimeType mime = db.mimeTypeForFile(file_info.filePath());
		if (mime.name().startsWith("image/"))
		{
			auto it = cached_image_file_icons_.find(file_info.filePath());
			if (it != cached_image_file_icons_.cend())
			{
				return it->second;
			}

			// load image asyncly
			std::weak_ptr<FileTreeIconProvider> weak_this = std::const_pointer_cast<FileTreeIconProvider>(shared_from_this());
			QtConcurrent::run([weak_this, file_info] {
				QImage scaled_image = QImage(file_info.filePath()).scaled(128, 128);

				// notify in main thread
				FunctionPerformer::PerformOnMainThread([weak_this, scaled_image, file_info] {
					auto shared_this = weak_this.lock();
					if (shared_this)
					{
						shared_this->OnImageFileIconLoaded(scaled_image, file_info.filePath());
					}
				});
			});
		}

		return QFileIconProvider::icon(file_info);
	}

private:
	void OnImageFileIconLoaded(const QImage& scaled_image, const QString& file_path)
	{
		if (scaled_image.isNull())
		{
			return;
		}

		QIcon icon = QIcon(QPixmap::fromImage(scaled_image));
		cached_image_file_icons_[file_path] = icon;

		if (handler_)
		{
			handler_->OnImageFileIconLoaded(icon, file_path);
		}
	}

private:
	QPointer<ResManagementPanelFileListHandler> handler_;
	std::unordered_map<QString, QIcon> cached_image_file_icons_;
};

ResManagementPanelFileListHandler::ResManagementPanelFileListHandler(ResManagementPanel* panel)
	: ResManagementPanelHandlerWithType(panel)
{}

void ResManagementPanelFileListHandler::Init()
{
	auto ui = UiOfPanel();
	if (ui == nullptr)
	{
		return;
	}

	// create stacked widgets
	{
		stacked_layout_ = new QStackedLayout(ui->file_list_wrapper_widget);

		file_tree_view_ = new QTreeView(ui->file_list_wrapper_widget);
		file_tree_view_->setFrameShape(QFrame::NoFrame);
		file_tree_view_->setFrameShadow(QFrame::Sunken);
		file_tree_view_->setLineWidth(0);

		file_list_view_ = new QListView(ui->file_list_wrapper_widget);
		file_list_view_->setFrameShape(QFrame::NoFrame);
		file_list_view_->setFrameShadow(QFrame::Sunken);
		file_list_view_->setViewMode(QListView::IconMode);
		file_list_view_->setMovement(QListView::Static); 
		file_list_view_->setIconSize(QSize(64, 64)); 
		file_list_view_->setGridSize(QSize(64 + 50, 64 + 30));
		file_list_view_->setResizeMode(QListView::Adjust);

		stacked_layout_->addWidget(file_tree_view_);
		stacked_layout_->addWidget(file_list_view_);
	}

	ui->slider->setRange(0, 16 * 7);
	slider_ = ui->slider;

	connect(file_tree_view_, &QTreeView::doubleClicked, this, &ResManagementPanelFileListHandler::OnFileTreeDoubleClicked);
	connect(slider_, &QSlider::valueChanged, this, &ResManagementPanelFileListHandler::OnSliderValueChanged);
}

void ResManagementPanelFileListHandler::AfterInited()
{
	// test code
	//this->ShowFilesInTargetDir(FileItem::Create(R"(C:\Users\luchengbiao\AppData\Local\neox-hub\demo_art_basic_tutorial\res\test)", "test"));
}

void ResManagementPanelFileListHandler::Reset()
{
	target_dir_.reset();
	current_file_tree_root_item_.reset();
	current_file_tree_model_.reset();
	file_tree_view_->setModel(nullptr);
}

void ResManagementPanelFileListHandler::ShowFilesInTargetDir(const FileItem_SharedPtr& target_dir)
{
	if (target_dir == target_dir_.lock())
	{
		return;
	}

	this->Reset();
	target_dir_ = target_dir;
	
	if (target_dir == nullptr)
	{
		return;
	}

	QString dir_path = target_dir->FilePath();
	if (dir_path.isEmpty() == false &&
		QFileInfo(dir_path).isDir())
	{
		current_file_tree_root_item_ = FileItem::Create(dir_path, QFileInfo(dir_path).fileName());
		current_file_tree_root_item_->BuildTree(QDir::Dirs | QDir::Files, false);
	}
	else
	{
		current_file_tree_root_item_ = FileItem::Create("", "");

		auto child_list = target_dir->ChildList(false);
		for (const auto& child : child_list)
		{
			QString file_path = child->FilePath();
			current_file_tree_root_item_->AppendChild(FileItem::Create(file_path, QFileInfo(file_path).fileName()));
		}
	}

	QVector<QString> header_column_names = {
			QStringLiteral("文件名"),
			QStringLiteral("最后修改时间"),
			QStringLiteral("大小"),
	};

	current_file_tree_model_.reset(new FileTreeModel(header_column_names, current_file_tree_root_item_));
	current_file_tree_model_->SetFilters(QDir::Dirs | QDir::Files);
	current_file_tree_model_->WatchFileTreeRecursively(false);

	if (!file_tree_icon_provider_)
	{
		file_tree_icon_provider_ = std::make_shared<FileTreeIconProvider>(this);
	}

	current_file_tree_model_->SetFileIconProvider(file_tree_icon_provider_);

	file_tree_view_->setModel(current_file_tree_model_.get());
	file_list_view_->setModel(current_file_tree_model_.get());

	for (int column = 0; column < current_file_tree_model_->columnCount(); ++column)
	{
		file_tree_view_->resizeColumnToContents(column);
	}
}

void ResManagementPanelFileListHandler::OnImageFileIconLoaded(const QIcon& icon, const QString& file_path)
{
	Q_UNUSED(icon);

	if (current_file_tree_model_ && current_file_tree_root_item_)
	{
		auto item = current_file_tree_root_item_->FindChildByPath(file_path);
		if (item)
		{
			current_file_tree_model_->NotifyFileIconLoaded(item);
		}
	}
}

void ResManagementPanelFileListHandler::OnFileTreeDoubleClicked(const QModelIndex& index)
{
	auto dir_tree_handler = SiblingHandler<ResManagementPanelDirTreeHandler>();
	if (dir_tree_handler == nullptr)
	{
		return;
	}

	auto file_tree_model = dynamic_cast<FileTreeModel*>(file_tree_view_->model());
	if (file_tree_model == nullptr)
	{
		return;
	}

	auto file_item = file_tree_model->TreeItemWithinModelIndex(index);
	if (file_item && file_item->IsDir())
	{
		dir_tree_handler->SelectItemWithFilePath(file_item->FilePath());
	}
}

void ResManagementPanelFileListHandler::OnSliderValueChanged(int value)
{
	if (stacked_layout_ == nullptr)
	{
		return;
	}

	const int critical_value = 16;

	const int current_index = value >= critical_value ? 1 : 0;
	stacked_layout_->setCurrentIndex(current_index);

	if (current_index == 1)
	{
		const int factor = (value - critical_value) / 16;
		const int size = 16 * (factor + 1);

		file_list_view_->setIconSize(QSize(size, size));
		file_list_view_->setGridSize(QSize(size + 50, size + 30));
	}
}
