#pragma once

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
          using GLbyte    = int8_t;
          using GLubyte   = uint8_t;
          using GLchar    = char;
          using GLshort   = int16_t;
          using GLushort  = uint16_t;
          using GLint     = int32_t;
          using GLuint    = uint32_t;
          //using GLfixed   = ?;
          using GLint64   = int64_t;
          using GLuint64  = uint64_t;
          using GLsizei   = uint32_t;
          using GLenum    = int32_t;
          //using GLintptr  = ?;
          using GLsizeiptr= size_t;
          using GLsync      = void*;
          using GLbitfield= uint32_t;
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