#ifndef RESMANAGEMENTPANELFILELISTHANDLER_H
#define RESMANAGEMENTPANELFILELISTHANDLER_H
#include <QPointer>
#include <QTreeView>
#include "ResManagementPanelHandler.h"
#include "src/model/FileItem_Fwd.h"

class FileTreeModel;

class ResManagementPanelFileListHandler : public ResManagementPanelHandlerWithType<ResManagementPanelHandlerType::FileList>
{
	class FileTreeIconProvider;
	friend class FileTreeIconProvider;

public:
    ResManagementPanelFileListHandler(ResManagementPanel* panel);

	void							ShowFilesInTargetDir(const FileItem_SharedPtr& target_dir);

private:
	virtual void					Init() override;
	virtual void					AfterInited() override;

	void							OnImageFileIconLoaded(const QIcon& icon, const QString& file_path);

private:
	FileItem_WeakPtr				target_dir_;

	FileItem_SharedPtr				current_file_tree_root_item_;
	QScopedPointer<FileTreeModel>	current_file_tree_model_{ nullptr };

	std::shared_ptr<FileTreeIconProvider> file_tree_icon_provider_;

	QPointer<QTreeView>				file_tree_view_;
};

#endif // RESMANAGEMENTPANELFILELISTHANDLER_H
