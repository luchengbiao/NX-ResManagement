#include "ResManagementPanelFileListHandler.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QFileIconProvider>
#include <QListView>
#include <QMimeDatabase>
#include <QMimeType>
#include <QStackedLayout>
#include <QtConcurrent>
#include "ResManagementPanelDirTreeHandler.h"
#include "src/model/FileItem.h"
#include "src/model/FileItemFactory.h"
#include "src/model/FileTreeModel.h"
#include "src/panel/SearchSettingForm.h"
#include "src/utils/FunctionPerformer.h"
#include "ui_ResManagementPanel.h"

static const int s_icon_grid_delta_x = 50;
static const int s_icon_grid_delta_y = 30;

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
		file_list_view_->setIconSize(QSize(16, 16));
		file_list_view_->setGridSize(QSize(16 + s_icon_grid_delta_x, 16 + s_icon_grid_delta_y));
		file_list_view_->setResizeMode(QListView::Adjust);

		stacked_layout_->addWidget(file_tree_view_);
		stacked_layout_->addWidget(file_list_view_);
	}

	ui->slider->setRange(0, 16 * 7);
	search_key_edit_ = ui->search_key_edit;
	search_setting_btn_ = ui->search_setting_btn;

	connect(file_tree_view_, &QTreeView::doubleClicked, this, &ResManagementPanelFileListHandler::OnFileItemDoubleClicked);
	connect(file_list_view_, &QListView::doubleClicked, this, &ResManagementPanelFileListHandler::OnFileItemDoubleClicked);
	connect(ui->slider, &QSlider::valueChanged, this, &ResManagementPanelFileListHandler::OnSliderValueChanged);
	connect(search_key_edit_, &QLineEdit::textChanged, this, &ResManagementPanelFileListHandler::OnSearchKeyEditTextChanged);
	connect(search_setting_btn_, &QToolButton::clicked, this, &ResManagementPanelFileListHandler::OnSearchSettingBtnClicked);
}

void ResManagementPanelFileListHandler::AfterInited()
{}

