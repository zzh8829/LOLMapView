#ifndef Z_WINDOW_H_
#define Z_WINDOW_H_

#include "Core.h"

#include "GL/glew.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"
#include "SDL2/SDL_image.h"

class Window
{
public:
  Window(int width,int height,const char* title)
    :width_(width),height_(height)
  {
    SDL_Init(SDL_INIT_EVERYTHING);

    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetSwapInterval(1);

    window_ = SDL_CreateWindow(
      title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width_, height_,
      SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);//|SDL_WINDOW_FULLSCREEN);

    context_ = SDL_GL_CreateContext(window_);
  }
  virtual ~Window()
  {
    SDL_GL_DeleteContext(context_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
  }

  void SetRelativeMouseMode(bool mode)
  {
    SDL_SetRelativeMouseMode(static_cast<SDL_bool>(mode));
  }

  const uint8_t* GetKeyboardState(int numKeys)
  {
    return SDL_GetKeyboardState(&numKeys);
  }

  void SwapBuffers()
  {
    SDL_GL_SwapWindow(window_);
  }

  int PollEvent(SDL_Event& e)
  {
    return SDL_PollEvent(&e);
  }

  int GetWidth()
  {
    return width_;
  }

  int GetHeight()
  {
    return height_;
  }

private:

  int width_,height_;
  SDL_Window* window_;
  SDL_GLContext context_;
};

#endif
