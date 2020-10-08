#ifndef FILETYPE_H
#define FILETYPE_H
#include <QStringList>

enum class FileType
{
	Scene,     // ����: scn
	Light,     // �ƹ�
	Model,	   // ģ��
	Animation, // ����
	Effect,    // ����
	Shader,    // Shader
	Material,  // ����
	Mapping,   // ��ͼ
	Audio,	   // ��Ч
	Video,	   // ��Ƶ
	UI,        // UI
	Count,
};

class FileTypeHelper
{
public:
	FileTypeHelper() = delete;

	static QStringList SuffixListByType(FileType type);
};

#endif // FILETYPE_H
