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

        enum class USAGE : gl_type::GLenum
        { STREAM_DRAW  = GL_STREAM_DRAW
        , STREAM_READ  = GL_STREAM_READ
        , STREAM_COPY  = GL_STREAM_COPY
        , STATIC_DRAW  = GL_STATIC_DRAW
        , STATIC_READ  = GL_STATIC_READ
        , STATIC_COPY  = GL_STATIC_COPY
        , DYNAMIC_DRAW = GL_DYNAMIC_DRAW
        , DYNAMIC_READ = GL_DYNAMIC_READ
        , DYNAMIC_COPY = GL_DYNAMIC_COPY
        };

#undef GL_STREAM_DRAW
#undef GL_STREAM_READ
#undef GL_STREAM_COPY
#undef GL_STATIC_DRAW
#undef GL_STATIC_READ
#undef GL_STATIC_COPY
#undef GL_DYNAMIC_DRAW
#undef GL_DYNAMIC_RAED
#undef GL_DYNAMIC_COPY

        std::string to_string( USAGE v )
        {
          switch ( v )
          {
            case USAGE::STREAM_DRAW:
              return "STREAM_DRAW";

            case USAGE::STREAM_READ:
              return "STREAM_READ";

            case USAGE::STREAM_COPY:
              return "STREAM_COPY";

            case USAGE::STATIC_DRAW:
              return "STATIC_DRAW";

            case USAGE::STATIC_READ:
              return "STATIC_READ";

            case USAGE::STATIC_COPY:
              return "STATIC_COPY";

            case USAGE::DYNAMIC_DRAW:
              return "DYNAMIC_DRAW";

            case USAGE::DYNAMIC_READ:
              return "DYNAMIC_READ";

            case USAGE::DYNAMIC_COPY:
              return "DYNAMIC_COPY";

            default:
              throw std::logic_error("to_string unkown SHADER type");
          }
        }

        template<gl_type::GLenum TVALUE>
        struct usage
        {
          static constexpr USAGE value = USAGE( TVALUE );
        };

#define WRP_TMP(a, b) using usage_ ## a = usage<gl_type::GLenum(USAGE:: b )>;
        WRP_TMP( stream_draw, STREAM_DRAW );
        WRP_TMP( stream_read, STREAM_READ );
        WRP_TMP( stream_copy, STREAM_COPY );
        WRP_TMP( static_draw, STATIC_DRAW );
        WRP_TMP( static_read, STATIC_READ );
        WRP_TMP( static_copy, STATIC_COPY );
        WRP_TMP( dynamic_draw, DYNAMIC_DRAW );
        WRP_TMP( dynamic_read, DYNAMIC_READ );
        WRP_TMP( dynamic_copy, DYNAMIC_COPY );
#undef WRP_TMP
      }
    }
  }
}