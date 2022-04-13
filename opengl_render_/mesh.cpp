#include "mesh.h"

Mesh::Mesh(std::vector<AssimpMesh::Vertex> vertices, std::vector<unsigned> indices, std::vector<AssimpMesh::Texture> texture)
	: vao_(nullptr),
	  vbo_(nullptr),
	  ebo_(nullptr)
{
	vertices_ = vertices;
	indices_  = indices;
	textures_ = texture;

	setupMesh();  // 使用传入的数据初始化网格 
}

void Mesh::draw(const Shader * shader) const
{
	unsigned int diffuse_nr  = 1;
	unsigned int specular_nr = 1;
	unsigned int normal_nr = 1;
	unsigned int height_nr = 1;

	// texture_ --> vector 记录当前网格的所有纹理 
	for(unsigned int i =0;i < textures_.size();i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);  // 激活对应的纹理单元

		// 获取纹理的序号 -- 用于设置给采样器 
		std::string  number;
		std::string name = textures_[i].type_;

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
		glBindTexture(GL_TEXTURE_2D, textures_[i].id_);
	}	// 绘制 网格
	vao_->bind();


	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices_.size()), GL_UNSIGNED_INT, 0);
	vao_->release();
	glActiveTexture(GL_TEXTURE0);  // 激活纹理党员 0  -- 默认激活 
}

Mesh::~Mesh()
{
#if 0
	if (vao_)
	{
		vao_->release();
		vao_->destroy();
		delete vao_;
		vao_ = nullptr;
	}

	if (vbo_)
	{
		vbo_->release();
		vbo_->destroy();
		delete vbo_;
		vbo_ = nullptr;
	}

	if (ebo_)
	{
		ebo_->release();
		ebo_->destroy();
		delete ebo_;
		ebo_ = nullptr;
	}
#endif 

}

void Mesh::setupMesh()
{
	vao_ = new VertexArrayObject;
	vao_->create();

	// create vertex buffer object 
	vbo_ = new BufferObject(object_buffer::OBJECT_TYPE::VERTEX_BUFFER);
	ebo_ = new BufferObject(object_buffer::OBJECT_TYPE::INDEX_BUFFER);

	vbo_->create();
	ebo_->create();

	vao_->bind();

	auto res = vbo_->bind();
	if(!res)
	{
		std::cout << "bind vertex buffer object error" << std::endl;
	}

	// 为数据分配空间  -- vbo
	int size = static_cast<int>(vertices_.size()) * static_cast<int>(sizeof(vertices_[0]));
	vbo_->allocate(vertices_.data(), size);   

	// ebo
	res = ebo_->bind();
	if (!res)
	{
		std::cout << "bind index buffer object error" << std::endl;
	}
	size = static_cast<int>(indices_.size() * sizeof(unsigned int));
	ebo_->allocate(indices_.data(), size);

	// 绑定到顶点属性
	// vertex 
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(AssimpMesh::Vertex), static_cast<void*>(0));

	// normal
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(AssimpMesh::Vertex), (void*)offsetof(AssimpMesh::Vertex,normal_));

	// texture d
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(AssimpMesh::Vertex), (void*)offsetof(AssimpMesh::Vertex, tex_coords_));
	// vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(AssimpMesh::Vertex), (void*)offsetof(AssimpMesh::Vertex, tangent_));
	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(AssimpMesh::Vertex), (void*)offsetof(AssimpMesh::Vertex, bitangent_));

	// ids
	glEnableVertexAttribArray(5);
	glVertexAttribIPointer(5, 4, GL_INT, sizeof(AssimpMesh::Vertex), (void*)offsetof(AssimpMesh::Vertex, m_boneids_));

	// weights
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(AssimpMesh::Vertex), (void*)offsetof(AssimpMesh::Vertex, m_weights_));

	// 绑定到默认的顶点属性 

	vao_->release();

	ebo_->release();
	vbo_->release();

}
