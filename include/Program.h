#ifndef Z_PROGRAM_H_
#define Z_PROGRAM_H_

#include "Core.h"
#include "gl/glew.h"
#include "Shader.h"

class Program
{
public:
	enum
	{
		POSITION = 0,
		NORMAL,
		UV0,
		UV1,
		COLOR0,
		COLOR1,
		BLENDINDEX,
		BLENDWEIGHT
	} AttributeLocation;

	Program()
	{
		program_ = glCreateProgram();
	}

	void AttachShader(const Shader& shader)
	{
		glAttachShader(program_,shader.GetShader());
	}

	void Link()
	{
		glBindAttribLocation(program_, POSITION,	"Z_POSITION");
		glBindAttribLocation(program_, NORMAL,		"Z_NORMAL");
		glBindAttribLocation(program_, UV0,			"Z_UV0");
		glBindAttribLocation(program_, UV1,			"Z_UV1");
		glBindAttribLocation(program_, COLOR0,		"Z_COLOR0");
		glBindAttribLocation(program_, COLOR1,		"Z_COLOR1");
		glBindAttribLocation(program_, BLENDINDEX,	"Z_BLENDINDEX");
		glBindAttribLocation(program_, BLENDWEIGHT,	"Z_BLENDWEIGHT");

		glLinkProgram(program_);

		GLint status;
		glGetProgramiv(program_, GL_LINK_STATUS, &status);
		if (!status) {
		
			GLint infoLogLength;
			glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &infoLogLength);

			GLchar* infoLog = new GLchar[infoLogLength + 1];
			glGetProgramInfoLog(program_, infoLogLength, NULL, infoLog);

			std::cerr << "Program linking error: " << infoLog << std::endl;;
			delete[] infoLog;
			glDeleteProgram(program_);
		}
	}

	void Use()
	{
		glUseProgram(program_);
	}

	GLint GetUniformLocation(const char* name)
	{
		return glGetUniformLocation(program_,name);
	}

	GLint GetAttribLocation(const char* name)
	{
		return glGetAttribLocation(program_,name);
	}

	GLint GetProgram() const
	{
		return program_;
	}

private:
	GLuint program_;

};

#endif