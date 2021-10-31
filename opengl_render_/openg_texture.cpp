#include "openg_texture.h"

Texture::Texture(std::string& path)
{
	data_ = nullptr;

	// 读取图片
	data_ = stbi_load(path.c_str(), &width_, &height_, &nr_channels_, 0);
	// 创建纹理对象 
	glGenTextures(1, &texture_id_);

	// 环绕方式
	wrap_s_ = GL_REPEAT;
	wrap_t_ = GL_REPEAT;

	// 过滤方式
	filter_min_ = GL_LINEAR_MIPMAP_LINEAR;
	filter_mag_ = GL_LINEAR;

}

Texture::~Texture()= default;

void Texture::bindTexture() const
{
	glBindTexture(GL_TEXTURE_2D, texture_id_);
}

void Texture::createTexture()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t_);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min_);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_mag_);

	// unsigned char* data = stbi_load("./container.jpg", &width_, &height_, &nr_channels_, 0);

	if (data_)
	{
		if(nr_channels_ < 4)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width_, height_, 0, GL_RGB, GL_UNSIGNED_BYTE, data_);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, data_);
		}

		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to create texture" << std::endl;
	}

	stbi_image_free(data_);
	data_ = nullptr;
}

void Texture::setWarpParameteri(const texture::WRAP wrap, unsigned type)
{
	if(wrap == texture::WRAP::S)
	{
		wrap_s_ = type;
	}
	else
	{
		wrap_t_ = type;
	}
}

void Texture::setFilterParameteri(texture::FILETER filter, unsigned type)
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

void Texture::releaseTexture()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}
