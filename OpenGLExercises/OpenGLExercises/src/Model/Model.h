#pragma once

#include "..\Utilities\stbImageLoader\stb_image.h"
#include "Mesh.h"

class Model
{
public:
	Model(const char* datapath);
	void Render(ShadersLoader shader);
private:
	void Init(std::string dataPath);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	unsigned int LoadTextureFromFile(const char *path, const std::string &directory, bool gamma = false);
	std::vector<Texture> LoadMaterialTexture(aiMaterial* material, aiTextureType textureType, std::string typeName);

	std::vector<Mesh>		m_meshes;
	std::string				m_path;
	std::vector<Texture>	m_loadedTextures;
};