void ResManagementPanelFileListHandler::Reset()
{
	target_dir_.reset();
	current_file_tree_root_item_.reset();
	current_file_tree_model_.reset();
	file_tree_view_->setModel(nullptr);
	file_list_view_->setModel(nullptr);
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

	CreateFileTreeRootItem();
	if (current_file_tree_root_item_ == nullptr)
	{
		return;
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

void ResManagementPanelFileListHandler::CreateFileTreeRootItem()
{
	auto target_dir = target_dir_.lock();
	if (target_dir == nullptr)
	{
		return;
	}

	QString dir_path = target_dir->FilePath();
	if (dir_path.isEmpty() == false &&
		QFileInfo(dir_path).isDir())
	{
		current_file_tree_root_item_ = FileItemFactory::CreateFileItem(dir_path, QFileInfo(dir_path).fileName());
		if (ShouldSearchFiles())
		{
			current_file_tree_root_item_->AppendChildList(SearchFilesInDir(dir_path));
		}
		else
		{
			current_file_tree_root_item_->BuildTree(QDir::Dirs | QDir::Files, false);
		}
	}
	else
	{
		current_file_tree_root_item_ = FileItemFactory::CreateFileItem("", "");

		auto child_list = target_dir->ChildList(false);

		if (ShouldSearchFiles())
		{
			current_file_tree_root_item_->AppendChildList(SearchFilesFromFileItemList(child_list));
		}
		else
		{
			for (const auto& child : child_list)
			{
				QString file_path = child->FilePath();
				current_file_tree_root_item_->AppendChild(FileItemFactory::CreateFileItem(file_path, QFileInfo(file_path).fileName()));
			}
		}
	}
}

bool ResManagementPanelFileListHandler::ShouldSearchFiles() const
{
	return !(search_key_word_.isEmpty() && file_suffix_set_to_search_.isEmpty());
}

QList<FileItem_SharedPtr> ResManagementPanelFileListHandler::SearchFilesInDir(const QString& dir_path)
{
	QList<FileItem_SharedPtr> searched_file_item_list;

	QDir dir(dir_path);
	QFileInfoList file_info_list = dir.entryInfoList(QDir::Dirs | QDir::Files , QDir::DirsFirst);
	for (const auto& file_info : file_info_list)
	{
		const QString file_name = file_info.fileName();
		if (file_name == "." | file_name == "..")
		{
			continue;
		}

		if (TestFileInfoForSearching(file_info))
		{
			auto file_item = FileItemFactory::CreateFileItem(file_info.filePath(), file_name);
			searched_file_item_list.append(file_item);
		}

		if (file_info.isDir())
		{
			searched_file_item_list.append(SearchFilesInDir(file_info.filePath()));
		}
	}

	return searched_file_item_list;
}

QList<FileItem_SharedPtr> ResManagementPanelFileListHandler::SearchFilesFromFileItemList(const QList<FileItem_SharedPtr>& file_item_list)
{
	QList<FileItem_SharedPtr> searched_file_item_list;

	for (const auto& file_item : file_item_list)
	{
		QString file_path = file_item->FilePath();
		QFileInfo file_info(file_path);
		if (TestFileInfoForSearching(file_info))
		{
			searched_file_item_list.append(FileItemFactory::CreateFileItem(file_path, file_info.fileName()));
		}

		if (file_info.isDir())
		{
			searched_file_item_list.append(SearchFilesInDir(file_path));
		}
	}

	return searched_file_item_list;
}

bool ResManagementPanelFileListHandler::TestFileInfoForSearching(const QFileInfo& file_info)
{
	if (file_suffix_set_to_search_.isEmpty() == false)
	{
		if (file_info.isFile() == false)
		{
			return false;
		}
		
		QString suffix = file_info.completeSuffix().toLower();
		if (file_suffix_set_to_search_.contains(suffix) == false)
		{
			return false;
		}
	}

	if (search_key_word_.isEmpty()
		|| file_info.fileName().contains(search_key_word_, case_senditivity_of_key_word_))
	{
		return true;
	}

	return false;
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

void ResManagementPanelFileListHandler::Reload()
{
	auto target_dir = target_dir_.lock();
	target_dir_.reset();
	this->ShowFilesInTargetDir(target_dir);
}

void ResManagementPanelFileListHandler::OnFileItemDoubleClicked(const QModelIndex& index)
{
	search_key_word_.clear();
	if (search_key_edit_)
	{
		search_key_edit_->setText(search_key_word_);
	}

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

	if (value < critical_value)
	{
		stacked_layout_->setCurrentIndex(0);
	}
	else
	{
		stacked_layout_->setCurrentIndex(1);

		const int factor = (value - critical_value) / 16;
		const int size = 16 * (factor + 1);

		file_list_view_->setIconSize(QSize(size, size));
		file_list_view_->setGridSize(QSize(size + s_icon_grid_delta_x, size + s_icon_grid_delta_y));
	}
}

void ResManagementPanelFileListHandler::OnSearchKeyEditTextChanged(const QString& text)
{
	if (search_key_word_ == text)
	{
		return;
	}

	search_key_word_ = text;

	this->Reload();
}

void ResManagementPanelFileListHandler::OnSearchSettingBtnClicked()
{
	if (!search_setting_form_)
	{
		search_setting_form_.reset(new SearchSettingForm());
		search_setting_form_->setWindowFlag(Qt::WindowStaysOnTopHint);
		search_setting_form_->setWindowFlag(Qt::WindowMinMaxButtonsHint, false);
		search_setting_form_->setWindowFlag(Qt::WindowContextHelpButtonHint);

		connect(search_setting_form_.get(), &SearchSettingForm::CaseSenditivityChanged, this, &ResManagementPanelFileListHandler::OnCaseSenditivityChanged);
		connect(search_setting_form_.get(), &SearchSettingForm::FileTypeChecked, this, &ResManagementPanelFileListHandler::OnFileTypeChecked);
		connect(search_setting_form_.get(), &SearchSettingForm::FileTypeUnchecked, this, &ResManagementPanelFileListHandler::OnFileTypeUnchecked);
	}

	if (search_setting_btn_)
	{
		QPoint pos = search_setting_btn_->mapToGlobal(QPoint(0, 0));

		pos.setX(pos.x() - search_setting_form_->width());
		pos.setY(pos.y() - search_setting_form_->height() / 2);

		QRect desktop_rc = QApplication::desktop()->availableGeometry();
		if (pos.x() < desktop_rc.left())
		{
			pos.setX(desktop_rc.left());
		}

		if (pos.x() + search_setting_form_->width() > desktop_rc.right())
		{
			pos.setX(desktop_rc.right() - search_setting_form_->width());
		}

		if (pos.y() < desktop_rc.top())
		{
			pos.setY(desktop_rc.top());
		}

		search_setting_form_->move(pos);
	}

	search_setting_form_->show();
}

void ResManagementPanelFileListHandler::OnCaseSenditivityChanged(Qt::CaseSensitivity case_sensitivity)
{
	if (case_senditivity_of_key_word_ == case_sensitivity)
	{
		return;
	}

	case_senditivity_of_key_word_ = case_sensitivity;
	if (search_key_word_.isEmpty() == false)
	{
		this->Reload();
	}
}

void ResManagementPanelFileListHandler::OnFileTypeChecked(FileType file_type)
{
	QStringList suffix_list = FileTypeHelper::SuffixListByType(file_type);
	if (suffix_list.isEmpty() == false)
	{
		const int old_size = file_suffix_set_to_search_.size();
		file_suffix_set_to_search_ |= suffix_list.toSet();
		if (file_suffix_set_to_search_.size() != old_size)
		{
			this->Reload();
		}
	}
}

void ResManagementPanelFileListHandler::OnFileTypeUnchecked(FileType file_type)
{
	QStringList suffix_list = FileTypeHelper::SuffixListByType(file_type);
	if (suffix_list.isEmpty() == false)
	{
		const int old_size = file_suffix_set_to_search_.size();
		file_suffix_set_to_search_ -= suffix_list.toSet();
		if (file_suffix_set_to_search_.size() != old_size)
		{
			this->Reload();
		}
	}
}
