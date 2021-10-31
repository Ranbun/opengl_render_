#ifndef OPENGL_TEXTURE_H
#define OPENGL_TEXTURE_H

#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GL/GL.h>
#include "render_en.h"
#include "stb_image.h"

class Texture
{
public:
	explicit Texture(std::string& path);  // 读取文件 

	~Texture();

	// bind 
	void bindTexture() const;

	//create 
	void createTexture();

	// set wrap & filter
	void setWarpParameteri(const texture::WRAP wrap, unsigned int type);
	void setFilterParameteri(texture::FILETER filter, unsigned int type);

	// release
	void releaseTexture();


private:
	
	// 紋理的的图片信息
	int width_;
	int height_;
	// 通道信息 
	int nr_channels_;
	// 调用create之前暂存的 图像数据 
	unsigned char* data_;  //暂存 像素的数据

	// wrap 
	GLuint wrap_s_;
	GLuint wrap_t_;

	// filter 
	GLuint filter_min_;
	GLuint filter_mag_;

	// texture ID  
	unsigned int texture_id_;

};



#endif // !1
