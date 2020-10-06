#ifndef RESMANAGEMENTPANELFILELISTHANDLER_H
#define RESMANAGEMENTPANELFILELISTHANDLER_H
#include "ResManagementPanelHandler.h"
#include "src/model/FileItem_Fwd.h"

class FileTreeModel;
class QListView;
class QModelIndex;
class QTreeView;
class QSlider;
class QStackedLayout;

class ResManagementPanelFileListHandler : public ResManagementPanelHandlerWithType<ResManagementPanelHandlerType::FileList>
{
	class FileTreeIconProvider;
	friend class FileTreeIconProvider;

public:
    ResManagementPanelFileListHandler(ResManagementPanel* panel);

	void							Reset();
	void							ShowFilesInTargetDir(const FileItem_SharedPtr& target_dir);

private:
	virtual void					Init() override;
	virtual void					AfterInited() override;

	void							OnImageFileIconLoaded(const QIcon& icon, const QString& file_path);

private Q_SLOTS:
	void							OnFileTreeDoubleClicked(const QModelIndex& index);
	void							OnSliderValueChanged(int value);

private:
	FileItem_WeakPtr				target_dir_;

	FileItem_SharedPtr				current_file_tree_root_item_;
	QScopedPointer<FileTreeModel>	current_file_tree_model_{ nullptr };

	std::shared_ptr<FileTreeIconProvider> file_tree_icon_provider_;

	QPointer<QStackedLayout>		stacked_layout_;
	QPointer<QTreeView>				file_tree_view_;
	QPointer<QListView>				file_list_view_;
	QPointer<QSlider>				slider_;
};

#endif // RESMANAGEMENTPANELFILELISTHANDLER_H
