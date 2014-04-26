#pragma once

#include <stdexcept>

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
        
        enum class SHADER : gl_type::GLuint
        { VERTEX          = GL_VERTEX_SHADER
        , TESS_CONTROL    = GL_TESS_CONTROL_SHADER
        , TESS_EVALUATION = GL_TESS_EVALUATION_SHADER
        , GEOMETRY        = GL_GEOMETRY_SHADER
        , FRAGMENT        = GL_FRAGMENT_SHADER
        };
        
        #undef GL_VERTEX_SHADER
        #undef GL_TESS_CONTROL_SHADER
        #undef GL_TESS_EVALUATION_SHADER
        #undef GL_GEOMETRY_SHADER
        #undef GL_FRAGMENT_SHADER
        
        std::string to_string(SHADER v){
          switch(v){
            case SHADER::VERTEX          : return "VERTEX";
            case SHADER::FRAGMENT        : return "FRAGMENT";
            case SHADER::GEOMETRY        : return "GEOMETRY";
            case SHADER::TESS_CONTROL    : return "TESS_CONTROL";
            case SHADER::TESS_EVALUATION : return "TESS_EVALUATION";
            default                      : throw std::logic_error("to_string unkown SHADER type");
          }
        }
        
      }
    }
  }
}