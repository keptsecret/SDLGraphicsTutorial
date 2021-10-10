#include "GLSLProgram.h"

#include <fstream>
#include <iostream>

#include "Errors.h"

GLSLProgram::GLSLProgram() : num_attrib_(0), program_id_(0), vertex_shader_id_(0), fragment_shader_id_(0)
{
}

GLSLProgram::~GLSLProgram()
{
}

void GLSLProgram::compileShaders(const std::string& vertex_shader_path, const std::string& fragment_shader_path)
{
	program_id_ = glCreateProgram();

	vertex_shader_id_ = glCreateShader(GL_VERTEX_SHADER);
	if (vertex_shader_id_ == 0)
	{
		fatalError("Vertex shader failed to be created!");
	}

	fragment_shader_id_ = glCreateShader(GL_FRAGMENT_SHADER);
	if (fragment_shader_id_ == 0)
	{
		fatalError("Fragment shader failed to be created!");
	}

	compileShader(vertex_shader_path, vertex_shader_id_);
	compileShader(fragment_shader_path, fragment_shader_id_);
	
}

void GLSLProgram::linkShaders()
{
	glAttachShader(program_id_, vertex_shader_id_);
	glAttachShader(program_id_, fragment_shader_id_);

	glLinkProgram(program_id_);

	GLint success = 0;
	char error_log[512];
	glGetProgramiv(program_id_, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(program_id_, 512, nullptr, error_log);
		glDeleteProgram(program_id_);
		glDeleteShader(vertex_shader_id_);
		glDeleteShader(fragment_shader_id_);

		std::cout << error_log << std::endl;
		fatalError("Shaders failed to link!");
	}

	glDetachShader(program_id_, vertex_shader_id_);
	glDetachShader(program_id_, fragment_shader_id_);
	glDeleteShader(vertex_shader_id_);
	glDeleteShader(fragment_shader_id_);
}

void GLSLProgram::addAttribute(const std::string& attrib_name)
{
	glBindAttribLocation(program_id_, num_attrib_++, attrib_name.c_str());
}

void GLSLProgram::use()
{
	glUseProgram(program_id_);
	for (int i = 0; i < num_attrib_; i++)
	{
		glEnableVertexAttribArray(i);
	}
}

void GLSLProgram::unuse()
{
	glUseProgram(0);
	for (int i = 0; i < num_attrib_; i++)
	{
		glDisableVertexAttribArray(i);
	}
}

GLint GLSLProgram::getUniformLocation(const std::string& uniform_name)
{
	GLint location = glGetUniformLocation(program_id_, uniform_name.c_str());
	if (location == static_cast<GLint> (GL_INVALID_INDEX))
	{
		fatalError("Uniform " + uniform_name + " not found in shader!");
	}
	return location;
}

void GLSLProgram::compileShader(const std::string& shader_path, const GLuint id)
{  
	std::fstream vertex_file(shader_path);
	if (vertex_file.fail())
	{
		perror(shader_path.c_str());
		fatalError("Failed to open " + shader_path);
	}

	std::string file_content;
	std::string line;

	while (std::getline(vertex_file, line))
	{
		file_content += line + "\n";
	}

	vertex_file.close();

	const char* file_content_ptr = file_content.c_str();
	glShaderSource(id, 1, &file_content_ptr, nullptr);
	glCompileShader(id);

	GLint success = 0;
	char error_log[512];
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(id, 512, nullptr, error_log);
		glDeleteShader(id);

		std::cout << error_log << std::endl;
		fatalError("Shader " + shader_path + " failed to compile!");
	}

}
