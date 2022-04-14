#ifndef MODEL_H_
#define MODEL_H_

#include <iostream>
#include <string>
#include <vector>
#include "mesh.h"
#include "shader.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>


class Model final
{
public:
    explicit Model(char* path, bool gamma = false);
    void draw(const Shader* shader) const;
    ~Model();

public:
    std::vector<Mesh> m_meshes;
    std::string m_directory;
    std::vector<AssimpMesh::Texture> m_texturesLoaded;  // 记录以及加载的纹理 
    bool m_gammaCorrection;
    char* m_path;

private:
    // function
    void loadModel(const std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<AssimpMesh::Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string type_name);

};

#endif
