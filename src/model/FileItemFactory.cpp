#include "FileItemFactory.h"
#include <QFileInfo>
#include "FileItem.h"
#include "src/model/refined_fileitems/MtgFileItem.h"
#include "src/model/refined_fileitems/NfxFileItem.h"

FileItem_SharedPtr FileItemFactory::CreateFileItem(const QString& file_path, 
												   const QString& display_name,
												   const FileItem_SharedPtr& parent)
{
	QFileInfo file_info(file_path);
	if (file_info.isFile())
	{
		using FileItemCreator = std::function<FileItem_SharedPtr(const QString&, const QString&, const FileItem_SharedPtr&)>;

#define FILE_ITEM_CREATOR(FileItemT) { FileItemT::Suffix, \
		[](const QString& file_path, const QString& display_name, const FileItem_SharedPtr& parent)->FileItem_SharedPtr \
		{ return FileItemT::Create(file_path, display_name, parent); } }
		
		static const std::unordered_map<QString, FileItemCreator> file_item_creators = {
			FILE_ITEM_CREATOR(MtgFileItem),
			FILE_ITEM_CREATOR(NfxFileItem),
		};

#undef FILE_ITEM_CREATOR
		
		const QString suffix = file_info.completeSuffix().toLower();
		auto it = file_item_creators.find(suffix);
		if (it != file_item_creators.cend())
		{
			return it->second(file_path, display_name, parent);
		}
	}

	return FileItem::Create(file_path, display_name, parent);
}
