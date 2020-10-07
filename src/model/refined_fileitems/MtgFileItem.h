#ifndef MTGFILEITEM_H
#define MTGFILEITEM_H
#include "src/model/FileItem.h"

struct MaterialItem
{
	QString name_;
};

struct MaterialGroup
{
	QList<MaterialItem> items_;
};

class MtgFileItem : public FileItem
{
	Q_OBJECT

public:
	static const QString Suffix;

public:
	static std::shared_ptr<MtgFileItem>	Create(const QString& file_path,
											   const QString& display_name,
											   const FileItem_SharedPtr& parent = nullptr);

	static MaterialGroup ExtractMaterialGroupFromMtgFile(const QString& file_path);

private:
	MtgFileItem(const QString& file_path,
				const QString& display_name,
				const FileItem_SharedPtr& parent = nullptr);
};

class MtgMaterialFileItem : public FileItem
{
	Q_OBJECT

public:
	static std::shared_ptr<MtgMaterialFileItem>	Create(const QString& file_path,
													   const QString& display_name,
													   const FileItem_SharedPtr& parent = nullptr);

private:
	MtgMaterialFileItem(const QString& file_path,
						const QString& display_name,
						const FileItem_SharedPtr& parent = nullptr);

	virtual QVariant	DataAtColumn(int column) override;
};

#endif // MTGFILEITEM_H
