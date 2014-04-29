#pragma once

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      namespace glew
      {
        namespace c
        {
          #include <GL/glew.h>

// Emscripten & WebGL1 force fix
#ifdef EMSCRIPTEN
  #ifdef GL_VERSION_4_4
    #undef GL_VERSION_4_4
  #endif
  #ifdef GL_VERSION_4_3
    #undef GL_VERSION_4_3
  #endif
  #ifdef GL_VERSION_4_2
    #undef GL_VERSION_4_2
  #endif
  #ifdef GL_VERSION_4_1
    #undef GL_VERSION_4_1
  #endif
  #ifdef GL_VERSION_4_0
    #undef GL_VERSION_4_0
  #endif
  #ifdef GL_VERSION_3_3
    #undef GL_VERSION_3_3
  #endif
#endif
        }
      }
    }
  }
}