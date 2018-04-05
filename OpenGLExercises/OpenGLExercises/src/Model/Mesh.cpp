#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
	: m_vertices(vertices),
	m_indices(indices),
	m_textures(textures)
{
	Init();
}

void Mesh::Init()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);
	//Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	//NormVec
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_normalVec));
	//TexCoords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_texCoords));

	//Clean up
	glBindVertexArray(0);
}

void Mesh::Render(ShadersLoader shader)
{
	unsigned int diffuseNum = 1;
	unsigned int specularNum = 1;

	for (unsigned int i = 0; i < m_textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		std::string texNumber;
		std::string textureType = m_textures[i].m_type;
		if (textureType == "texture_diffuse")
		{
			texNumber = std::to_string(diffuseNum++);
		}
		else if (textureType == "texture_specular")
		{
			texNumber = std::to_string(specularNum++);
		}

		shader.SetInt("material_" + textureType + texNumber, i);
		glBindTexture(GL_TEXTURE_2D, m_textures[i].m_id);
	}
}
