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
        
        enum class VERTEX_ATTRIBUTE : gl_type::GLenum
        { BYTE           = GL_BYTE
        , UNSIGNED_BYTE  = GL_UNSIGNED_BYTE
        , SHORT          = GL_SHORT
        , UNSIGNED_SHORT = GL_UNSIGNED_SHORT
        , INT            = GL_INT
        , UNSIGNED_INT   = GL_UNSIGNED_INT

        , HALF_FLOAT     = GL_HALF_FLOAT
        , FLOAT          = GL_FLOAT
        , DOUBLE         = GL_DOUBLE
        , FIXED          = GL_FIXED

        , INT_2_10_10_10_REV          = GL_INT_2_10_10_10_REV
        , UNSIGNED_INT_2_10_10_10_REV = GL_UNSIGNED_INT_2_10_10_10_REV

        , INT8   = BYTE
        , UINT8  = UNSIGNED_BYTE
        , INT16  = SHORT
        , UINT16 = UNSIGNED_SHORT
        , INT32  = INT
        , UINT32 = UNSIGNED_INT

        , BINARY16 = HALF_FLOAT
        , BINARY32 = FLOAT
        , BINARY64 = DOUBLE
        };

#undef GL_BYTE
#undef GL_UNSIGNED_BYTE
#undef GL_SHORT
#undef GL_UNSIGNED_SHORT
#undef GL_INT
#undef GL_UNSIGNED_INT
#undef GL_HALF_FLOAT
#undef GL_FLOAT
#undef GL_DOUBLE
#undef GL_FIXED
#undef GL_INT_2_10_10_10_REV
#undef GL_UNSIGNED_2_10_10_10_REV

        std::string to_string( VERTEX_ATTRIBUTE v )
        {
          switch ( v )
          {
            case VERTEX_ATTRIBUTE::INT8    :
              return "INT8";

            case VERTEX_ATTRIBUTE::UINT8   :
              return "UINT8";

            case VERTEX_ATTRIBUTE::INT16   :
              return "INT16";

            case VERTEX_ATTRIBUTE::UINT16  :
              return "UINT16";

            case VERTEX_ATTRIBUTE::INT32   :
              return "INT32";

            case VERTEX_ATTRIBUTE::UINT32  :
              return "UINT32";

            case VERTEX_ATTRIBUTE::BINARY16 :
              return "BINARY16";

            case VERTEX_ATTRIBUTE::BINARY32 :
              return "BINARY32";

            case VERTEX_ATTRIBUTE::BINARY64 :
              return "BINARY64";

            case VERTEX_ATTRIBUTE::FIXED    :
              return "FIXED";

            case VERTEX_ATTRIBUTE::INT_2_10_10_10_REV          :
              return "GL_INT_2_10_10_10_REV";

            case VERTEX_ATTRIBUTE::UNSIGNED_INT_2_10_10_10_REV :
              return "GL_UNSIGNED_INT_2_10_10_10_REV";

            default:
              throw std::logic_error("to_string unkown VERTEX_ATTRIBUTE type");
          }
        }

        template<class T> struct vertex_attribute_from_type final { };

#define WRP_TMP(a,b) \
  template<> struct vertex_attribute_from_type <a> final {\
    static constexpr VERTEX_ATTRIBUTE vertex_attribute = VERTEX_ATTRIBUTE::b;\
  };
        WRP_TMP( gl_type::GLbyte, INT8 );
        WRP_TMP( gl_type::GLubyte, UINT8 );
        WRP_TMP( gl_type::GLshort, INT16 );
        WRP_TMP( gl_type::GLushort, UINT16 );
        WRP_TMP( gl_type::GLint, INT32 );
        WRP_TMP( gl_type::GLuint, UINT32 );
        //WRP_TMP(gl_type::GLhalf, BINARY16);
        WRP_TMP( gl_type::GLfloat, BINARY32 );
        WRP_TMP( gl_type::GLdouble, BINARY64 );
        //WRP_TMP(gl_type::GLfixed, FIXED);
#undef WRP_TMP
        
      }
    }
  }
}