#pragma once

#define GLM_SWIZZLE
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/vec1.hpp>

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
        struct misc_t
        {
          static inline auto flush()
            -> void
          { glew::c::glFlush(); }
          
          static inline auto clear_color( const glm::vec4& color )
            -> void
          { glew::c::glClearColor( color.r, color.g, color.b, color.a ); }
          
          static inline auto clear_color( const glm::vec3& color )
            -> void
          { glew::c::glClearColor( color.r, color.g, color.b, 1.0f ); }
          
          static inline auto clear_color( const glm::vec1& color )
            -> void
          { clear_color( color.r ); }
          
          static inline auto clear_color( const float color )
            -> void
          { glew::c::glClearColor( color, color, color, 1.0f ); }
          
          static inline auto clear
          ( glew::gl_type::GLbitfield flag
            = GL_COLOR_BUFFER_BIT
            | GL_DEPTH_BUFFER_BIT
            | GL_STENCIL_BUFFER_BIT
          )
            -> void
          { glew::c::glClear( glew::gl_type::GLint( flag ) ); }
        };
      }
    }
  }
}