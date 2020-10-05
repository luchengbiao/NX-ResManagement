#include "ResManagementPanelFileListHandler.h"
#include <QFileIconProvider>
#include <QMimeDatabase>
#include <QMimeType>
#include <QtConcurrent>
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
				QImage scaled_image = QImage(file_info.filePath()).scaled(16, 16);

				// notify in main thread
				FunctionPerformer::PerformInMainThread([weak_this, scaled_image, file_info] {
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

	file_tree_view_ = ui->file_tree_view;
}

void ResManagementPanelFileListHandler::AfterInited()
{
	// test path
	//this->ShowFilesInTargetDir(FileItem::Create(R"(C:\Users\luchengbiao\AppData\Local\neox-hub\demo_art_basic_tutorial\res\test)", "test"));
}

void ResManagementPanelFileListHandler::ShowFilesInTargetDir(const FileItem_SharedPtr& target_dir)
{
	if (target_dir == target_dir_.lock())
	{
		return;
	}

	target_dir_ = target_dir;
	current_file_tree_root_item_.reset();
	current_file_tree_model_.reset();
	file_tree_view_->setModel(nullptr);

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

	if (current_file_tree_root_item_)
	{
		QVector<QString> header_column_names = { 
			QStringLiteral("�ļ���"),
			QStringLiteral("����޸�ʱ��"),
			QStringLiteral("��С"),
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

		for (int column = 0; column < current_file_tree_model_->columnCount(); ++column)
		{
			file_tree_view_->resizeColumnToContents(column);
		}
	}

	// FIXME: �ڸ��ڵ��������µ��ļ����ļ���, QTreeViewû�м�ʱ��ʾ����.
}

void ResManagementPanelFileListHandler::OnImageFileIconLoaded(const QIcon& icon, const QString& file_path)
{
	if (current_file_tree_model_ && current_file_tree_root_item_)
	{
		auto item = current_file_tree_root_item_->FindChildByPath(file_path);
		if (item)
		{
			current_file_tree_model_->NotifyFileIconLoaded(item);
		}
	}
}