#include "FileType.h"
#include <unordered_map>

QStringList FileTypeHelper::SuffixListByType(FileType type)
{
	static const QStringList suffixes[static_cast<int>(FileType::Count)] = {
		{ "scn" }, // Scene
		{ "?" }, // Light
		{ "gim" }, // Model
		{ "animation", "skeleton" }, // Animation
		{ "pse", "sfx" }, // Effect
		{ "surf", "nfx" }, // Shader
		{ "mtg", "mtl" }, // Material
		{ "cube", "dds", "jpg", "png", "tga" }, // Mapping
		{ "bnk" }, // Audio
		{ "?" }, // Video
		{ "uiprefab" }, // UI
	};

	if (static_cast<int>(type) >= 0
		&& type < FileType::Count)
	{
		return suffixes[static_cast<int>(type)];
	}

	return QStringList();
}