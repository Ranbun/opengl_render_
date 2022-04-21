#include "mesh.h"

#include <iostream>

Mesh::Mesh(std::vector<mesh::Vertex> vertices, std::vector<unsigned> indices, std::vector<mesh::Texture> texture)
	: m_vao(nullptr),
	  m_vbo(nullptr),
	  m_ebo(nullptr)
{
	// use std::copy
	m_vertices = vertices;
	m_indices  = indices;
	m_textures = texture;

	setupMesh();  // 使用传入的数据初始化网格 
}

void Mesh::draw(const Shader * shader) const
{
	unsigned int diffuse_nr  = 1;
	unsigned int specular_nr = 1;
	unsigned int normal_nr = 1;
	unsigned int height_nr = 1;

	// texture_ --> vector 记录当前网格的所有纹理 
	for(unsigned int i =0;i < m_textures.size();i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);  // 激活对应的纹理单元

		// 获取纹理的序号 -- 用于设置给采样器 
		std::string  number;
		std::string name = m_textures[i].m_type;

		if(name == "texture_diffuse")
		{
			number = std::to_string(diffuse_nr++);
		}
		else if(name == "texture_specular")
		{
			number = std::to_string(specular_nr++);
		}
		else if (name == "texture_normal")
		{
			number = std::to_string(normal_nr++); 
		}
		else if (name == "texture_height")
		{
			number = std::to_string(height_nr++);
		}

		// 设置采样器(告诉采样器当前纹理属于那个纹理单元)
		shader->setFloat(("material." + name + number), static_cast<float>(i));
		// shader->setFloat((name + number), i);
		glBindTexture(GL_TEXTURE_2D, m_textures[i].m_id);
	}	// 绘制 网格
	m_vao->bind();


	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, 0);
	m_vao->release();
	glActiveTexture(GL_TEXTURE0);  // 激活纹理党员 0  -- 默认激活 
}

Mesh::~Mesh()
{
}

void Mesh::setupMesh()
{
	m_vao = new VertexArrayObject;
	m_vao->create();

	// create vertex buffer object 
	m_vbo = new BufferObject(object_type::vertex_buffer);
	m_ebo = new BufferObject(object_type::index_buffer);

	m_vbo->create();
	m_ebo->create();

	m_vao->bind();

	auto res = m_vbo->bind();
	if(!res)
	{
		std::cout << "bind vertex buffer object error" << std::endl;
	}

	// 为数据分配空间  -- vbo
	int size = static_cast<int>(m_vertices.size()) * static_cast<int>(sizeof(m_vertices[0]));
	m_vbo->allocate(m_vertices.data(), size);   					 

	// ebo
	res = m_ebo->bind();
	if (!res)
	{
		std::cout << "bind index buffer object error" << std::endl;
	}
	size = static_cast<int>(m_indices.size() * sizeof(unsigned int));
	m_ebo->allocate(m_indices.data(), size);

	// 绑定到顶点属性
	// vertex 
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(mesh::Vertex), static_cast<void*>(nullptr));

	// normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(mesh::Vertex)
		, reinterpret_cast<void*>(offsetof(mesh::Vertex, m_normal)));	// NOLINT(performance-no-int-to-ptr)

	// texture d
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(mesh::Vertex)
		, reinterpret_cast<void*>(offsetof(mesh::Vertex, m_texCoords)));  // NOLINT(performance-no-int-to-ptr)
	// vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(mesh::Vertex)
		, reinterpret_cast<void*>(offsetof(mesh::Vertex, m_tangent)));	 // NOLINT(performance-no-int-to-ptr)
	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(mesh::Vertex)
		, reinterpret_cast<void*>(offsetof(mesh::Vertex, m_bitangent)));   // NOLINT(performance-no-int-to-ptr)

	// ids
	glEnableVertexAttribArray(5);
	glVertexAttribIPointer(5, 4, GL_INT, sizeof(mesh::Vertex)
		, reinterpret_cast<void*>(offsetof(mesh::Vertex, m_boneids)));	 // NOLINT(performance-no-int-to-ptr)

	// weights
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(mesh::Vertex)
		, reinterpret_cast<void*>(offsetof(mesh::Vertex, m_weights)));  // NOLINT(performance-no-int-to-ptr)

	// 绑定到默认的顶点属性 

	m_vao->release();

	m_ebo->release();
	m_vbo->release();

}
