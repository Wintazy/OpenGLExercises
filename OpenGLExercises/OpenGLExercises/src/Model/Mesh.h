#pragma once
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../Shader/ShadersLoader.h"

struct Vertex
{
	glm::vec3 m_position;
	glm::vec3 m_normalVec;
	glm::vec2 m_texCoords;
};

struct Texture
{
	unsigned int m_id;
	std::string m_type;
	aiString m_path;
};

class Mesh
{
public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

	void Render(ShadersLoader shader);
private:
	void Init();

	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<Texture> m_textures;

	unsigned int VAO, VBO, EBO;
};
