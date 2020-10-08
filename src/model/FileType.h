#ifndef FILETYPE_H
#define FILETYPE_H
#include <QStringList>

enum class FileType
{
	Scene,     // 场景: scn
	Light,     // 灯光
	Model,	   // 模型
	Animation, // 动画
	Effect,    // 特性
	Shader,    // Shader
	Material,  // 材质
	Mapping,   // 贴图
	Audio,	   // 音效
	Video,	   // 视频
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
