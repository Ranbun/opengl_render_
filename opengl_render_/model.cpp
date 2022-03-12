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
	// ����ģ�͵�Assimp�����ݽṹ
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);  // �����ļ� ������Ԥ������Ŀ
	// aiProcess_Triangulate  ��ģ�͵�ͼԪת��Ϊ������
	// aiProcess_FlipUVs      ��תY�����������
	// ����Ԥ���ɷ���
	// ���Խ�������Ŀ�϶������ָ�Ϊ���
	// ��С����ƴ�� 

	// ���س���ʧ�� ��鳡�� �Լ�ĳ����־  ���ڵ��Ƿ�Ϊ�� 
	if(!scene || scene->mFlags && AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR: ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}

	// ��¼ģ�͵��ļ��� 
	directory_ = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);

}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	// ������ǰ�ڵ�
	// ����ǰ�ڵ������ 
	for(auto i = 0;i < node->mNumMeshes;i++)
	{
		// Scene�µ�mMeshes���鴢����������Mesh����
		// �ڵ��е�mMeshes���鱣���ֻ�ǳ������������������
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		// �������� 
		meshes_.push_back(processMesh(mesh, scene));
	}
	// �����ڵ���ӽڵ�
	for(auto i = 0; i < node->mNumChildren;i++)
	{
		processNode(node->mChildren[i], scene);
	}


}

// ���� mesh ���� 
Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<AssimpMesh::Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<AssimpMesh::Texture> textures;

	for(auto i = 0;i < mesh->mNumVertices; i++)
	{
		AssimpMesh::Vertex vertex;
		// ��Ӷ��� ���� ��������
		vertex.position_.x = mesh->mVertices[i].x;
		vertex.position_.y = mesh->mVertices[i].y;
		vertex.position_.z = mesh->mVertices[i].z;

		if (mesh->HasNormals())
		{
			vertex.normal_.x = mesh->mNormals[i].x;
			vertex.normal_.y = mesh->mNormals[i].y;
			vertex.normal_.z = mesh->mNormals[i].z;
		}

		// ��Ҫ�ж�������������
		// һ����� ����ʹ�õ�һ����������(�����ж�����������) 
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;

			vertex.tex_coords_ = vec;

			glm::vec3 vector;

			if (mesh->HasTangentsAndBitangents())
			{
				// ����
				vector.x = mesh->mTangents[i].x;
				vector.y = mesh->mTangents[i].y;
				vector.z = mesh->mTangents[i].z;
				vertex.tangent_ = vector;

				// ������ 
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

	// ���� ���� 
	for(auto i = 0; i < mesh->mNumFaces;i++)  // ѭ������ÿ����
	{
		aiFace face = mesh->mFaces[i];
		for (auto j = 0; j < face.mNumIndices;j++) // ����ÿ��������� 
		{
			indices.push_back(face.mIndices[j]);  // ��������ӵ����� 
		}
	}

	// �������
	if(mesh->mMaterialIndex >= 0)
	{
		// ��ȡʵ�ʵĲ��� -- ��ǰ��������в�����Ϣ 
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
	for(auto i = 0;i < mat->GetTextureCount(type);i++)  // ����ǰ���͵����в��� 
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		bool skip = false;
		// �жϵ�ǰ�����ǲ����Լ������ع� 
		for(auto j =0; j < textures_loaded_.size();j++)
		{
			// �жϼ��ص��ļ��Ƿ��Լ������ع� 
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
