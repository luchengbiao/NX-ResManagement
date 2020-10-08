#ifndef FILEITEMONLYWITHNAME_H
#define FILEITEMONLYWITHNAME_H
#include "src/model/FileItem.h"

class FileItemOnlyWithName : public FileItem
{
    Q_OBJECT

public:
    static std::shared_ptr<FileItemOnlyWithName>	Create(const QString& file_path,
                                                           const QString& display_name,
                                                           const FileItem_SharedPtr& parent = nullptr);

private:
    FileItemOnlyWithName(const QString& file_path,
                         const QString& display_name,
                         const FileItem_SharedPtr& parent = nullptr);

    virtual QVariant	DataAtColumn(int column) override;
};

#endif // FILEITEMONLYWITHNAME_H
