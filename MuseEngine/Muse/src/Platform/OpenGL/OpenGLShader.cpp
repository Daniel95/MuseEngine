#include "MusePCH.h"
#include "OpenGLShader.h"
#include "Core/Resource/Resource.h"
#include "glad/glad.h"
#include "Core/Utilities/Log.h"
#include "Core/Utilities/Defines.h"
#include "glm/gtc/type_ptr.hpp"

#include <fstream>
#include <filesystem>
#include "Core/Instrumentor.h"

namespace Muse
{
	static GLenum ShaderTypeFromString(const std::string& a_Type)
	{
		MUSE_PROFILE_FUNCTION();

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

    OpenGLShader::OpenGLShader(const std::string& a_VertexSource, const std::string& a_FragmentSource)
    {
		MUSE_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = a_VertexSource;
		sources[GL_FRAGMENT_SHADER] = a_FragmentSource;
		Compile(sources);
    }

	OpenGLShader::OpenGLShader(const std::string& a_FilePath)
	{
		MUSE_PROFILE_FUNCTION();

		if (std::filesystem::exists(a_FilePath))
		{
			std::string source = ReadFile(a_FilePath);
			auto shaderSources = Preprocess(source);
			Compile(shaderSources);
		}
	}

	OpenGLShader::~OpenGLShader()
	{
		MUSE_PROFILE_FUNCTION();

		glDeleteProgram(m_RendererID);
	}

    void OpenGLShader::Bind() const
	{
		MUSE_PROFILE_FUNCTION();

		ASSERT_ENGINE(m_IsCompiled, "This shader is not compiled yet!");

		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		MUSE_PROFILE_FUNCTION();

		ASSERT_ENGINE(m_IsCompiled, "This shader is not compiled yet!");

		glUseProgram(0);
	}

    void OpenGLShader::SetInt(const std::string& a_Name, int a_Value)
    {
		MUSE_PROFILE_FUNCTION();

        UploadUniformInt(a_Name, a_Value);
    }

    void OpenGLShader::SetIntArray(const std::string& a_Name, int* a_Values, uint32_t a_Count)
    {
        UploadUniformIntArray(a_Name, a_Values, a_Count);
    }

    void OpenGLShader::SetFloat(const std::string& a_Name, float a_Value)
    {
		MUSE_PROFILE_FUNCTION();

        UploadUniformFloat(a_Name, a_Value);
    }

    void OpenGLShader::SetFloat3(const std::string& a_Name, const glm::vec3& a_Value)
    {
		MUSE_PROFILE_FUNCTION();

        UploadUniformFloat3(a_Name, a_Value);
    }

    void OpenGLShader::SetFloat4(const std::string& a_Name, const glm::vec4& a_Value)
    {
		MUSE_PROFILE_FUNCTION();

        UploadUniformFloat4(a_Name, a_Value);
    }

    void OpenGLShader::SetMat4(const std::string& a_Name, const glm::mat4& a_Value)
    {
		MUSE_PROFILE_FUNCTION();

        UploadUniformMat4(a_Name, a_Value);
    }
    void OpenGLShader::Compile(const std::string& a_VertexSource, const std::string& a_FragmentSource)
    {
		MUSE_PROFILE_FUNCTION();

		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = a_VertexSource;
		sources[GL_FRAGMENT_SHADER] = a_FragmentSource;
		Compile(sources);
    }

    std::string OpenGLShader::ReadFile(const std::string& a_FilePath)
    {
		MUSE_PROFILE_FUNCTION();

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
		MUSE_PROFILE_FUNCTION();

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
		MUSE_PROFILE_FUNCTION();

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

    void OpenGLShader::UploadUniformInt(const std::string& a_Name, int a_Value)
    {
        GLint location = glGetUniformLocation(m_RendererID, a_Name.c_str());
        glUniform1i(location, a_Value);
    }

    void OpenGLShader::UploadUniformIntArray(const std::string& a_Name, int* a_Values, uint32_t count)
    {
        GLint location = glGetUniformLocation(m_RendererID, a_Name.c_str());
        glUniform1iv(location, count, a_Values);
    }

    void OpenGLShader::UploadUniformFloat(const std::string& a_Name, float a_Value)
    {
        GLint location = glGetUniformLocation(m_RendererID, a_Name.c_str());
        glUniform1f(location, a_Value);
    }

    void OpenGLShader::UploadUniformFloat2(const std::string& a_Name, const glm::vec2& a_Value)
    {
        GLint location = glGetUniformLocation(m_RendererID, a_Name.c_str());
        glUniform2f(location, a_Value.x, a_Value.y);
    }

    void OpenGLShader::UploadUniformFloat3(const std::string& a_Name, const glm::vec3& a_Value)
    {
        GLint location = glGetUniformLocation(m_RendererID, a_Name.c_str());
        glUniform3f(location, a_Value.x, a_Value.y, a_Value.z);
    }

    void OpenGLShader::UploadUniformFloat4(const std::string& a_Name, const glm::vec4& a_Value)
    {
        GLint location = glGetUniformLocation(m_RendererID, a_Name.c_str());
        glUniform4f(location, a_Value.x, a_Value.y, a_Value.z, a_Value.w);
    }

    void OpenGLShader::UploadUniformMat3(const std::string& a_Name, const glm::mat3& a_Matrix)
    {
        GLint location = glGetUniformLocation(m_RendererID, a_Name.c_str());
        glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(a_Matrix));
    }

    void OpenGLShader::UploadUniformMat4(const std::string& a_Name, const glm::mat4& a_Matrix)
    {
        GLint location = glGetUniformLocation(m_RendererID, a_Name.c_str());
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(a_Matrix));
    }
}
