#ifndef Z_RENDERER_H_
#define Z_RENDERER_H_

#include "Core.h"
#include "Window.h"
#include "GL/glew.h"

class Renderer
{
public:
  Renderer(Window* window)
    :window_(window),width_(window->GetWidth()),height_(window->GetHeight())
  {
    glewInit();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable (GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    //glBlendFunc (GL_SRC_ALPHA, GL_SRC_ALPHA);
    //glBlendFunc( GL_SRC_ALPHA, GL_ONE );

    glEnable (GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.5);
    //glEnable(GL_CULL_FACE);

  }
  virtual ~Renderer(){};

  void Enable(GLenum cap)
  {
    glEnable(cap);
  }
  void Disable(GLenum cap)
  {
    glDisable(cap);
  }

  void Resize(int width,int height)
  {
    width_ = width;
    height_ = height;
  }

private:
  Window* window_;
  int width_,height_;
};

#endif
