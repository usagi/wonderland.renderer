#pragma once

#include "../c.hxx"
#include "../gl_type.hxx"

#include "exception.hxx"

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      namespace glew
      {
        struct program_t
        {
          
          static inline auto use_program( const gl_type::GLuint program_id = 0 )
            -> void
          { glew::c::glUseProgram( program_id ); }
          
          static inline auto current_program()
            -> gl_type::GLuint
          {
            glew::gl_type::GLint program_id = 0;
            
            glew::c::glGetIntegerv( GL_CURRENT_PROGRAM, &program_id );
            
            if ( program_id )
              return program_id;
            
            throw exception::program_is_not_set("use_program: program is not set.");
          }
        };
      }
    }
  }
}