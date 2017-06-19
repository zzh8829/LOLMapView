#ifndef Z_TEXTURE_H_
#define Z_TEXTURE_H_

#include "Core.h"
#include "GL/glew.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

class Texture
{
public:
  Texture()
    :width_(0),height_(0),texture_(0)
  {
  }
  Texture(GLuint texture)
    :texture_(texture)
  {
  }
  Texture(int width,int height,GLenum format,void* data)
    :width_(width),height_(height)
  {
    glGenTextures(1,&texture_);

    glBindTexture(GL_TEXTURE_2D,texture_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
    glTexImage2D(
      GL_TEXTURE_2D, 0,           /* target, level of detail */
      GL_RGBA,                    /* internal format */
      width, height, 0,           /* width, height, border */
      format, GL_UNSIGNED_BYTE,   /* external format, type */
      data                   /* pixels */
    );
    glGenerateMipmap(GL_TEXTURE_2D);
    //glBindTexture(GL_TEXTURE_2D,0);
  }

  GLuint GetTexture()
  {
    return texture_;
  }

  void Bind()
  {
    glBindTexture(GL_TEXTURE_2D,texture_);
  }

  static Texture CreateTextureFromFile(const char* filename)
  {
    SDL_Surface* sf = IMG_Load(filename);
    if(!sf)
      return Texture();
    GLenum format = GL_RGBA;
    if(sf->format->BytesPerPixel == 4)
    {
      if(sf->format->Rmask != 0x000000ff)
        format = GL_BGRA;
    } else {
      if(sf->format->Rmask == 0x000000ff)
        format = GL_RGB;
      else
        format = GL_BGR;
    }
    Texture tex = Texture(sf->w,sf->h,format,sf->pixels);
    //std::cerr << "Load Texture: " << filename << " " << tex.GetTexture() << std::endl;
    SDL_FreeSurface(sf);
    return tex;
  }

private:
  int width_,height_;
  GLuint texture_;
};

#endif
