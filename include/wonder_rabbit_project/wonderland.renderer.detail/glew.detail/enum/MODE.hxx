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

        enum class MODE : gl_type::GLenum
#define WRP_TMP(a) a = GL_ ## a
        { WRP_TMP( POINTS )
        , WRP_TMP( LINE_STRIP )
        , WRP_TMP( LINE_LOOP )
        , WRP_TMP( LINES )
        , WRP_TMP( LINE_STRIP_ADJACENCY )
        , WRP_TMP( LINES_ADJACENCY )
        , WRP_TMP( TRIANGLE_STRIP )
        , WRP_TMP( TRIANGLE_FAN )
        , WRP_TMP( TRIANGLES )
        , WRP_TMP( TRIANGLE_STRIP_ADJACENCY )
        , WRP_TMP( TRIANGLES_ADJACENCY )
        , WRP_TMP( PATCHES )
#undef WRP_TMP
        };

        #undef STREAM_DRAW
        #undef STREAM_READ
        #undef STREAM_COPY
        #undef STATIC_DRAW
        #undef STATIC_READ
        #undef STATIC_COPY
        #undef DYNAMIC_DRAW
        #undef DYNAMIC_RAED
        #undef DYNAMIC_COPY

        std::string to_string( MODE v )
        {
          switch ( v )
          {
#define WRP_TMP(a) case MODE:: a : return #a ;
              WRP_TMP( POINTS )
              WRP_TMP( LINE_STRIP )
              WRP_TMP( LINE_LOOP )
              WRP_TMP( LINES )
              WRP_TMP( LINE_STRIP_ADJACENCY )
              WRP_TMP( LINES_ADJACENCY )
              WRP_TMP( TRIANGLE_STRIP )
              WRP_TMP( TRIANGLE_FAN )
              WRP_TMP( TRIANGLES )
              WRP_TMP( TRIANGLE_STRIP_ADJACENCY )
              WRP_TMP( TRIANGLES_ADJACENCY )
              WRP_TMP( PATCHES )
#undef WRP_TMP

            default
                :
              return "";
          }
        }

        template<gl_type::GLenum TVALUE>
        struct mode
        {
          static constexpr MODE value = MODE( TVALUE );
        };

#define WRP_TMP(a, b) using mode_ ## a = mode<gl_type::GLenum(MODE:: b )>;
        WRP_TMP( points, POINTS );
        WRP_TMP( line_strip, LINE_STRIP )
        WRP_TMP( line_loop, LINE_LOOP )
        WRP_TMP( lines, LINES )
        WRP_TMP( line_strip_adjacency, LINE_STRIP_ADJACENCY )
        WRP_TMP( lines_adjacency, LINES_ADJACENCY )
        WRP_TMP( triangle_strip, TRIANGLE_STRIP )
        WRP_TMP( triangle_fan, TRIANGLE_FAN )
        WRP_TMP( triangles, TRIANGLES )
        WRP_TMP( triangle_strip_adjacency, TRIANGLE_STRIP_ADJACENCY )
        WRP_TMP( triangles_adjacency, TRIANGLES_ADJACENCY )
        WRP_TMP( patches, PATCHES )
#undef WRP_TMP
      }
    }
  }
}