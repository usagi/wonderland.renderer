#pragma once

#include <stdexcept>
#include <string>

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
        
        enum class CLEAR : gl_type::GLint
        { NONE    = 0
        , COLOR   = GL_COLOR_BUFFER_BIT
        , DEPTH   = GL_DEPTH_BUFFER_BIT
        //, ACCUM   = GL_ACCUM_BUFFER_BIT
        , STENCIL = GL_STENCIL_BUFFER_BIT
        , ALL     = COLOR | DEPTH /*| ACCUM*/ | STENCIL
        };
        
        CLEAR operator|(CLEAR a, CLEAR b)
        { return CLEAR( gl_type::GLint(a) | gl_type::GLint(a) ); }
        
        #undef GL_COLOR_BUFFER_BIT
        #undef GL_DEPTH_BUFFER_BIT
        //#undef GL_ACCUM_BUFFER_BIT
        #undef GL_STENCIL_BUFFER_BIT
        
        std::string to_string( CLEAR v )
        {
          switch ( v )
          {
            case CLEAR::NONE:
              return "NONE";
              
            case CLEAR::COLOR:
              return "COLOR";
              
            case CLEAR::DEPTH:
              return "DEPTH";
              
            //case CLEAR::ACCUM:
            //  return "ACCUM";
              
            case CLEAR::STENCIL:
              return "STENCIL";
              
            default:
              throw std::logic_error("to_string unkown CLEAR type");
          }
        }
        
      }
    }
  }
}