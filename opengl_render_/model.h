#ifndef MODEL_H_
#define MODEL_H_

#include <iostream>
#include <string>
#include <vector>
#include "mesh.h"
#include "shader.h"

// assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


class Model final
{
public:
	explicit Model(char * path,bool gamma = false);
	void draw(const Shader * shader) const;
	~Model();

public:
	std::vector<Mesh> meshes_;
	std::string directory_;
	std::vector<AssimpMesh::Texture> textures_loaded_;  // 记录以及加载的纹理 
	bool gamma_correction_;

private:

	// function
	void loadModel(const std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<AssimpMesh::Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string type_name);

};

#endif
 