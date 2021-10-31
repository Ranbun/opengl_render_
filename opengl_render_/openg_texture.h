#ifndef OPENGL_TEXTURE_H
#define OPENGL_TEXTURE_H

#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GL/GL.h>
#include "render_en.h"
#include "stb_image.h"


#if 0
glGenTextures(1, &texture_);
glBindTexture(GL_TEXTURE_2D, texture_);

glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

int width, height, nr_channels;
unsigned char* data = stbi_load("./container.jpg", &width, &height, &nr_channels, 0);

if (data)
{
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
}
else
{
	std::cout << "Failed to load texture" << std::endl;
}

stbi_image_free(data);

#endif 

class Texture
{
public:
	explicit Texture(std::string& path);  // 读取文件 

	~Texture();


	void bindTexture() const;

	void createTexture();

	void setWarpParameteri(const texture::WRAP wrap, unsigned int type);
	void setFilterParameteri(texture::FILETER filter, unsigned int type);

	void releaseTexture();


private:
	
	int width_;
	int height_;
	int nr_channels_;
	unsigned char* data_;  //暂存 像素的数据

	GLuint wrap_s_;
	GLuint wrap_t_;

	GLuint filter_min_;
	GLuint filter_mag_;



	unsigned int texture_id_;

};



#endif // !1
