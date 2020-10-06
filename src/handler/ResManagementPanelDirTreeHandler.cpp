#include "ResManagementPanelDirTreeHandler.h"
#include <QFileIconProvider>
#include "ResManagementPanelFileListHandler.h"
#include "ResManagementPanelFilePathNaviHandler.h"
#include "src/model/FileItem.h"
#include "src/model/FileTreeModel.h"
#include "ui_ResManagementPanel.h"

class DirTreeIconProvider : public QFileIconProvider
{
public:
	virtual QIcon icon(IconType) const
	{
		return DefaultDirIcon();
	}

	virtual QIcon icon(const QFileInfo&) const
	{
		return DefaultDirIcon();
	}

private:
	const QIcon& DefaultDirIcon() const
	{
		if (dir_icon_.isNull())
		{
			QFileInfo file_info(QDir::currentPath());
			dir_icon_ = QFileIconProvider::icon(file_info).pixmap(16, 16);
		}

		return dir_icon_;
	}

private:
	mutable QIcon dir_icon_;
};

ResManagementPanelDirTreeHandler::ResManagementPanelDirTreeHandler(ResManagementPanel* panel)
    : ResManagementPanelHandlerWithType(panel)
	, dir_tree_root_item_(FileItem::Create("", ""))
{}

void ResManagementPanelDirTreeHandler::Init()
{
    auto ui = UiOfPanel();
    if (ui == nullptr)
    {
        return;
    }

    QString project_res_dir = panel_->ProjectResDir();
    QString builtin_res_dir = panel_->BuiltinResDir();

	auto content_item = FileItem::Create("", "Content");
	auto project_res_item = FileItem::Create(project_res_dir, "Game/res");

	auto packages_item = FileItem::Create("", "Packages");
	auto builtin_res_item = FileItem::Create(builtin_res_dir, "builtin/res");

	dir_tree_root_item_->AppendChild(content_item);
	dir_tree_root_item_->AppendChild(packages_item);

	content_item->AppendChild(project_res_item);
	packages_item->AppendChild(builtin_res_item);

	project_res_item->BuildTree(QDir::Dirs, true);
	builtin_res_item->BuildTree(QDir::Dirs, true);

	dir_tree_model_ = new FileTreeModel({ QStringLiteral("") }, dir_tree_root_item_, this);
	dir_tree_model_->SetFileIconProvider(std::make_shared<DirTreeIconProvider>());

    dir_tree_view_ = ui->dir_tree_view;
    dir_tree_view_->setModel(dir_tree_model_);

	connect(dir_tree_view_->selectionModel(), &QItemSelectionModel::selectionChanged, this, &ResManagementPanelDirTreeHandler::OnDirTreeSelectionChanged);
}

void ResManagementPanelDirTreeHandler::AfterInited()
{}

void ResManagementPanelDirTreeHandler::OnDirTreeSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected)
{
	Q_UNUSED(deselected);

	auto file_list_handler = SiblingHandler<ResManagementPanelFileListHandler>();
	auto file_path_navi_handler = SiblingHandler<ResManagementPanelFilePathNaviHandler>();

	auto indexes = selected.indexes();
	if (indexes.isEmpty())
	{
		file_list_handler->Reset();
		file_path_navi_handler->Reset();
		
		return;
	}

	if (dir_tree_model_ == nullptr)
	{
		return;
	}

	auto file_item = dir_tree_model_->TreeItemWithinModelIndex(indexes.front());
	if (file_item)
	{
		file_list_handler->ShowFilesInTargetDir(file_item);
		file_path_navi_handler->ShowFilePath(file_item);
	}
}

void ResManagementPanelDirTreeHandler::SelectItemWithFilePath(const QString& file_path)
{
	if (dir_tree_root_item_ == nullptr)
	{
		return;
	}

	auto target_item = dir_tree_root_item_->FindChildByPath(file_path, true);
	if (target_item)
	{
		this->SelectFileItem(target_item);
	}
}

void ResManagementPanelDirTreeHandler::SelectFileItem(const FileItem_SharedPtr& file_item)
{
	if (dir_tree_view_ == nullptr
		|| dir_tree_model_ == nullptr
		|| dir_tree_root_item_ == nullptr)
	{
		return;
	}

	if (file_item 
		&& dir_tree_root_item_->IsAncestorOf(*file_item))
	{
		QModelIndex model_index = dir_tree_model_->CreateIndex(file_item, 0);
		dir_tree_view_->selectionModel()->select(model_index, { QItemSelectionModel::ClearAndSelect });
		dir_tree_view_->expand(model_index);
		dir_tree_view_->scrollTo(model_index);
	}
}

