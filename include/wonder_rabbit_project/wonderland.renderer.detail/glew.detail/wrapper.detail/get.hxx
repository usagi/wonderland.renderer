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
        struct get_t
        {
          
          template < typename gl_type::GLenum T_pname >
          static inline auto gl_get()
            -> gl_type::GLint
          {
            gl_type::GLint result;
            c::glGetIntegerv( T_pname, &result );
            return result;
          }
          
          static inline auto gl_get( typename gl_type::GLenum pname )
            -> gl_type::GLint
          {
            gl_type::GLint result;
            c::glGetIntegerv( pname, &result );
            return result;
          }
          
        };
      }
    }
  }
}