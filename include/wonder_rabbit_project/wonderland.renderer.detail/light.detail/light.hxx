#pragma once

#include <memory>

#define GLM_SWIZZLE
#include <glm/vec3.hpp>

#include "../glew.hxx"

#include "../glew.detail/wrapper.detail/program.hxx"

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      
      class light_t
        : glew::program_t
      {
      protected:
        
        virtual
        auto activate_impl( glew::gl_type::GLint program_id ) const
          -> void
        = 0;
        
      public:
        
        auto activate() const
          -> void
        {
          if ( const auto p = current_program() )
            activate_impl( p );
        }
        
        virtual ~light_t(){}
        
      };
      
    }
  }
}