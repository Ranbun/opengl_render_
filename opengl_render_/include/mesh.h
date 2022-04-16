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

        // 切线于副切线 
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
        unsigned int m_id; // 纹理ID
        std::string m_type; // 纹理的类别 (diff spec ambient)
        std::string m_path; // 纹理的路径 
    };
};

class Mesh
{
public:
    // mesh date
    std::vector<mesh::Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<mesh::Texture> m_textures;

    // 初始化数据 
    Mesh(std::vector<mesh::Vertex> vertices, std::vector<unsigned int> indices,
         std::vector<mesh::Texture> texture);

    // 绘制网格
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
