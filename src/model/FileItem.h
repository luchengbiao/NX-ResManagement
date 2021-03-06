#ifndef FILEITEM_H
#define FILEITEM_H
#include <unordered_map>
#include <QDir>
#include <QList>
#include <QObject>
#include <QVariant>
#include <QVector>
#include "FileItem_Fwd.h"

class DirChangedObserver;
class FileTreeModel;

class FileItem : public QObject, public std::enable_shared_from_this<FileItem>
{
	Q_OBJECT

public:
	static FileItem_SharedPtr			Create(const QString& file_path,
										       const QString& display_name,
										       const FileItem_SharedPtr& parent = nullptr);

public:
	bool								IsDir() const;
	bool								IsFile() const;

	QString								FilePath() const;
	QString								NameToDisplay() const;
	virtual QVariant					DataAtColumn(int column);

	void								BuildTree(QDir::Filters filters, bool recursively = true);

	void								AppendChild(const FileItem_SharedPtr& child);
	void								AppendChildList(const QList<FileItem_SharedPtr>& child_list);
	FileItem_SharedPtr					ChildAt(int index) const;
	int									CountOfChildren() const;

	FileItem_SharedPtr					Parent() const;
	bool								IsAncestorOf(const FileItem& child) const;

	int									IndexInParent() const;
	int									IndexOfChild(const FileItem_SharedPtr& child) const;

	QStringList							ChildFilePathList(bool recursively = true) const;
	QList<FileItem_SharedPtr>			ChildList(bool recursively = true) const;
	FileItem_SharedPtr					FindChildByPath(const QString& file_path, bool recursively = true);

	void								HandleDirChanged(QDir::Filters filters, FileTreeModel* model = nullptr, bool build_new_tree_node_recursively = true);

Q_SIGNALS:
	void								FilePathRenamed(const QString& old_file_path, const QString& new_file_path);

protected:
	FileItem(const QString& file_path,
			 const QString& display_name,
			 const FileItem_SharedPtr& parent = nullptr);

	void								ResetIndexOfChild(int from = 0);

	int									IndexOfChildWithPath(const QString& file_path);

	void								RemoveAt(int index);

	void								RenameFilePath(const QString& file_path);
	void								OnChildFilePathRenamed(const FileItem_SharedPtr& child, const QString& old_file_path);

	static QStringList					ChildFilePathList(QDir::Filters filters, const QString& parent_file_path);

protected:
	QString								file_path_;
	QString								display_name_;

	FileItem_WeakPtr					parent_;
	QVector<FileItem_SharedPtr>			children_;

	std::unordered_map<FileItem_SharedPtr, int> child_2_index_; // for quick query index of child
	std::unordered_map<QString, FileItem_SharedPtr> file_path_2_child_; // for quick query child by path
};


#endif // FILEITEM_H
