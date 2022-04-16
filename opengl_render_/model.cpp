#include "model.h"

#include <iostream>
#include <assimp/postprocess.h>
#include "stb_image.h"
#include "texture.h"

Model::Model(char* path, const bool gamma)
    : m_gammaCorrection(gamma)
    , m_path(path)
{
    loadModel(path);
}

void Model::draw(const Shader* shader) const
{

    for (auto& item : m_meshes)
    {
        item.draw(shader);
    }
}

Model::~Model()
{
    m_meshes.clear();
}

void Model::loadModel(const std::string path)
{
    // load model
    // 加载模型到Assimp的数据结构
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);  // 加载文件 并设置预处理项目
    // aiProcess_Triangulate  将模型的图元转换为三角形
    // aiProcess_FlipUVs      翻转Y轴的纹理坐标
    // 可以预生成法线
    // 可以将顶点数目较多的网格分割为多个
    // 将小网格拼接 

    // 加载场景失败 检查场景 以及某个标志  根节点是否为空 
    if ((!scene || scene->mFlags) && (AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode))
    {
        std::cout << "ERROR: ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }

    // 记录模型的文件夹 
    m_directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);

}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    // 解析当前节点
    // 处理当前节点的网格 
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // Scene下的mMeshes数组储存了真正的Mesh对象
        // 节点中的mMeshes数组保存的只是场景中网格数组的索引
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

        // 解析网格 
        m_meshes.push_back(processMesh(mesh, scene));
    }
    // 处理当节点的子节点
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }


}

// 创建 mesh 对象 
Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<mesh::Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<mesh::Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        mesh::Vertex vertex;
        // 添加顶点 法线 纹理坐标
        vertex.m_position.x = mesh->mVertices[i].x;
        vertex.m_position.y = mesh->mVertices[i].y;
        vertex.m_position.z = mesh->mVertices[i].z;

        if (mesh->HasNormals())
        {
            vertex.m_normal.x = mesh->mNormals[i].x;
            vertex.m_normal.y = mesh->mNormals[i].y;
            vertex.m_normal.z = mesh->mNormals[i].z;
        }

        // 需要判断有纹理坐标吗？
        // 一般情况 我们使用第一组纹理坐标(允许有多组纹理坐标) 
        if (mesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;

            vertex.m_texCoords = vec;

            glm::vec3 vector;

            if (mesh->HasTangentsAndBitangents())
            {
                // 切线
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.m_tangent = vector;

                // 副切线 
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.m_bitangent = vector;
            }
        }
        else
        {
            vertex.m_texCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    // 处理 索引 
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)  // 循环处理每个面
    {
        aiFace face = mesh->mFaces[i];
        for (uint32_t j = 0; j < face.mNumIndices; j++) // 处理每个面的索引 
        {
            indices.push_back(face.mIndices[j]);  // 将索引添加到数组 
        }
    }

    // 处理材质
    if (mesh->mMaterialIndex >= 0)
    {
        // 获取实际的材料 -- 当前网格的所有材料信息 
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<mesh::Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<mesh::Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        std::vector<mesh::Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. height maps
        std::vector<mesh::Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
    }


    return Mesh(vertices, indices, textures);
}

std::vector<mesh::Texture> Model::loadMaterialTextures(const aiMaterial* mat, aiTextureType type, std::string type_name)
{
    std::vector<mesh::Texture> textures;
    for (uint32_t i = 0; i < mat->GetTextureCount(type); i++)  // 处理当前类型的所有材料 
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        bool skip = false;
        // 判断当前材料是不是以及被加载过 
        for (auto& j : m_texturesLoaded)
        {
            // 判断加载的文件是否以及被加载过 
            if (std::strcmp(j.m_path.data(), str.C_Str()) == 0)
            {
                textures.push_back(j);
                skip = true;
                break;
            }
        }

        if (!skip)
        {
            mesh::Texture texture;
            texture.m_id = Texture::textureFromFile(str.C_Str(), m_directory);
            texture.m_type = type_name;
            texture.m_path = str.C_Str();
            textures.push_back(texture);
            m_texturesLoaded.push_back(texture);
        }
    }

    return textures;
}
