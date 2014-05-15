#pragma once

#include <memory>

#define GLM_SWIZZLE
#include <glm/vec3.hpp>

#include "../glew.hxx"

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      
      class light_t
      {
      protected:
        
        virtual auto check_program() const
          -> glew::gl_type::GLint
        {
          glew::gl_type::GLint program_id;
          glew::c::glGetIntegerv( GL_CURRENT_PROGRAM, &program_id );
          return program_id;
        }
        
        virtual auto activate_impl( glew::gl_type::GLint program_id ) const
          -> void
        = 0;
        
      public:
        
        auto activate() const
          -> void
        {
          if ( const auto p = check_program() )
            activate_impl( p );
        }
        
        virtual ~light_t(){}
        
      };
      
    }
  }
}