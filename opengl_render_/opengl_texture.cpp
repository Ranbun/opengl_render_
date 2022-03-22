#include "opengl_texture.h"

Texture::Texture(std::string& path)
	: init_(false)
	, data_(nullptr)
	, wrap_s_(GL_REPEAT)
	, wrap_t_(GL_REPEAT)
	, filter_min_(GL_LINEAR_MIPMAP_LINEAR)
	, filter_mag_(GL_LINEAR)
{
	data_ = stbi_load(path.c_str(), &width_, &height_, &nr_channels_, 0);
	glGenTextures(1, &texture_id_);

	// 调用创建 -- 记录状态 
	init_ = true;
}

Texture::~Texture() = default;

void Texture::bindTexture() const
{
	glBindTexture(GL_TEXTURE_2D, texture_id_);
}

void Texture::createTexture()
{
	if (data_)
	{
		// 绑定到当前创建的纹理 
		bindTexture();

		if (nr_channels_ < 4)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, data_);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, data_);
		}

		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s_);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t_);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min_);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_mag_);

	}
	else
	{
		std::cout << "Failed to create texture" << std::endl;
	}

	stbi_image_free(data_);
	data_ = nullptr;
}

void Texture::setWarpParameteri(const texture::WRAP wrap, unsigned int type)
{
	if (wrap == texture::WRAP::S)
	{
		wrap_s_ = type;
	}
	else if (wrap == texture::WRAP::T)
	{
		wrap_t_ = type;
	}
}

void Texture::setFilterParameteri(texture::FILETER filter, unsigned int type)
{
	if (filter == texture::FILETER::MIN)
	{
		filter_min_ = type;
	}
	else
	{
		filter_mag_ = type;
	}
}

void Texture::releaseTexture() const
{
	if (init_)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Texture::flipVerticallyImage()
{
	// 需要纹理被加载 -- 即照片被加载
	if (!init_)
		return;

	stbi_set_flip_vertically_on_load(true);

}

unsigned Texture::textureId() const
{
	return texture_id_;
}

unsigned Texture::textureFromFile(std::string path, std::string & directory, bool gamma)
{
	std::string file_name = std::string(path);
	file_name = directory + '/' + file_name;

	unsigned int texture_id;
	glGenTextures(1, &texture_id);

	int width, height, nr_components;
	unsigned char* data = stbi_load(file_name.c_str(), &width, &height, &nr_components, 0);

	if (data)
	{
		GLenum format = GL_RGB;
		if (nr_components == 1)
		{
			format = GL_RED;
		}
		else if (nr_components == 3)
		{
			format = GL_RGB;
		}
		else if (nr_components == 4)
		{
			format = GL_RGBA;
		}

		glBindTexture(GL_TEXTURE_2D, texture_id);
		glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(format), width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);

	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return texture_id;
}

unsigned Texture::loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format = GL_RGB;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(format), width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

