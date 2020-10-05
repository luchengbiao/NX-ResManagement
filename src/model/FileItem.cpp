#include "FileItem.h"
#include <QDateTime>
#include <QDir>
#include <QPointer>
#include <QSet>
#include "FileTreeModel.h"

FileItem_SharedPtr FileItem::Create(const QString& file_path,
											const QString& display_name,
											const FileItem_SharedPtr& parent)
{
	return FileItem_SharedPtr(new FileItem(file_path, display_name, parent));
}

FileItem::FileItem(const QString& file_path,
	                       const QString& display_name,
	                       const FileItem_SharedPtr& parent)
	: file_path_(file_path)
	, display_name_(display_name)
	, parent_(parent)
{}

QString FileItem::FilePath() const
{
	return file_path_;
}

QString FileItem::NameToDisplay() const
{
	return display_name_;
}

QVariant FileItem::DataAtColumn(int column)
{
	switch (column)
	{
	case 0:
		return QVariant(display_name_);

	case 1:
		return QVariant(QFileInfo(file_path_).lastModified());

	case 2:
	{
		QFileInfo file_info(file_path_);
		if (file_info.isDir())
		{
			return QVariant();
		}
		else
		{
			qint64 size = QFileInfo(file_path_).size();
			if (size < 1024)
			{
				return QString("%1B").arg(size);
			}
			else
			{
				return QString("%1KB").arg(size / 1024);
			}
		}
	}


	default:
		break;
	}

	return QVariant();
}

void FileItem::BuildTree(QDir::Filters filters, bool recursively)
{
	const QStringList child_file_path_list = ChildFilePathList(filters, file_path_);
	for (const auto& file_path : child_file_path_list)
	{
		auto dir_item = FileItem::Create(file_path, QFileInfo(file_path).fileName());
		this->AppendChild(dir_item);

		if (recursively)
		{
			dir_item->BuildTree(filters, true);
		}
	}
}

void FileItem::AppendChild(const FileItem_SharedPtr& child)
{
	if (child && !children_.contains(child))
	{
		children_.append(child);

		if (child->Parent().get() != this)
		{
			child->parent_ = shared_from_this();
		}

		this->ResetIndexOfChild(children_.size() - 1);

		file_path_2_child_[child->FilePath()] = child;
	}
}

FileItem_SharedPtr FileItem::ChildAt(int index) const
{
	if (index >= 0 && index < children_.size())
	{
		return children_.at(index);
	}

	return nullptr;
}

int FileItem::CountOfChildren() const
{
	return children_.size();
}

FileItem_SharedPtr FileItem::Parent() const
{
	return parent_.lock();
}

int FileItem::IndexInParent() const
{
	auto parent = this->Parent();
	if (parent)
	{
		return parent->IndexOfChild(std::const_pointer_cast<FileItem>(shared_from_this()));
	}

	return -1;
}

int FileItem::IndexOfChild(const FileItem_SharedPtr& child) const
{
	auto it = child_2_index_.find(child);
	if (it != child_2_index_.cend())
	{
		return it->second;
	}

	return -1;
}

QStringList FileItem::ChildFilePathList(bool recursively) const
{
	QStringList str_list;

	for (const auto& child : children_)
	{
		auto file_path = child->FilePath();
		if (file_path.isEmpty() == false)
		{
			str_list.append(file_path);
		}

		if (recursively)
		{
			str_list.append(child->ChildFilePathList(true));
		}
	}

	return str_list;
}

QList<FileItem_SharedPtr> FileItem::ChildList(bool recursively) const
{
	QList<FileItem_SharedPtr> children_list = QList<FileItem_SharedPtr>::fromVector(children_);

	if (recursively)
	{
		for (const auto& child : children_)
		{
			children_list.append(child->ChildList(true));
		}
	}

	return children_list;
}

FileItem_SharedPtr FileItem::FindChildByPath(const QString& file_path, bool recursively)
{
	auto it = file_path_2_child_.find(file_path);
	if (it != file_path_2_child_.cend())
	{
		return it->second;
	}

	if (recursively)
	{
		for (const auto& child : children_)
		{
			auto item = child->FindChildByPath(file_path);
			if (item)
			{
				return item;
			}
		}
	}

	return nullptr;
}

