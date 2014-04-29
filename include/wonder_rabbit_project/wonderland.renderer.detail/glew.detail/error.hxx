#pragma once

#include <stdexcept>
#include <sstream>

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
        inline auto test_error( const std::string& file = "?", const unsigned line = 0 ) -> void
        {
          auto e = get_error();
          
          if ( is_error( e ) )
          {
            std::stringstream message;
            message << "glGetError ( from " << file << " +" << line << " ) : " << e << " ";
            switch ( e )
            {
              case GL_INVALID_ENUM     : message << "GL_INVALID_ENUM";      break;
              case GL_INVALID_VALUE    : message << "GL_INVALID_VALUE";     break;
              case GL_INVALID_OPERATION: message << "GL_INVALID_OPERATION"; break;
              case GL_STACK_OVERFLOW   : message << "GL_STACK_OVERFLOW";    break;
              case GL_STACK_UNDERFLOW  : message << "GL_STACK_UNDERFLOW";   break;
              case GL_OUT_OF_MEMORY    : message << "GL_OUT_OF_MEMORY";     break;
            }
            throw std::runtime_error( message.str() );
          }
        }
        
      }
    }
  }
}