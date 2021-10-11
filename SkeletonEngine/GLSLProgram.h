#pragma once
#include <string>
#include <GL/glew.h>

namespace SkeletonEngine
{

	class GLSLProgram
	{
	public:
		GLSLProgram();
		~GLSLProgram();

		void compileShaders(const std::string& vertex_shader_path, const std::string& fragment_shader_path);
		void linkShaders();
		void addAttribute(const std::string& attrib_name);
		void use();
		void unuse();

		GLint getUniformLocation(const std::string& uniform_name);

	private:
		int num_attrib_;
		GLuint program_id_;
		GLuint vertex_shader_id_;
		GLuint fragment_shader_id_;

		void compileShader(const std::string& shader_path, const GLuint id);
	};

}
