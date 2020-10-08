#ifndef NFXFILEITEM_H
#define NFXFILEITEM_H
#include "src/model/FileItem.h"

struct TechniqueItem
{
	QString name_;
};

struct TechniqueGroup
{
	QList<TechniqueItem> items_;
};

class NfxFileItem : public FileItem
{
	Q_OBJECT

public:
	static const QString Suffix;

public:
	static std::shared_ptr<NfxFileItem>	Create(const QString& file_path,
											   const QString& display_name,
											   const FileItem_SharedPtr& parent = nullptr);

	static TechniqueGroup ExtractTechniqueGroupFromNfxFile(const QString& file_path);

private:
	NfxFileItem(const QString& file_path,
		        const QString& display_name,
		        const FileItem_SharedPtr& parent = nullptr);
};

#endif // NFXFILEITEM_H
