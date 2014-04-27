#pragma once

#include <cstdint>
#include <cstddef>

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      namespace glew
      {
        namespace gl_type
        {
          using GLboolean = bool;
          using GLbyte    = std::int8_t;
          using GLubyte   = std::uint8_t;
          using GLchar    = char;
          using GLshort   = std::int16_t;
          using GLushort  = std::uint16_t;
          using GLint     = std::int32_t;
          using GLuint    = std::uint32_t;
          //using GLfixed   = ?;
          using GLint64   = std::int64_t;
          using GLuint64  = std::uint64_t;
          using GLsizei   = std::uint32_t;
          using GLenum    = std::int32_t;
          //using GLintptr  = ?;
          using GLsizeiptr= std::size_t;
          using GLsync      = void*;
          using GLbitfield= std::uint32_t;
          //using GLhalf    = ?;
          using GLfloat   = float;
          using GLclampf  = float;
          using GLdouble  = double;
          using GLclampd  = double;
        }
        
        #undef GLboolean
        #undef GLbyte
        #undef GLubyte
        #undef GLchar
        #undef GLshort
        #undef GLushort
        #undef GLint
        #undef GLuint
        #undef GLint64
        #undef GLuint64
        #undef GLsizei
        #undef GLenum
        #undef GLsizeiptr
        #undef GLsync
        #undef GLbitfield
        #undef GLfloat
        #undef GLclampf
        #undef GLdouble
        #undef GLclampd
        
      }
    }
  }
}