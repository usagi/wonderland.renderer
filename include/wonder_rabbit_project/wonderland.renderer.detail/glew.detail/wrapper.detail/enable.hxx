#pragma once

#include "../c.hxx"
#include "../gl_type.hxx"

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      namespace glew
      {
        struct enable_t
        {
          
          static inline auto blend ( bool enable = true )
            -> void
          {
            if( enable )
              c::glEnable( GL_BLEND );
            else
              c::glDisable( GL_BLEND );
          }
          
          static inline auto cull_face ( bool enable = true )
            -> void
          {
            if( enable )
              c::glEnable( GL_CULL_FACE );
            else
              c::glDisable( GL_CULL_FACE );
          }
          
          static inline auto debug_output ( bool enable = true )
            -> void
          {
            if( enable )
              c::glEnable( GL_DEBUG_OUTPUT );
            else
              c::glDisable( GL_DEBUG_OUTPUT );
          }
          
          static inline auto depth_test ( bool enable = true )
            -> void
          {
            if( enable )
              c::glEnable( GL_DEPTH_TEST );
            else
              c::glDisable( GL_DEPTH_TEST );
          }
          
          static inline auto multisample_capability()
            -> bool
          {
            gl_type::GLint buffers, samples;
            
            c::glGetIntegerv(GL_SAMPLE_BUFFERS, &buffers);
            c::glGetIntegerv(GL_SAMPLES, &samples);
            
            return buffers >= 1 and samples >= 2;
          }
          
          inline auto multisample( bool enable = true )
            -> void
          {
            if ( not enable )
            {
#ifndef EMSCRIPTEN
              c::glDisable( GL_MULTISAMPLE );
#endif
              c::glDisable( GL_SAMPLE_ALPHA_TO_COVERAGE );
              return;
            }
            
            if ( not multisample_capability() )
              throw std::runtime_error( "cannot use multi sample." );
#ifndef EMSCRIPTEN
            // need OpenGL API
            //  but on GLES2 MULTISAMPLE is true default and not support glEnable/glDisable it.
            c::glEnable( GL_MULTISAMPLE );
#endif
            c::glEnable( GL_SAMPLE_ALPHA_TO_COVERAGE );
            
          }
          
        };
      }
    }
  }
}