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
        struct program_t
        {
          static inline auto use_program( const gl_type::GLuint program_id = 0 )
            -> void
          { glew::c::glUseProgram( program_id ); }
        };
      }
    }
  }
}