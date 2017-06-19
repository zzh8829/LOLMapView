#ifndef Z_SHADER_H_
#define Z_SHADER_H_

#include "Core.h"
#include "GL/glew.h"

class Shader
{
public:
  Shader()
    :shader_(0)
  {

  }
  Shader(GLenum type, const char* source)
  {
    shader_ = glCreateShader(type);
    const GLchar* buffer = reinterpret_cast<const GLchar*>(source);
    glShaderSource(shader_,1,(const GLchar**)&(buffer),0);
    glCompileShader(shader_);

    GLint status;
    glGetShaderiv(shader_,GL_COMPILE_STATUS, &status);
    if(!status)
    {
      GLint infoLogLength;
      glGetShaderiv(shader_, GL_INFO_LOG_LENGTH, &infoLogLength);

      GLchar* infoLog = new GLchar[infoLogLength + 1];
      glGetShaderInfoLog(shader_, infoLogLength, NULL, infoLog);

      std::cerr << "Shader compilation error: " << infoLog << std::endl;;
      delete[] infoLog;
      glDeleteShader(shader_);
    }
  }
  GLint GetShader() const
  {
    return shader_;
  }
  static Shader CreateShaderFromFile(GLenum type, const char* filename)
  {
    std::ifstream fi(filename,ios::binary);
    return CreateShaderFromStream(type,fi);
  }
  static Shader CreateShaderFromStream(GLenum type, std::istream& inStream)
  {
    char* data;
    size_t size;
    inStream.seekg(0, std::ios::end);
    size = inStream.tellg();
    inStream.seekg(0, std::ios::beg);
    data = new char[size+1];
    inStream.read(data,size);
    data[size] = 0;
    return Shader(type,data);
  }

private:
  GLint shader_;
};

#endif
