#include "model.h"
#include "stb_image.h"
#include "opengl_texture.h"

Model::Model(char* path, bool gamma)
	:gamma_correction_(gamma)
{
	loadModel(path);
}

void Model::draw(const Shader * shader) const
{

	for(auto & item: meshes_)
	{
		item.draw(shader);
	}
}

Model::~Model()
{
	meshes_.clear();
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
	if(!scene || scene->mFlags && AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR: ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}

	// 记录模型的文件夹 
	directory_ = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);

}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	// 解析当前节点
	// 处理当前节点的网格 
	for(auto i = 0;i < node->mNumMeshes;i++)
	{
		// Scene下的mMeshes数组储存了真正的Mesh对象
		// 节点中的mMeshes数组保存的只是场景中网格数组的索引
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		// 解析网格 
		meshes_.push_back(processMesh(mesh, scene));
	}
	// 处理当节点的子节点
	for(auto i = 0; i < node->mNumChildren;i++)
	{
		processNode(node->mChildren[i], scene);
	}


}

// 创建 mesh 对象 
Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<AssimpMesh::Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<AssimpMesh::Texture> textures;

	for(auto i = 0;i < mesh->mNumVertices; i++)
	{
		AssimpMesh::Vertex vertex;
		// 添加顶点 法线 纹理坐标
		vertex.position_.x = mesh->mVertices[i].x;
		vertex.position_.y = mesh->mVertices[i].y;
		vertex.position_.z = mesh->mVertices[i].z;

		if (mesh->HasNormals())
		{
			vertex.normal_.x = mesh->mNormals[i].x;
			vertex.normal_.y = mesh->mNormals[i].y;
			vertex.normal_.z = mesh->mNormals[i].z;
		}

		// 需要判断有纹理坐标吗？
		// 一般情况 我们使用第一组纹理坐标(允许有多组纹理坐标) 
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;

			vertex.tex_coords_ = vec;

			glm::vec3 vector;

			if (mesh->HasTangentsAndBitangents())
			{
				// 切线
				vector.x = mesh->mTangents[i].x;
				vector.y = mesh->mTangents[i].y;
				vector.z = mesh->mTangents[i].z;
				vertex.tangent_ = vector;

				// 副切线 
				vector.x = mesh->mBitangents[i].x;
				vector.y = mesh->mBitangents[i].y;
				vector.z = mesh->mBitangents[i].z;
				vertex.bitangent_ = vector;
		}
		}
		else
		{
			vertex.tex_coords_ = glm::vec2(0.0f, 0.0f);
		}

		vertices.push_back(vertex);
	}

	// 处理 索引 
	for(auto i = 0; i < mesh->mNumFaces;i++)  // 循环处理每个面
	{
		aiFace face = mesh->mFaces[i];
		for (auto j = 0; j < face.mNumIndices;j++) // 处理每个面的索引 
		{
			indices.push_back(face.mIndices[j]);  // 将索引添加到数组 
		}
	}

	// 处理材质
	if(mesh->mMaterialIndex >= 0)
	{
		// 获取实际的材料 -- 当前网格的所有材料信息 
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<AssimpMesh::Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<AssimpMesh::Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		std::vector<AssimpMesh::Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		// 4. height maps
		std::vector<AssimpMesh::Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	}


	return Mesh(vertices, indices, textures);
}

std::vector<AssimpMesh::Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string type_name)
{
	std::vector<AssimpMesh::Texture> textures;
	for(auto i = 0;i < mat->GetTextureCount(type);i++)  // 处理当前类型的所有材料 
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		bool skip = false;
		// 判断当前材料是不是以及被加载过 
		for(auto j =0; j < textures_loaded_.size();j++)
		{
			// 判断加载的文件是否以及被加载过 
			if(std::strcmp(textures_loaded_[j].path_.data(),str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded_[j]);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			AssimpMesh::Texture texture;
			texture.id_ = Texture::textureFromFile(str.C_Str(), directory_);
			texture.type_ = type_name;
			texture.path_ = str.C_Str();
			textures.push_back(texture);
			textures_loaded_.push_back(texture);
		}
	}

	return textures;
}
