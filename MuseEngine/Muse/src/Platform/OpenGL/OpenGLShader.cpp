#include "MusePCH.h"
#include "OpenGLShader.h"
#include "glad/glad.h"
#include "Core/Utilities/Log.h"
#include "Core/Utilities/Defines.h"
#include "glm/gtc/type_ptr.hpp"

namespace Muse
{
	OpenGLShader::OpenGLShader(const std::string& a_VertexSrc, const std::string& a_FragmentSrc)
	{
		// Create an empty vertex shader handle
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		const GLchar* source = a_VertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);

		// Compile the vertex shader
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(vertexShader);

			// Debug the code.
			LOG_ENGINE_ERROR("{0}", infoLog.data());
			ASSERT(false, "Vertex shader compilation failure!");
			return;
		}

		// Create an empty fragment shader handle
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = a_FragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		// Compile the fragment shader
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);

			// Debug the code.
			LOG_ENGINE_ERROR("{0}", infoLog.data());
			ASSERT(false, "Fragment shader compilation failure!");
			return;
		}

		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		m_RendererID = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(m_RendererID, vertexShader);
		glAttachShader(m_RendererID, fragmentShader);

		// Link our program
		glLinkProgram(m_RendererID);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(m_RendererID);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			// Debug the code.
			LOG_ENGINE_ERROR("{0}", infoLog.data());
			ASSERT(false, "Shader link compilation failure!");
			return;
		}

		// Always detach shaders after a successful link.
		glDetachShader(m_RendererID, vertexShader);
		glDetachShader(m_RendererID, fragmentShader);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	///Improvement: cache the uniform location in a map, id is a_Name.
    void OpenGLShader::UploadUniformInt(const std::string& a_Name, int a_Int) const
    {
		GLint location = glGetUniformLocation(m_RendererID, a_Name.c_str());
		ASSERT(location != -1, "Uniform not found!");
		glUniform1i(location, a_Int);
    }

    void OpenGLShader::UploadUniformFloat(const std::string& a_Name, float a_Float) const
    {
		GLint location = glGetUniformLocation(m_RendererID, a_Name.c_str());
		ASSERT(location != -1, "Uniform not found!");
		glUniform1f(location, a_Float);
    }

    void OpenGLShader::UploadUniformFloat2(const std::string& a_Name, const glm::vec2& a_Float2) const
    {
		GLint location = glGetUniformLocation(m_RendererID, a_Name.c_str());
		ASSERT(location != -1, "Uniform not found!");
		glUniform2f(location, a_Float2.x, a_Float2.y);
    }

    void OpenGLShader::UploadUniformFloat3(const std::string& a_Name, const glm::vec3& a_Float3) const
    {
		GLint location = glGetUniformLocation(m_RendererID, a_Name.c_str());
		ASSERT(location != -1, "Uniform not found!");
		glUniform3f(location, a_Float3.x, a_Float3.y, a_Float3.z);
    }

	void OpenGLShader::UploadUniformFloat4(const std::string& a_Name, const glm::vec4& a_Float4) const
	{
		GLint location = glGetUniformLocation(m_RendererID, a_Name.c_str());
		ASSERT(location != -1, "Uniform not found!");
		glUniform4f(location, a_Float4.x, a_Float4.y, a_Float4.z, a_Float4.w);
	}

    void OpenGLShader::UploadUniformMat3(const std::string& a_Name, const glm::mat3& a_Matrix) const
    {
		GLint location = glGetUniformLocation(m_RendererID, a_Name.c_str());
		ASSERT(location != -1, "Uniform not found!");
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(a_Matrix));
    }

	void OpenGLShader::UploadUniformMat4(const std::string& a_Name, const glm::mat4& a_Matrix) const
    {
		GLint location = glGetUniformLocation(m_RendererID, a_Name.c_str());
		ASSERT(location != -1, "Uniform not found!");
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(a_Matrix));
	}
}
