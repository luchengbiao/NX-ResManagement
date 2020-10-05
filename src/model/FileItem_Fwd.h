#ifndef FILEITEM_FWD_H
#define FILEITEM_FWD_H
#include <memory>

class FileItem;
using FileItem_SharedPtr = std::shared_ptr<FileItem>;
using FileItem_WeakPtr = std::weak_ptr<FileItem>;

#endif // FILEITEM_FWD_H
