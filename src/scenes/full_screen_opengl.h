#pragma once

#include "../common.h"
// #include "../cuda_memory.hpp"
#include "pt.h"
#include <GL/glew.h>

#include <SFML/Graphics/RenderWindow.hpp>

class FullScreenOpenGLScene {
public:
  FullScreenOpenGLScene(sf::RenderWindow const &window);
  ~FullScreenOpenGLScene();

  void update(AppContext &ctx);
  void render(sf::RenderWindow &window);
  void resetBuffer(AppContext &ctx);

private:
  void initScene();

  unsigned int width, height;

  std::vector<Pixel> screenBuffer_;
  GLuint glVBO_;
  // cudaGraphicsResource_t cudaVBO_;
  // cuda::raw_ptr<Pixel> vboPtr_;
  PathTracer pt_;
  Scene scene_;
  Camera cam_;
};