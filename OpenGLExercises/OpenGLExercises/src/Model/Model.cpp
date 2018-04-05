#include "Model.h"

Model::Model(const char* datapath)
{
	Init(datapath);
}

void Model::Render(ShadersLoader shader)
{
	for (unsigned int i = 0; i < m_meshes.size(); i++)
	{
		m_meshes[i].Render(shader);
	}
}

void Model::Init(std::string dataPath)
{
	Assimp::Importer dataImporter;
	const aiScene* scene = dataImporter.ReadFile(dataPath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSMIMP::" << dataImporter.GetErrorString() << std::endl;
		return;
	}

	m_path = dataPath.substr(0, dataPath.find_last_of('/'));

	ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back(ProcessMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene *scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	//Vectices
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;

		glm::vec3 vertexAttrib;
		vertexAttrib.x = mesh->mVertices[i].x;
		vertexAttrib.y = mesh->mVertices[i].y;
		vertexAttrib.z = mesh->mVertices[i].z;
		vertex.m_position = vertexAttrib;
		
		if(mesh->HasNormals())
		{
			vertexAttrib.x = mesh->mNormals[i].x;
			vertexAttrib.y = mesh->mNormals[i].y;
			vertexAttrib.z = mesh->mNormals[i].z;
			vertex.m_normalVec = vertexAttrib;
		}

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 textCoord;
			textCoord.x = mesh->mTextureCoords[0][i].x;
			textCoord.y = mesh->mTextureCoords[0][i].y;
			vertex.m_texCoords = textCoord;
		}
		else
			vertex.m_texCoords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}

	//Indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	//Materials
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = LoadMaterialTexture(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture> specularMaps = LoadMaterialTexture(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::LoadMaterialTexture(aiMaterial* material, aiTextureType textureType, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < material->GetTextureCount(textureType); i++)
	{
		aiString texturePath;
		material->GetTexture(textureType, i, &texturePath);
		bool isTextureLoaded = false;
		for (unsigned int j = 0; j < m_loadedTextures.size(); j++)
		{
			if (std::strcmp(m_loadedTextures[j].m_path.c_str(), texturePath.C_Str()) == 0)
			{
				textures.push_back(m_loadedTextures[j]);
				isTextureLoaded = true;
				break;
			}
		}

		if (!isTextureLoaded)
		{
			Texture texture;
			texture.m_id = LoadTextureFromFile(texturePath.C_Str(), m_path);
			texture.m_type = typeName;
			texture.m_path = texturePath.C_Str();
			textures.push_back(texture);
		}
	}

	return textures;
}

unsigned int Model::LoadTextureFromFile(const char *path, const std::string &directory, bool gamma)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}