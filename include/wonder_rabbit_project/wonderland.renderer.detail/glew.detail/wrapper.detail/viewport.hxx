#pragma once

#define GLM_SWIZZLE
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/vec1.hpp>

#include "../c.hxx"
#include "../gl_type.hxx"

#include "../../destruct_invoker.hxx"

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      namespace glew
      {
        struct viewport_t
        {
          
          template < class T = void >
          static inline auto viewport()
            -> glm::i32vec4
          {
            glm::i32vec4 rect;
            glew::c::glGetIntegerv(GL_VIEWPORT, &rect[0]);
            return rect;
          }
          
          template < class T = void >
          static inline auto viewport( glm::i32vec4 rect )
            -> void
          {
            glew::c::glViewport( rect[0], rect[1], rect[2], rect[3] );
          }
          
          template < class T = void >
          static inline auto scoped_viewport( glm::i32vec4 rect )
            -> destruct_invoker_t
          {
            const auto backup = viewport();
            glew::c::glViewport( rect[0], rect[1], rect[2], rect[3] );
            return destruct_invoker_t( [ backup ]{ viewport( backup ); } );
          }
          
          
        };
        
      }
    }
  }
}