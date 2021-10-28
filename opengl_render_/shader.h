#ifndef SHADER_H
#define SHADER_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

enum class SHADER_TYPE
{
	VERTEX_SHADER,
	FRAGMENT_SHADER,
	GEOMETRY_SHADER,
	PROGRAM
	
};

class Shader
{
public:
	unsigned int program_; 
	Shader(const char* vertex_path, const char* fragment_path,const char * geometry_path = nullptr);
	~Shader();
	void use() const;
	void setBool(const std::string& name, const bool value) const;
	void setInt(const std::string& name,  const int value ) const;
	void setFloat(const std::string& name,  const float value ) const;
	void setVec2(const std::string& name, glm::vec2 & value) const;
	void setVec2(const std::string& name,const float x, const float y) const;

	void setVec3(const std::string& name, glm::vec3& value) const;
	void setVec3(const std::string& name, const float x, const float y,const float z) const;

	void setVec4(const std::string& name, glm::vec4& value) const;
	void setVec4(const std::string& name, const float x, const float y,const float z,const float w) const;

	void setMat2(const std::string& name, glm::mat2& value) const;
	void setMat3(const std::string& name, glm::mat3& value) const;
	void setMat4(const std::string& name, glm::mat4& value) const;



private:
	static void checkCompileErrors(const unsigned int shader, const SHADER_TYPE type);
};

#endif 