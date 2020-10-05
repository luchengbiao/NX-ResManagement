#ifndef RESMANAGEMENTPANELDIRTREEHANDLER_H
#define RESMANAGEMENTPANELDIRTREEHANDLER_H
#include <QPointer>
#include <QTreeView>
#include "src/model/FileItem_Fwd.h"
#include "ResManagementPanelHandler.h"

class FileTreeModel;

class ResManagementPanelDirTreeHandler : public ResManagementPanelHandlerWithType<ResManagementPanelHandlerType::DirTree>
{
    Q_OBJECT

public:
    explicit ResManagementPanelDirTreeHandler(ResManagementPanel* panel);

private:
    virtual void					Init() override;
    virtual void					AfterInited() override;

private Q_SLOTS:
	void							OnDirTreeClicked(const QModelIndex& index);

private:
	FileItem_SharedPtr				dir_tree_root_item_;
    FileTreeModel*                  dir_tree_model_{ nullptr };
    QPointer<QTreeView>				dir_tree_view_;
};

#endif // RESMANAGEMENTPANELDIRTREEHANDLER_H
