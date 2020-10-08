#include "NfxFileItem.h"
#include <QFile>
#include "FileItemOnlyWithName.h"
#include "src/model/FileItemFactory.h"

const QString NfxFileItem::Suffix = "nfx";

std::shared_ptr<NfxFileItem> NfxFileItem::Create(const QString& file_path,
	                                             const QString& display_name,
	                                             const FileItem_SharedPtr& parent)
{
	std::shared_ptr<NfxFileItem> file_item(new NfxFileItem(file_path, display_name, parent));

	if (file_item)
	{
		TechniqueGroup tech_group = ExtractTechniqueGroupFromNfxFile(file_path);
		for (const auto& item : tech_group.items_)
		{
			file_item->AppendChild(FileItemOnlyWithName::Create(item.name_, item.name_));
		}
	}

	return file_item;
}

TechniqueGroup NfxFileItem::ExtractTechniqueGroupFromNfxFile(const QString& file_path)
{
	TechniqueGroup group;

	QFile file(file_path);
	if (file.open(QFile::ReadOnly | QFile::Text) == false)
	{
		return group;
	}

	while (!file.atEnd())
	{
		QString line = file.readLine();
		if (line.startsWith("technique ") && line.length() > 10) // only parse simplely here; TODO with token-parsing.
		{
			TechniqueItem tech_item;

			QString name = line.mid(10).trimmed();
			tech_item.name_ = name;

			group.items_.append(tech_item);
		}
	};

	file.close();

	return group;
}

NfxFileItem::NfxFileItem(const QString& file_path,
	                     const QString& display_name,
	                     const FileItem_SharedPtr& parent)
	: FileItem(file_path, display_name, parent)
{}