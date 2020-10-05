#include "FileTreeModel.h"
#include <QFileIconProvider>
#include <QFileSystemWatcher>
#include "FileItem.h"

FileTreeModel::FileTreeModel(const QVector<QString>& header_column_names,
							 const FileItem_SharedPtr& root_item,
						     QObject* parent)
	: QAbstractItemModel(parent)
	, header_column_names_(header_column_names)
	, root_item_(root_item)
{
	this->InstallFileSystemWatcher();
}

FileTreeModel::~FileTreeModel()
{}

void FileTreeModel::SetFilters(QDir::Filters filters)
{
	filters_ = filters;
}

void FileTreeModel::WatchFileTreeRecursively(bool recursively)
{
	if (watch_file_tree_recursively_ != recursively)
	{
		watch_file_tree_recursively_ = recursively;

		if (file_system_watcher_)
		{
			file_system_watcher_.reset();

			this->InstallFileSystemWatcher();
		}
	}
}

void FileTreeModel::SetFileIconProvider(const std::shared_ptr<QFileIconProvider>& file_icon_provider)
{
	file_icon_provider_ = file_icon_provider;
}

void FileTreeModel::NotifyFileIconLoaded(const FileItem_SharedPtr& item)
{
	emit dataChanged(CreateIndex(item, 0), CreateIndex(item, 0), { Qt::DecorationRole });
}

void FileTreeModel::InstallFileSystemWatcher()
{
	file_system_watcher_.reset(new QFileSystemWatcher(this));

	file_system_watcher_->addPath(root_item_->FilePath());
	file_system_watcher_->addPaths(root_item_->ChildFilePathList(watch_file_tree_recursively_));

	connect(file_system_watcher_.get(), &QFileSystemWatcher::directoryChanged, this, &FileTreeModel::OnDirectoryChanged);
	connect(file_system_watcher_.get(), &QFileSystemWatcher::fileChanged, this, &FileTreeModel::OnFileChanged);
}

QVariant FileTreeModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid())
	{
		return QVariant();
	}

	auto item = this->TreeItemWithinModelIndex(index);
	if (item == nullptr)
	{
		return QVariant();
	}

	if (role == Qt::DecorationRole)
	{
		if (index.column() == 0)
		{
			if (file_icon_provider_)
			{
				return file_icon_provider_->icon(QFileInfo(item->FilePath()));
			}
		}
		
		return QVariant();
	}
		
	if (role == Qt::DisplayRole)
	{
		return item->DataAtColumn(index.column());
	}

	return QVariant();
}

Qt::ItemFlags FileTreeModel::flags(const QModelIndex& index) const
{
	if (!index.isValid())
	{
		return Qt::NoItemFlags;
	}

	return QAbstractItemModel::flags(index);
}

QVariant FileTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	Q_UNUSED(section);

	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
	{
		if (section >= 0 && section < header_column_names_.size())
		{
			return header_column_names_.at(section);
		}
	}
		
	return QVariant();
}

QModelIndex FileTreeModel::index(int row, int column, const QModelIndex& parent) const
{
	if (!hasIndex(row, column, parent))
	{
		return QModelIndex();
	}
		
	auto parent_item = this->TreeItemWithinModelIndex(parent);
		
	auto child_item = parent_item->ChildAt(row);
	if (child_item)
	{
		return CreateIndex(child_item, column);
	}
		
	return QModelIndex();
}

QModelIndex FileTreeModel::parent(const QModelIndex& index) const
{
	if (!index.isValid())
	{
		return QModelIndex();
	}

	auto child_item = this->TreeItemWithinModelIndex(index);
	auto parent_item = child_item->Parent();

	if (parent_item == nullptr 
		|| parent_item == root_item_)
	{
		return QModelIndex();
	}

	return CreateIndex(parent_item, 0);
}

int FileTreeModel::rowCount(const QModelIndex& parent) const
{
	if (parent.column() > 0)
	{
		return 0;
	}

	auto parent_item = this->TreeItemWithinModelIndex(parent);

	return parent_item->CountOfChildren();
}

int FileTreeModel::columnCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);

	return header_column_names_.size();
}

void FileTreeModel::OnItemAboutToBeRemoved(const FileItem_SharedPtr& parent, const FileItem_SharedPtr& item, int index_of_item)
{
	Q_UNUSED(item);

	emit rowsAboutToBeRemoved(CreateIndex(parent, 0), index_of_item, index_of_item, {}); // View may change the selected item
}

void FileTreeModel::OnItemRemoved(const FileItem_SharedPtr& parent, const FileItem_SharedPtr& item, int index_of_item)
{
	items_within_model_index_.erase(item.get());

	auto item_list = item->ChildList(true);
	if (item_list.isEmpty() == false)
	{
		for (const auto& it : item_list)
		{
			items_within_model_index_.erase(it.get());
		}
	}

	emit rowsRemoved(CreateIndex(parent, 0), index_of_item, index_of_item, {});
}

void FileTreeModel::OnBeginToAddItem(const FileItem_SharedPtr& parent, const FileItem_SharedPtr& item, int index_of_item)
{
	Q_UNUSED(item);

	this->beginInsertRows(CreateIndex(parent, parent == root_item_? -1 : 0), index_of_item, index_of_item);
}

void FileTreeModel::OnItemAdded(const FileItem_SharedPtr& parent, const FileItem_SharedPtr& item, int index_of_item)
{
	Q_UNUSED(item);

	this->endInsertRows();
}

void FileTreeModel::OnItemChanged(const FileItem_SharedPtr& parent, const FileItem_SharedPtr& item, int index_of_item)
{
	Q_UNUSED(parent);

	Q_UNUSED(index_of_item);

	emit dataChanged(CreateIndex(item, 0), CreateIndex(item, header_column_names_.size() - 1), { Qt::DisplayRole });
}

FileItem_SharedPtr FileTreeModel::TreeItemWithinModelIndex(const QModelIndex& index) const
{
	if (index.isValid() == false)
	{
		return root_item_;
	}

	do
	{
		FileItem* item = static_cast<FileItem*>(index.internalPointer());
		if (item == nullptr)
		{
			break;
		}

		auto it = items_within_model_index_.find(item);
		if (it == items_within_model_index_.cend())
		{
			break;
		}

		auto item_shared_ptr = it->second.lock();
		if (item_shared_ptr)
		{
			return item_shared_ptr;
		}

	} while (false);

	return root_item_;
}

QModelIndex FileTreeModel::CreateIndex(const FileItem_SharedPtr& item, int column) const
{
	QModelIndex index = QAbstractItemModel::createIndex(item->IndexInParent(), column, item.get());

	if (item)
	{
		items_within_model_index_[item.get()] = item;
	}

	return index;
}

QModelIndex FileTreeModel::CreateParentIndex(const FileItem_SharedPtr& item) const
{
	auto parent = item->Parent();
	if (parent == nullptr)
	{
		return QModelIndex();
	}

	return CreateIndex(parent, 0);
}

void FileTreeModel::OnDirectoryChanged(const QString& path)
{
	auto item = (root_item_->FilePath() == path)? root_item_ :  root_item_->FindChildByPath(path);
	if (item)
	{
		file_system_watcher_->removePaths(item->ChildFilePathList());

		item->HandleDirChanged(filters_, this, watch_file_tree_recursively_);

		file_system_watcher_->addPaths(item->ChildFilePathList());
	}
}

void FileTreeModel::OnFileChanged(const QString& path)
{
	auto item = root_item_->FindChildByPath(path);
	if (item)
	{
		this->OnItemChanged(item->Parent(), item, item->IndexInParent());
	}
}