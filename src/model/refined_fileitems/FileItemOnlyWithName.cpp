#include "FileItemOnlyWithName.h"

std::shared_ptr<FileItemOnlyWithName> FileItemOnlyWithName::Create(const QString& file_path,
                                                                   const QString& display_name,
                                                                   const FileItem_SharedPtr& parent)
{
    return std::shared_ptr<FileItemOnlyWithName>(new FileItemOnlyWithName(file_path, display_name, parent));
}

FileItemOnlyWithName::FileItemOnlyWithName(const QString& file_path,
                                           const QString& display_name,
                                           const FileItem_SharedPtr& parent)
    : FileItem(file_path, display_name, parent)
{}

QVariant FileItemOnlyWithName::DataAtColumn(int column)
{
    if (column == 0)
    {
        return QVariant(display_name_);
    }

    return QVariant();
}
