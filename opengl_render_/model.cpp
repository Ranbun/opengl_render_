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
    // ����ģ�͵�Assimp�����ݽṹ
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);  // �����ļ� ������Ԥ������Ŀ
    // aiProcess_Triangulate  ��ģ�͵�ͼԪת��Ϊ������
    // aiProcess_FlipUVs      ��תY�����������
    // ����Ԥ���ɷ���
    // ���Խ�������Ŀ�϶������ָ�Ϊ���
    // ��С����ƴ�� 

    // ���س���ʧ�� ��鳡�� �Լ�ĳ����־  ���ڵ��Ƿ�Ϊ�� 
    if ((!scene || scene->mFlags) && (AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode))
    {
        std::cout << "ERROR: ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }

    // ��¼ģ�͵��ļ��� 
    m_directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);

}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    // ������ǰ�ڵ�
    // ����ǰ�ڵ������ 
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // Scene�µ�mMeshes���鴢����������Mesh����
        // �ڵ��е�mMeshes���鱣���ֻ�ǳ������������������
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

        // �������� 
        m_meshes.push_back(processMesh(mesh, scene));
    }
    // �����ڵ���ӽڵ�
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }


}

// ���� mesh ���� 
Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<mesh::Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<mesh::Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        mesh::Vertex vertex;
        // ��Ӷ��� ���� ��������
        vertex.m_position.x = mesh->mVertices[i].x;
        vertex.m_position.y = mesh->mVertices[i].y;
        vertex.m_position.z = mesh->mVertices[i].z;

        if (mesh->HasNormals())
        {
            vertex.m_normal.x = mesh->mNormals[i].x;
            vertex.m_normal.y = mesh->mNormals[i].y;
            vertex.m_normal.z = mesh->mNormals[i].z;
        }

        // ��Ҫ�ж�������������
        // һ����� ����ʹ�õ�һ����������(�����ж�����������) 
        if (mesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;

            vertex.m_texCoords = vec;

            glm::vec3 vector;

            if (mesh->HasTangentsAndBitangents())
            {
                // ����
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.m_tangent = vector;

                // ������ 
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

    // ���� ���� 
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)  // ѭ������ÿ����
    {
        aiFace face = mesh->mFaces[i];
        for (uint32_t j = 0; j < face.mNumIndices; j++) // ����ÿ��������� 
        {
            indices.push_back(face.mIndices[j]);  // ��������ӵ����� 
        }
    }

    // �������
    if (mesh->mMaterialIndex >= 0)
    {
        // ��ȡʵ�ʵĲ��� -- ��ǰ��������в�����Ϣ 
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
    for (uint32_t i = 0; i < mat->GetTextureCount(type); i++)  // ����ǰ���͵����в��� 
    {
        aiString str;
        mat->GetTexture(type, i, &str);

        bool skip = false;
        // �жϵ�ǰ�����ǲ����Լ������ع� 
        for (auto& j : m_texturesLoaded)
        {
            // �жϼ��ص��ļ��Ƿ��Լ������ع� 
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
