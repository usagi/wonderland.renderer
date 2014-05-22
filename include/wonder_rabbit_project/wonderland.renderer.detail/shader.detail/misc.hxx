#pragma once

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      namespace shader
      {
        constexpr unsigned max_bones = 48;
        constexpr unsigned count_of_textures = 1;
        
#ifdef EMSCRIPTEN
        constexpr bool emscripten = true;
#else
        constexpr bool emscripten = false;
#endif
        
#ifdef EMSCRIPTEN
        constexpr unsigned glsl_version = 100;
#elif defined( GL_VERSION_4_4 )
        constexpr unsigned glsl_version = 440;
#elif defined( GL_VERSION_4_3 )
        constexpr unsigned glsl_version = 430;
#elif defined( GL_VERSION_4_2 )
        constexpr unsigned glsl_version = 420;
#elif defined( GL_VERSION_4_1 )
        constexpr unsigned glsl_version = 410;
#elif defined( GL_VERSION_4_0 )
        constexpr unsigned glsl_version = 400;
#elif defined( GL_VERSION_3_3 )
        constexpr unsigned glsl_version = 330;
#elif defined( GL_VERSION_3_2 )
        constexpr unsigned glsl_version = 150;
#elif defined( GL_VERSION_3_1 )
        constexpr unsigned glsl_version = 140;
#elif defined( GL_VERSION_3_0 )
        constexpr unsigned glsl_version = 130;
#elif defined( GL_VERSION_2_1 )
        constexpr unsigned glsl_version = 120;
#elif defined( GL_VERSION_2_0 )
        constexpr unsigned glsl_version = 110;
#else
        constexpr unsigned glsl_version = 100;
#endif
      }
    }
  }
}