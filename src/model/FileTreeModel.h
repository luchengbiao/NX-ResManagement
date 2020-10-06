#ifndef FILETREEMODEL_H
#define FILETREEMODEL_H
#include <unordered_map>
#include <QAbstractItemModel>
#include <QDir>
#include "FileItem_Fwd.h"

class QFileIconProvider;
class QFileSystemWatcher;

class FileTreeModel : public QAbstractItemModel
{
    Q_OBJECT

	friend class FileItem;

	using FileItemsWithinModelIndex = std::unordered_map<FileItem*, FileItem_WeakPtr>;

public:
	FileTreeModel(const QVector<QString>& header_column_names,
				  const FileItem_SharedPtr& root_item,
                  QObject* parent = nullptr);

	~FileTreeModel();

	void					SetFilters(QDir::Filters filters);
	void					WatchFileTreeRecursively(bool recursively);
	void					SetFileIconProvider(const std::shared_ptr<QFileIconProvider>& file_icon_provider);
	void					NotifyFileIconLoaded(const FileItem_SharedPtr& item);

	virtual QVariant		data(const QModelIndex& index, int role) const override;
	virtual Qt::ItemFlags	flags(const QModelIndex& index) const override;
	virtual QVariant		headerData(int section, Qt::Orientation orientation, int role) const override;
	virtual QModelIndex		index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
	virtual QModelIndex		parent(const QModelIndex& index) const override;
	virtual int				rowCount(const QModelIndex& parent = QModelIndex()) const override;
	virtual int				columnCount(const QModelIndex& parent = QModelIndex()) const override;

	QModelIndex				CreateIndex(const FileItem_SharedPtr& item, int column) const;
	QModelIndex				CreateParentIndex(const FileItem_SharedPtr& item) const;
	FileItem_SharedPtr		TreeItemWithinModelIndex(const QModelIndex& index) const;

private:
	void					InstallFileSystemWatcher();
	
	void					OnItemAboutToBeRemoved(const FileItem_SharedPtr& parent, const FileItem_SharedPtr& item, int index_of_item);
	void					OnItemRemoved(const FileItem_SharedPtr& parent, const FileItem_SharedPtr& item, int index_of_item);
	void					OnBeginToAddItem(const FileItem_SharedPtr& parent, const FileItem_SharedPtr& item, int index_of_item);
	void					OnItemAdded(const FileItem_SharedPtr& parent, const FileItem_SharedPtr& item, int index_of_item);
	void					OnItemChanged(const FileItem_SharedPtr& parent, const FileItem_SharedPtr& item, int index_of_item);

	void					OnDirectoryChanged(const QString& path);
	void					OnFileChanged(const QString& path);

private:
	const QVector<QString>	header_column_names_;

	FileItem_SharedPtr		root_item_;

	// to prevent dangling pointer from static_cast<FileItem*>(index.internalPointer())
	mutable FileItemsWithinModelIndex items_within_model_index_;

	QDir::Filters			filters_{ QDir::Dirs };
	bool					watch_file_tree_recursively_{ true };

	std::shared_ptr<QFileIconProvider> file_icon_provider_;

	QScopedPointer<QFileSystemWatcher> file_system_watcher_;
};

#endif // FILETREEMODEL_H
