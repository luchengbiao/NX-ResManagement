#include "FileItemFactory.h"
#include <QFileInfo>
#include "FileItem.h"
#include "src/model/refined_fileitems/MtgFileItem.h"

FileItem_SharedPtr FileItemFactory::CreateFileItem(const QString& file_path, 
												   const QString& display_name,
												   const FileItem_SharedPtr& parent)
{
	QFileInfo file_info(file_path);
	if (file_info.isFile())
	{
		const QString suffix = file_info.completeSuffix();
		if (suffix.compare(MtgFileItem::Suffix) == 0)
		{
			return MtgFileItem::Create(file_path, display_name, parent);
		}
	}

	return FileItem::Create(file_path, display_name, parent);
}
