#pragma once

#define GLM_SWIZZLE
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/vec1.hpp>

#include "../c.hxx"
#include "../gl_type.hxx"

#include "get.hxx"
#include "enable.hxx"

#include "../../destruct_invoker.hxx"

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      namespace glew
      {
        struct misc_t
        {
          template < class T = void >
          static inline auto flush()
            -> void
          { c::glFlush(); }
          
          template < class T = void >
          static inline auto clear_color()
            -> glm::vec4
          { return get_t::get< GL_COLOR_CLEAR_VALUE, glm::vec4 >(); }
          
          template < class T = void >
          static inline auto clear_color( const glm::vec4& color )
            -> void
          { c::glClearColor( color.r, color.g, color.b, color.a ); }
          
          template < class T = void >
          static inline auto clear_color( const glm::vec3& color )
            -> void
          { c::glClearColor( color.r, color.g, color.b, 1.0f ); }
          
          template < class T = void >
          static inline auto clear_color( const glm::vec1& color )
            -> void
          { clear_color( color.r ); }
          
          template < class T = void >
          static inline auto clear_color( const float color )
            -> void
          { c::glClearColor( color, color, color, 1.0f ); }
          
          template < class T >
          static inline auto scoped_clear_color( const T& color )
            -> destruct_invoker_t
          {
            auto backup = clear_color();
            clear_color( color );
            return destruct_invoker_t( [ backup ]{ clear_color( backup ); } );
          }
          
          template < class T = void >
          static inline auto clear
          ( gl_type::GLbitfield flag
            = GL_COLOR_BUFFER_BIT
            | GL_DEPTH_BUFFER_BIT
            | GL_STENCIL_BUFFER_BIT
          )
            -> void
          { c::glClear( glew::gl_type::GLint( flag ) ); }
          
          template < class T = void >
          static inline auto color_mask
          ( const bool r
          , const bool g
          , const bool b
          , const bool a
          )
            -> void
          { c::glColorMask( r, g, b, a ); }
          
          template < class T = void >
          static inline auto color_mask
          ( const gl_type::GLuint buffer_index
          , const bool r
          , const bool g
          , const bool b
          , const bool a
          )
            -> void
          { c::glColorMaski( buffer_index, r, g, b, a ); }
          
          template < class T = void >
          static inline auto color_mask( const bool all )
            -> void
          { color_mask( all, all, all, all ); }
          
          template < class T = void >
          static inline auto color_mask( const gl_type::GLuint buffer_index, const bool all )
            -> void
          { color_mask( buffer_index, all, all, all, all ); }
          
          template < class T = void >
          static inline auto color_mask( const glm::bvec4& rgba )
            -> void
          { color_mask( rgba[0], rgba[1], rgba[2], rgba[3] ); }
          
          template < class T = void >
          static inline auto color_mask( const gl_type::GLuint buffer_index, const glm::bvec4& rgba )
            -> void
          { color_mask( buffer_index, rgba[0], rgba[1], rgba[2], rgba[3] ); }
          
          template < class T = void >
          static inline auto color_mask()
            -> glm::bvec4
          { return get_t::get< GL_COLOR_WRITEMASK, glm::bvec4 >(); }
          
          template < class ... Ts >
          static inline auto scoped_color_mask
          ( const Ts& ... rgba )
            -> destruct_invoker_t
          {
            const auto backup = color_mask();
            color_mask( rgba ... );
            return destruct_invoker_t( [ backup ]{ color_mask( backup ); } );
          }
          
          template < class T = void >
          static inline auto cull_face( const gl_type::GLenum flag = GL_BACK )
            -> void
          { c::glCullFace( flag ); }
          
          template < class T = void >
          static inline auto scoped_cull_face( const gl_type::GLenum flag = GL_FRONT )
            -> destruct_invoker_t
          {
            const auto backup = get_t::get< GL_CULL_FACE_MODE >();
            cull_face( flag );
            return destruct_invoker_t( [ backup ]{ cull_face( backup ); } );
          }
          
          template < class T = void >
          static inline auto polygon_offset( gl_type::GLfloat factor, gl_type::GLfloat units )
            -> void
          { c::glPolygonOffset( factor, units ); }
          
          template < class T = void >
          static inline auto scoped_polygon_offset( gl_type::GLfloat factor, gl_type::GLfloat units )
            -> destruct_invoker_t
          {
            const auto backup_factor = polygon_offset_factor();
            const auto backup_units  = polygon_offset_units();
            polygon_offset( factor, units );
            return destruct_invoker_t( [ backup_factor, backup_units ]{ polygon_offset( backup_factor, backup_units ); } );
          }
          
          template < class T = void >
          static inline auto polygon_offset_factor()
            -> gl_type::GLfloat
          { return get_t::get< GL_POLYGON_OFFSET_FACTOR, gl_type::GLfloat >(); }
          
          template < class T = void >
          static inline auto polygon_offset_units()
            -> gl_type::GLfloat
          { return get_t::get< GL_POLYGON_OFFSET_UNITS, gl_type::GLfloat >(); }
          
          
        };
      }
    }
  }
}