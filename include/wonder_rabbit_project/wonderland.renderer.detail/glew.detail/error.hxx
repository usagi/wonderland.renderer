#pragma once

#include <stdexcept>
#include <string>

#include "c.hxx"
#include "gl_type.hxx"

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      namespace glew
      {

        template < class T = void >
        auto get_error() -> gl_type::GLint
        { return c::glGetError(); }

        template < class T = void >
        inline auto is_no_error( const gl_type::GLint& v ) -> bool
        { return v == GL_NO_ERROR; }
        
        template < class T = void >
        inline auto is_error( const gl_type::GLint& v ) -> bool
        { return not is_no_error( v ); }
        
        template < class = void >
        inline auto test_error() -> void
        {
          auto e = get_error();
          
          if ( is_error( e ) )
          {
            auto message = std::string( "glGetError: " + std::to_string( e ) );
            throw std::runtime_error( message );
          }
        }
        
      }
    }
  }
}