void FileItem::HandleDirChanged(QDir::Filters filters, FileTreeModel* model, bool build_new_tree_node_recursively)
{
	QPointer<FileTreeModel> model_guard(model); // to prevent observer is destroyed in any callback.

	const QStringList current_dir_list = this->ChildFilePathList(false);
	const QSet<QString> current_dir_set = QSet<QString>::fromList(current_dir_list);

	const QStringList new_dir_list = ChildFilePathList(filters, file_path_);
	const QSet<QString> new_dir_set = QSet<QString>::fromList(new_dir_list);

	// dirs that have been added.
	QSet<QString> added_dir_set = new_dir_set - current_dir_set;
	QStringList added_dir_list = added_dir_set.toList();

	// dirs that have been removed
	QSet<QString> removed_dir_set = current_dir_set - new_dir_set;
	QStringList removed_dir_list = removed_dir_set.toList();

	FileItem_SharedPtr shared_this = shared_from_this();

	if (added_dir_list.count() == 1
		&& removed_dir_list.count() == 1)
	{
		// only one dir that has been renamed
		auto item = this->FindChildByPath(removed_dir_list.front(), false);
		if (item)
		{
			item->RenameFilePath(added_dir_list.front());

			if (model_guard)
			{
				model_guard->OnItemChanged(shared_this, item, IndexOfChild(item));
			}
		}
	}
	else
	{
		if (removed_dir_list.isEmpty() == false)
		{
			QVector<int> index_list_to_remove;
			for (const auto& dir : removed_dir_list)
			{
				int index = this->IndexOfChildWithPath(dir);
				if (index >= 0)
				{
					index_list_to_remove.append(index);
				}
			}

			qSort(index_list_to_remove);

			for (int i = index_list_to_remove.size() - 1; i >= 0; --i) // remove from back to front
			{
				int index = index_list_to_remove.at(i);
				FileItem_SharedPtr item = children_.at(index);

				if (model_guard)
				{
					model_guard->OnItemAboutToBeRemoved(shared_this, item, index);
				}

				this->RemoveAt(index);

				if (model_guard)
				{
					model_guard->OnItemRemoved(shared_this, item, index);
				}
			}
		}

		if (added_dir_list.isEmpty() == false)
		{
			for (const auto& dir : added_dir_list)
			{
				auto dir_item = FileItem::Create(dir, QFileInfo(dir).fileName());

				if (model_guard)
				{
					model_guard->OnBeginToAddItem(shared_this, dir_item, CountOfChildren());
				}

				this->AppendChild(dir_item);

				if (build_new_tree_node_recursively)
				{
					dir_item->BuildTree(filters, true);
				}

				if (model_guard)
				{
					model_guard->OnItemAdded(shared_this, dir_item, IndexOfChild(dir_item));
				}
			}
		}
	}
}

void FileItem::ResetIndexOfChild(int from)
{
	for (int i = from; i < children_.size(); ++i)
	{
		child_2_index_[children_.at(i)] = i;
	}
}

int FileItem::IndexOfChildWithPath(const QString& file_path)
{
	auto it = file_path_2_child_.find(file_path);
	if (it == file_path_2_child_.cend())
	{
		return -1;
	}

	return IndexOfChild(it->second);
}

void FileItem::RemoveAt(int index)
{
	if (index >= 0 && index < children_.size())
	{
		auto item = children_.at(index);

		child_2_index_.erase(item);
		file_path_2_child_.erase(item->FilePath());

		children_.removeAt(index);

		this->ResetIndexOfChild(index);
	}
}

void FileItem::RenameFilePath(const QString& file_path)
{
	QString old_file_path = file_path_;

	file_path_ = file_path;
	display_name_ = QFileInfo(file_path_).fileName();

	auto parent = Parent();
	if (parent)
	{
		parent->OnChildFilePathRenamed(shared_from_this(), old_file_path);
	}
}

void FileItem::OnChildFilePathRenamed(const FileItem_SharedPtr& child, const QString& old_file_path)
{
	file_path_2_child_.erase(old_file_path);

	file_path_2_child_[child->FilePath()] = child;
}

QStringList FileItem::ChildFilePathList(QDir::Filters filters, const QString& parent_file_path)
{
	QStringList child_file_path_list;

	QDir dir(parent_file_path);
	QFileInfoList file_info_list = dir.entryInfoList(filters, QDir::DirsFirst);
	for (const auto& file_info : file_info_list)
	{
		const QString file_name = file_info.fileName();
		if (file_name == "." | file_name == "..")
		{
			continue;
		}

		child_file_path_list.append(file_info.filePath());
	}

	return child_file_path_list;
}