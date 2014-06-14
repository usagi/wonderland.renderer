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
        constexpr unsigned gl_version    = 200;
        constexpr unsigned glsl_version  = 100;
#elif defined( GL_VERSION_4_4 )
        constexpr unsigned gl_version    = 440;
        constexpr unsigned glsl_version  = 440;
#elif defined( GL_VERSION_4_3 )
        constexpr unsigned gl_version   = 430;
        constexpr unsigned glsl_version = 430;
#elif defined( GL_VERSION_4_2 )
        constexpr unsigned gl_version   = 420;
        constexpr unsigned glsl_version = 420;
#elif defined( GL_VERSION_4_1 )
        constexpr unsigned gl_version   = 410;
        constexpr unsigned glsl_version = 410;
#elif defined( GL_VERSION_4_0 )
        constexpr unsigned gl_version   = 400;
        constexpr unsigned glsl_version = 400;
#elif defined( GL_VERSION_3_3 )
        constexpr unsigned gl_version   = 330;
        constexpr unsigned glsl_version = 330;
#elif defined( GL_VERSION_3_2 )
        constexpr unsigned gl_version   = 320;
        constexpr unsigned glsl_version = 150;
#elif defined( GL_VERSION_3_1 )
        constexpr unsigned gl_version   = 310;
        constexpr unsigned glsl_version = 140;
#elif defined( GL_VERSION_3_0 )
        constexpr unsigned gl_version   = 300;
        constexpr unsigned glsl_version = 130;
#elif defined( GL_VERSION_2_1 )
        constexpr unsigned gl_version   = 210;
        constexpr unsigned glsl_version = 120;
#elif defined( GL_VERSION_2_0 )
        constexpr unsigned gl_version   = 200;
        constexpr unsigned glsl_version = 110;
#else
        constexpr unsigned gl_version   = 110;
        constexpr unsigned glsl_version = 100;
#endif
        
        constexpr bool gl_version_ge( unsigned version ) { return gl_version >= version; }
        constexpr bool gl_version_le( unsigned version ) { return gl_version <= version; }
        constexpr bool gl_version_gt( unsigned version ) { return gl_version >  version; }
        constexpr bool gl_version_lt( unsigned version ) { return gl_version <  version; }
        
        constexpr bool glsl_version_ge( unsigned version ) { return glsl_version >= version; }
        constexpr bool glsl_version_le( unsigned version ) { return glsl_version <= version; }
        constexpr bool glsl_version_gt( unsigned version ) { return glsl_version >  version; }
        constexpr bool glsl_version_lt( unsigned version ) { return glsl_version <  version; }
        
        namespace pseudo_pre_processor
        {
          
          namespace vertex_shader
          {
            constexpr auto IN  = ( glsl_version >= 130 ) ? u8"in"  : u8"attribute";
            constexpr auto OUT = ( glsl_version >= 130 ) ? u8"out" : u8"varying"  ;
            constexpr auto OUT_POSITION_DECLARE = ( glsl_version >= 130 ) ? u8"out vec4 var_position;" : u8"";
            constexpr auto OUT_POSITION         = ( glsl_version >= 130 ) ? u8"var_position" : u8"gl_Position";
          }
          
          namespace fragment_shader
          {
            constexpr auto IN  = ( glsl_version >= 130 ) ? u8"in" : u8"varying"  ;
            constexpr auto OUT_COLOR_DECLARE = ( glsl_version >= 130 ) ? u8"out vec4 out_color;" : u8"";
            constexpr auto OUT_COLOR         = ( glsl_version >= 130 ) ? u8"out_color" : u8"gl_FragColor";
          }
          
        }
      }
    }
  }
}