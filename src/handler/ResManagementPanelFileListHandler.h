#ifndef RESMANAGEMENTPANELFILELISTHANDLER_H
#define RESMANAGEMENTPANELFILELISTHANDLER_H
#include <QSet>
#include "ResManagementPanelHandler.h"
#include "src/model/FileItem_Fwd.h"

class FileTreeModel;
class QFileInfo;
class QLineEdit;
class QListView;
class QModelIndex;
class QToolButton;
class QTreeView;
class QStackedLayout;
class SearchSettingForm;

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
	void							OnSearchSettingBtnClicked();

private:
	FileItem_WeakPtr				target_dir_;

	FileItem_SharedPtr				current_file_tree_root_item_;
	QScopedPointer<FileTreeModel>	current_file_tree_model_{ nullptr };

	std::shared_ptr<FileTreeIconProvider> file_tree_icon_provider_;

	QString							search_key_word_;
	Qt::CaseSensitivity				case_senditivity_of_key_word_{ Qt::CaseInsensitive };
	QSet<QString>					file_suffix_set_to_search_;

	QPointer<QStackedLayout>		stacked_layout_;
	QPointer<QTreeView>				file_tree_view_;
	QPointer<QListView>				file_list_view_;
	QPointer<QLineEdit>				search_key_edit_;

	QPointer<QToolButton>			search_setting_btn_;
	QScopedPointer<SearchSettingForm> search_setting_form_;
};

#endif // RESMANAGEMENTPANELFILELISTHANDLER_H
