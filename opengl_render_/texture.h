#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <string>
#include <glad/glad.h>

#include "enum.h"
#include "stb_image.h"

using namespace oglRender;

class Texture
{
public:
	explicit Texture(const std::string& path);  // ��ȡ�ļ� 
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

	// ��תͼ��
	static void flipVerticallyImage();

	unsigned int textureId() const;

private:
	// �y��ĵ�ͼƬ��Ϣ
	int m_width;
	int m_height;
	// ͨ����Ϣ 
	int m_nrChannels;
	// ����create֮ǰ�ݴ�� ͼ������ 
	unsigned char* m_data;  //�ݴ� ���ص�����

	// wrap 
	GLuint m_wrapS;
	GLuint m_wrapT;

	// filter 
	GLuint m_filterMin;
	GLuint m_filterMag;

	// texture ID  
	unsigned int m_textureId{0};

public:
	// ʹ�� stb_image ��ȡһ������ ���������ID 
	static unsigned int textureFromFile(const std::string& path, const std::string& directory,bool gamma = false);
	// load picture as texture 
	static  unsigned int loadTexture(char const* path);
};



#endif // !1
