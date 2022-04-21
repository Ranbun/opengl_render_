#ifndef MODEL_H_
#define MODEL_H_

#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>

#include "mesh.h"
#include "shader.h"


class Model final
{
public:
    explicit Model(char* path, bool gamma = false);
    void draw(const Shader* shader) const;
    ~Model();

public:
    std::vector<Mesh> m_meshes;
    std::string m_directory;
    std::vector<mesh::Texture> m_texturesLoaded;  // 记录以及加载的纹理 
    bool m_gammaCorrection;
    char* m_path;

private:
    // function
    void loadModel(const std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<mesh::Texture> loadMaterialTextures(const aiMaterial* mat, aiTextureType type, std::string type_name);

};

#endif
