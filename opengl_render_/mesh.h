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

		// �����ڸ����� 
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
		unsigned int id_;     // ����ID
		std::string type_;    // �������� (diff spec ambient)
		std::string path_;    // �����·�� 
	};

};

class Mesh
{
public:
	// mesh date
	std::vector<AssimpMesh::Vertex>       vertices_;
	std::vector<unsigned int> indices_;
	std::vector<AssimpMesh::Texture>      textures_;

	// ��ʼ������ 
	Mesh(std::vector<AssimpMesh::Vertex>  vertices, std::vector<unsigned int>  indices, std::vector<AssimpMesh::Texture>  texture);

	// ��������
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