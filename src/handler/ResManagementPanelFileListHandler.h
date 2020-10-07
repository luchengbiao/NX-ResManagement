#ifndef RESMANAGEMENTPANELFILELISTHANDLER_H
#define RESMANAGEMENTPANELFILELISTHANDLER_H
#include "ResManagementPanelHandler.h"
#include "src/model/FileItem_Fwd.h"

class FileTreeModel;
class QFileInfo;
class QLineEdit;
class QListView;
class QModelIndex;
class QTreeView;
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

	void							CreateFileTreeRootItem();
	bool							ShouldSearchFiles() const;
	QList<FileItem_SharedPtr>		SearchFilesInDir(const QString& dir_path);
	QList<FileItem_SharedPtr>		SearchFilesFromFileItemList(const QList<FileItem_SharedPtr>& file_item_list);
	bool							TestFileInfoForSearching(const QFileInfo& file_info);

	void							OnImageFileIconLoaded(const QIcon& icon, const QString& file_path);

private Q_SLOTS:
	void							OnFileItemDoubleClicked(const QModelIndex& index);
	void							OnSliderValueChanged(int value);
	void							OnSearchKeyEditTextChanged(const QString& text);

private:
	FileItem_WeakPtr				target_dir_;

	FileItem_SharedPtr				current_file_tree_root_item_;
	QScopedPointer<FileTreeModel>	current_file_tree_model_{ nullptr };

	std::shared_ptr<FileTreeIconProvider> file_tree_icon_provider_;

	QString							search_key_word_;
	Qt::CaseSensitivity				case_senditivity_of_key_word_{ Qt::CaseInsensitive };

	QPointer<QStackedLayout>		stacked_layout_;
	QPointer<QTreeView>				file_tree_view_;
	QPointer<QListView>				file_list_view_;
	QPointer<QLineEdit>				search_key_edit_;
};

#endif // RESMANAGEMENTPANELFILELISTHANDLER_H
