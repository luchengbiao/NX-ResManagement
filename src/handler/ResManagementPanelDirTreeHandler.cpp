#include "ResManagementPanelDirTreeHandler.h"
#include <QFileIconProvider>
#include "ResManagementPanelFileListHandler.h"
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

	connect(dir_tree_view_, &QTreeView::clicked, this, &ResManagementPanelDirTreeHandler::OnDirTreeClicked);
}

void ResManagementPanelDirTreeHandler::AfterInited()
{}

void ResManagementPanelDirTreeHandler::OnDirTreeClicked(const QModelIndex& index)
{
	auto file_tree_model = dynamic_cast<FileTreeModel*>(dir_tree_view_->model());
	if (file_tree_model == nullptr)
	{
		return;
	}

	auto file_list_handler = SiblingHandler<ResManagementPanelFileListHandler>();
	if (file_list_handler)
	{
		auto file_item = file_tree_model->TreeItemWithinModelIndex(index);

		file_list_handler->ShowFilesInTargetDir(file_item);
	}
}

