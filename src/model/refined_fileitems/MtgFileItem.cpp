#include "MtgFileItem.h"
#include <QFile>
#include <QXmlStreamReader>
#include "src/model/FileItemFactory.h"

const QString MtgFileItem::Suffix = "mtg";

std::shared_ptr<MtgFileItem> MtgFileItem::Create(const QString& file_path,
												 const QString& display_name,
											     const FileItem_SharedPtr& parent)
{
	std::shared_ptr<MtgFileItem> file_item(new MtgFileItem(file_path, display_name, parent));

	if (file_item)
	{
		MaterialGroup material_group = ExtractMaterialGroupFromMtgFile(file_path);
		for (const auto& item : material_group.items_)
		{
			file_item->AppendChild(MtgMaterialFileItem::Create(item.name_, item.name_));
		}
	}

	return file_item;
}

static MaterialItem ExtractMaterialItem(QXmlStreamReader& reader);

MaterialGroup MtgFileItem::ExtractMaterialGroupFromMtgFile(const QString& file_path)
{
	MaterialGroup group;

	QFile file(file_path);
	if (file.open(QFile::ReadOnly | QFile::Text) == false)
	{
		return group;
	}

	QXmlStreamReader reader(&file);
	while (!reader.atEnd() && !reader.hasError())
	{
		const QXmlStreamReader::TokenType token = reader.readNext();

		if (token == QXmlStreamReader::StartDocument)
		{
			continue;
		}

		if (token == QXmlStreamReader::StartElement)
		{
			if (reader.name() == "Material")
			{
				group.items_.append(ExtractMaterialItem(reader));
			}
		}

		if (token == QXmlStreamReader::EndElement)
		{}
	}

	reader.clear();
	file.close();

	return group;
}

MaterialItem ExtractMaterialItem(QXmlStreamReader& reader)
{
	MaterialItem item;

	if (reader.tokenType() != QXmlStreamReader::StartElement &&
		reader.name() == "Material")
	{
		return item;
	}

	QXmlStreamAttributes attributes = reader.attributes();
	if (attributes.hasAttribute("Name"))
	{
		item.name_ = attributes.value("Name").toString();
	}

	// ...

	return item;
}

MtgFileItem::MtgFileItem(const QString& file_path,
						 const QString& display_name,
						 const FileItem_SharedPtr& parent)
	: FileItem(file_path, display_name, parent)
{}

std::shared_ptr<MtgMaterialFileItem> MtgMaterialFileItem::Create(const QString& file_path,
																 const QString& display_name,
																 const FileItem_SharedPtr& parent)
{
	return std::shared_ptr<MtgMaterialFileItem>(new MtgMaterialFileItem(file_path, display_name, parent));
}

MtgMaterialFileItem::MtgMaterialFileItem(const QString& file_path,
										 const QString& display_name,
										 const FileItem_SharedPtr& parent)
	: FileItem(file_path, display_name, parent)
{}

QVariant MtgMaterialFileItem::DataAtColumn(int column)
{
	if (column == 0)
	{
		return QVariant(display_name_);
	}

	return QVariant();
}