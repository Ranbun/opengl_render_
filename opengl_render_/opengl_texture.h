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
	explicit Texture(std::string& path);        // ��ȡ�ļ� 
	~Texture();                                  // �������� 
	void bindTexture() const;                    // bind
	void createTexture();                        //create
	// set wrap & filter
	void setWarpParameteri(const texture::WRAP wrap, unsigned int type);
	void setFilterParameteri(texture::FILETER filter, unsigned int type);

	void releaseTexture() const;                         // �����ǰ����İ� 
	void flipVerticallyImage();                          // ��תͼ��
	unsigned int textureId() const;                      // ���ش����������ID

private:
	// �y��ĵ�ͼƬ��Ϣ
	int width_{};               // ���
	int height_{};              // �߶� 
	int nr_channels_{};         // ͨ����Ϣ

	bool init_;                 // ��ʼ����ǰ���� -- ��¼�����Ƿ񱻴��� 

	// ����create֮ǰ�ݴ�� ͼ������ 
	unsigned char* data_;      //�ݴ� ���ص�����

	// wrap -- ���Ʒ�ʽ 
	GLuint wrap_s_;
	GLuint wrap_t_;

	// filter -- ����
	GLuint filter_min_;
	GLuint filter_mag_;

	// texture ID  
	unsigned int texture_id_{};

public:
	// ʹ�� stb_image ��ȡһ������ ���������ID 
	static unsigned int textureFromFile(std::string path, std::string& directory,bool gamma = false);
	// load picture as texture 
	static  unsigned int loadTexture(char const* path);
};



#endif // !1
