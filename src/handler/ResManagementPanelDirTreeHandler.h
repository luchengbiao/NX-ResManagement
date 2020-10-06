#ifndef RESMANAGEMENTPANELDIRTREEHANDLER_H
#define RESMANAGEMENTPANELDIRTREEHANDLER_H
#include "src/model/FileItem_Fwd.h"
#include "ResManagementPanelHandler.h"

class FileTreeModel;
class QItemSelection;
class QTreeView;

class ResManagementPanelDirTreeHandler : public ResManagementPanelHandlerWithType<ResManagementPanelHandlerType::DirTree>
{
    Q_OBJECT

public:
    explicit ResManagementPanelDirTreeHandler(ResManagementPanel* panel);

	void							SelectItemWithFilePath(const QString& file_path);

private:
    virtual void					Init() override;
    virtual void					AfterInited() override;

private Q_SLOTS:
	void							OnDirTreeSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);

private:
	FileItem_SharedPtr				dir_tree_root_item_;
    FileTreeModel*                  dir_tree_model_{ nullptr };
    QPointer<QTreeView>				dir_tree_view_;
};

#endif // RESMANAGEMENTPANELDIRTREEHANDLER_H
