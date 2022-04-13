#ifndef MESH_H_
#define MESH_H_

#include <string>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <iostream>
#include <vector>

#include "bufferobject.h"
#include "vertexarraybufferobject.h"
#include "shader.h"

#define MAX_BONE_INFLUENCE 4

class Shader;

namespace AssimpMesh
{

	struct Vertex
	{
		Vertex()
		{
			
		}
		glm::vec3 position_;
		glm::vec3 normal_;
		glm::vec2 tex_coords_;

		// 切线于副切线 
		// tangent
		glm::vec3 tangent_;
		// bitangent
		glm::vec3 bitangent_;
		//bone indexes which will influence this vertex
		int m_boneids_[MAX_BONE_INFLUENCE];
		//weights from each bone
		float m_weights_[MAX_BONE_INFLUENCE];

	};

	struct Texture
	{
		unsigned int id_;     // 纹理ID
		std::string type_;    // 纹理的类别 (diff spec ambient)
		std::string path_;    // 纹理的路径 
	};

};

class Mesh
{
public:
	// mesh date
	std::vector<AssimpMesh::Vertex>       vertices_;
	std::vector<unsigned int> indices_;
	std::vector<AssimpMesh::Texture>      textures_;

	// 初始化数据 
	Mesh(std::vector<AssimpMesh::Vertex>  vertices, std::vector<unsigned int>  indices, std::vector<AssimpMesh::Texture>  texture);

	// 绘制网格
	void draw(const Shader * shader) const;

	~Mesh();
protected:

	VertexArrayObject* vao_;
	BufferObject* vbo_;
	BufferObject* ebo_;

private:

	void setupMesh();


};



#endif // MESH_H_