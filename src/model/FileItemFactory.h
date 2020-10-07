#ifndef FILEITEMFACTORY_H
#define FILEITEMFACTORY_H
#include <QString>
#include "FileItem_Fwd.h"

class FileItemFactory
{
public:
	FileItemFactory() = delete;

	static FileItem_SharedPtr CreateFileItem(const QString& file_path, 
											 const QString& display_name,
											 const FileItem_SharedPtr& parent = nullptr);
};

#endif // FILEITEMFACTORY_H
