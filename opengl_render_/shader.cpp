#include "shader.h"
#include <glm/gtc/type_ptr.hpp>


Shader::Shader(const char* vertex_path, const char* fragment_path, const char* geometry_path)
{
	// 读取 着色器的源码文件
	std::string vertex_code;
	std::string fragment_code;
	std::string geometry_code;
	std::ifstream v_shader_file;
	std::ifstream f_shader_file;
	std::ifstream g_shader_file;

	v_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	f_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	g_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// 打开文件
		v_shader_file.open(vertex_path);
		f_shader_file.open(fragment_path);

		// 创建字符串流对象 -- 存储文件中的数据
		std::stringstream v_shader_stream, f_shader_stream;

		// 将文件读入到流中 
		v_shader_stream << v_shader_file.rdbuf();
		f_shader_stream << f_shader_file.rdbuf();

		// 关闭文件
		v_shader_file.close();
		f_shader_file.close();

		vertex_code = v_shader_stream.str();
		fragment_code = f_shader_stream.str();

		if (geometry_path != nullptr)
		{
			g_shader_file.open(geometry_path);
			std::stringstream geometry_stream;
			geometry_stream << g_shader_file.rdbuf();
			g_shader_file.close();
			geometry_code = geometry_stream.str();
		}
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << e.what() << std::endl;
		std::cout << "ERROR::SHADER:FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	// 获取C风格的字符串
	const char* vertex_shader_source = vertex_code.c_str();
	const char* fragment_shader_source = fragment_code.c_str();

	// compile  shader  -- 编译着色器
	unsigned int vertex, fragment;

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vertex_shader_source, nullptr);
	glCompileShader(vertex);
	checkCompileErrors(vertex, SHADER_TYPE::VERTEX_SHADER);

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fragment_shader_source, nullptr);
	glCompileShader(fragment);
	checkCompileErrors(fragment, SHADER_TYPE::FRAGMENT_SHADER);

	unsigned int geometry;
	if (geometry_path != nullptr)
	{
		const char* geometry_shader_source = geometry_code.c_str();
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &geometry_shader_source, nullptr);
		checkCompileErrors(geometry, SHADER_TYPE::GEOMETRY_SHADER);
	}

	// 着色器程序
	program_ = glCreateProgram();
	glAttachShader(program_, vertex);
	glAttachShader(program_, fragment);

	if (geometry_path)
	{
		glAttachShader(program_, geometry);
	}

	glLinkProgram(program_);
	checkCompileErrors(program_, SHADER_TYPE::PROGRAM);

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::~Shader()
{
	glDeleteProgram(program_);
}

void Shader::use() const
{
	glUseProgram(program_);
}

void Shader::setBool(const std::string& name, const bool value) const
{
	const auto location = glGetUniformLocation(program_, name.c_str());
	glUniform1i(location, static_cast<int>(value));
}

void Shader::setInt(const std::string& name, const int value) const
{
	const auto location = glGetUniformLocation(program_, name.c_str());
	glUniform1i(location, value);
}

void Shader::setFloat(const std::string& name, const float value) const
{
	const auto location = glGetUniformLocation(program_, name.c_str());
	glUniform1f(location, value);
}

void Shader::setVec2(const std::string& name, glm::vec2& value) const
{
	const auto location = glGetUniformLocation(program_, name.c_str());
	glUniform2fv(location, 1, &value[0]);
}

void Shader::setVec2(const std::string& name, const float x, const float y) const
{
	const auto location = glGetUniformLocation(program_, name.c_str());
	glUniform2f(location, x,y);
}

void Shader::setVec3(const std::string& name, glm::vec3& value) const
{
	const auto location = glGetUniformLocation(program_, name.c_str());
	glUniform3fv(location, 1,&value[0]);
}

void Shader::setVec3(const std::string& name, const float x, const float y, const float z) const
{
	const auto location = glGetUniformLocation(program_, name.c_str());
	glUniform3f(location, x, y, z);
}

void Shader::setVec4(const std::string& name, glm::vec4& value) const
{
	const auto location = glGetUniformLocation(program_, name.c_str());
	glUniform4fv(location, 1, &value[0]);
}

void Shader::setVec4(const std::string& name, const float x, const float y, const float z, const float w) const
{
	const auto location = glGetUniformLocation(program_, name.c_str());
	glUniform4f(location, x, y, z, w);
}

void Shader::setMat2(const std::string& name, glm::mat2& value) const
{
	const auto location = glGetUniformLocation(program_, name.c_str());
	glUniformMatrix2fv(location, 1, GL_FALSE, &value[0][0]);
}

void Shader::setMat3(const std::string& name, glm::mat3& value) const
{
	unsigned int location = glGetUniformLocation(program_, name.c_str());
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setMat4(const std::string& name, glm::mat4& value) const
{
	unsigned int location = glGetUniformLocation(program_, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::checkCompileErrors(const unsigned shader, const SHADER_TYPE type)
{
	int success;
	char info_log[1024];
	switch (type)
	{
	case SHADER_TYPE::VERTEX_SHADER:
	case SHADER_TYPE::FRAGMENT_SHADER:
	case SHADER_TYPE::GEOMETRY_SHADER:
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, nullptr, info_log);
			std::cout << "ERROR::SHADER::COMPILATION_FAILED of type: " << (type == SHADER_TYPE::VERTEX_SHADER
				                                                               ? "VERTEX"
				                                                               : (type == SHADER_TYPE::FRAGMENT_SHADER
					                                                               ? "FRAGMENT"
					                                                               : "GEOMETRY")) << "\n" <<
				info_log << std::endl;
		}
		break;
	case SHADER_TYPE::PROGRAM:
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, nullptr, info_log);
			std::cout << "ERROR::PROGRAM::LINK_FAILED of type: " << "PROGRAM" << "\n" << info_log << std::endl;
		}
		break;
	}
}
