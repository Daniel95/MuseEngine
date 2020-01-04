#include "MusePCH.h"
#include "OpenGLShader.h"
#include "glad/glad.h"
#include "Core/Utilities/Log.h"
#include "Core/Utilities/Defines.h"
#include "glm/gtc/type_ptr.hpp"

#include <fstream>
#include <filesystem>

namespace Muse
{
	static GLenum ShaderTypeFromString(const std::string& a_Type)
	{
		if (a_Type == "vertex")
		{
			return GL_VERTEX_SHADER;
		    
		}
		if (a_Type == "fragment" || a_Type == "pixel")
		{
			return GL_FRAGMENT_SHADER;
		}

		ASSERT_ENGINE(false, "Unknown shader type!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& a_FilePath)
	{
		if (std::filesystem::exists(a_FilePath))
		{
			std::string source = ReadFile(a_FilePath);
			auto shaderSources = Preprocess(source);
			Compile(shaderSources);
		}
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		ASSERT_ENGINE(m_IsCompiled, "This shader is not compiled yet!");

		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		ASSERT_ENGINE(m_IsCompiled, "This shader is not compiled yet!");

		glUseProgram(0);
	}

    ///Improvement: cache the uniform location in a map, id is a_Name.
    void OpenGLShader::SetInt(const std::string& a_Name, int a_Int)
    {
		GLint location = glGetUniformLocation(m_RendererID, a_Name.c_str());
		ASSERT(location != -1, "Uniform not found!");
		glUniform1i(location, a_Int);
    }

    void OpenGLShader::SetFloat1(const std::string& a_Name, float a_Float)
    {
		GLint location = glGetUniformLocation(m_RendererID, a_Name.c_str());
		ASSERT(location != -1, "Uniform not found!");
		glUniform1f(location, a_Float);
    }

    void OpenGLShader::SetFloat2(const std::string& a_Name, const glm::vec2& a_Float2)
    {
		GLint location = glGetUniformLocation(m_RendererID, a_Name.c_str());
		ASSERT(location != -1, "Uniform not found!");
		glUniform2f(location, a_Float2.x, a_Float2.y);
    }

    void OpenGLShader::SetFloat3(const std::string& a_Name, const glm::vec3& a_Float3)
    {
		GLint location = glGetUniformLocation(m_RendererID, a_Name.c_str());
		ASSERT(location != -1, "Uniform not found!");
		glUniform3f(location, a_Float3.x, a_Float3.y, a_Float3.z);
    }

	void OpenGLShader::SetFloat4(const std::string& a_Name, const glm::vec4& a_Float4)
	{
		GLint location = glGetUniformLocation(m_RendererID, a_Name.c_str());
		ASSERT(location != -1, "Uniform not found!");
		glUniform4f(location, a_Float4.x, a_Float4.y, a_Float4.z, a_Float4.w);
	}

    void OpenGLShader::SetMat3(const std::string& a_Name, const glm::mat3& a_Mat3)
    {
		GLint location = glGetUniformLocation(m_RendererID, a_Name.c_str());
		ASSERT(location != -1, "Uniform not found!");
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(a_Mat3));
    }

	void OpenGLShader::SetMat4(const std::string& a_Name, const glm::mat4& a_Mat4)
    {
		GLint location = glGetUniformLocation(m_RendererID, a_Name.c_str());
		ASSERT(location != -1, "Uniform not found!");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(a_Mat4));
	}

    void OpenGLShader::Compile(const std::string& a_VertexSource, const std::string& a_FragmentSource)
    {
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = a_VertexSource;
		sources[GL_FRAGMENT_SHADER] = a_FragmentSource;
		Compile(sources);
    }

    std::string OpenGLShader::ReadFile(const std::string& a_FilePath)
    {
		std::string result;
		std::ifstream in(a_FilePath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			size_t size = in.tellg();
			if (size != -1)
			{
				result.resize(size);
				in.seekg(0, std::ios::beg);
				in.read(&result[0], size);
				in.close();
			}
			else
			{
				LOG_ERROR("Could not read from file '{0}'", a_FilePath);
			}
		}
		else
		{
			LOG_ERROR("Could not open file '{0}'", a_FilePath);
		}

		return result;
    }

	std::unordered_map<GLenum, std::string> OpenGLShader::Preprocess(const std::string& a_Source)
    {
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = a_Source.find(typeToken, 0); //Start of shader type declaration line
		while (pos != std::string::npos)
		{
			size_t eol = a_Source.find_first_of("\r\n", pos); //End of shader type declaration line
			ASSERT_ENGINE(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1; //Start of shader type name (after "#type " keyword)
			std::string type = a_Source.substr(begin, eol - begin);
			ASSERT_ENGINE(ShaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = a_Source.find_first_not_of("\r\n", eol); //Start of shader code after shader type declaration line
			ASSERT_ENGINE(nextLinePos != std::string::npos, "Syntax error");
			pos = a_Source.find(typeToken, nextLinePos); //Start of next shader type declaration line

			if(pos == std::string::npos)
			{
				shaderSources[ShaderTypeFromString(type)] = a_Source.substr(nextLinePos);
			}
		    else
			{
				shaderSources[ShaderTypeFromString(type)] = a_Source.substr(nextLinePos, pos - nextLinePos);
			}
		}

		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& a_ShaderSources)
	{
		m_IsCompiled = true;

		GLuint program = glCreateProgram();
		ASSERT_ENGINE(a_ShaderSources.size() <= 2, "We only support 2 shaders for now");
		std::array<GLenum, 2> glShaderIDs;
		int glShaderIDIndex = 0;
		for (auto& kv : a_ShaderSources)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;

			GLuint shader = glCreateShader(type);

			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				LOG_ENGINE_ERROR("{0}", infoLog.data());
				ASSERT_ENGINE(false, "Shader compilation failure!");
				break;
			}

			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}

		m_RendererID = program;

		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);

			for (auto id : glShaderIDs)
				glDeleteShader(id);

			LOG_ENGINE_ERROR("{0}", infoLog.data());
			ASSERT_ENGINE(false, "Shader link failure!");
			return;
		}

		for (auto id : glShaderIDs)
		{
			glDetachShader(program, id);
			glDeleteShader(id);
		}
	}
}
