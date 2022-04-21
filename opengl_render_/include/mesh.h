#ifndef MESH_H_
#define MESH_H_

#include <string>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "bufferobject.h"
#include "shader.h"
#include "vertexarraybufferobject.h"

constexpr int MAX_BONE_INFLUENCE = 4;

class Shader;

namespace mesh
{
    struct Vertex
    {
        Vertex() = default;
        glm::vec3 m_position{};
        glm::vec3 m_normal{};
        glm::vec2 m_texCoords{};

        // �����ڸ����� 
        // tangent
        glm::vec3 m_tangent{};
        // bitangent
        glm::vec3 m_bitangent{};
        //bone indexes which will influence this vertex
        int m_boneids[MAX_BONE_INFLUENCE]{};
        //weights from each bone
        float m_weights[MAX_BONE_INFLUENCE]{};
    };

    struct Texture
    {
        unsigned int m_id; // ����ID
        std::string m_type; // �������� (diff spec ambient)
        std::string m_path; // �����·�� 
    };
};

class Mesh
{
public:
    // mesh date
    std::vector<mesh::Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<mesh::Texture> m_textures;

    // ��ʼ������ 
    Mesh(std::vector<mesh::Vertex> vertices, std::vector<unsigned int> indices,
         std::vector<mesh::Texture> texture);

    // ��������
    void draw(const Shader* shader) const;

    ~Mesh();
protected:
    VertexArrayObject* m_vao;
    BufferObject* m_vbo;
    BufferObject* m_ebo;

private:
    void setupMesh();
};


#endif // MESH_H_
