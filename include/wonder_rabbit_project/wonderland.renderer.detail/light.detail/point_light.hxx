#pragma once

#include <memory>

#define GLM_SWIZZLE
#include <glm/vec3.hpp>

#include "../glew.hxx"
#include "light.hxx"

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      
      class point_light_t
        : public light_t
      {
      protected:
        
        auto activate_impl( glew::gl_type::GLint program_id ) const
          -> void override
        {
          const auto location_of_point_light_position = glew::c::glGetUniformLocation( program_id, std::string( ( "point_light_position") + std::to_string( id ) ).data() );
          const auto location_of_point_light_color    = glew::c::glGetUniformLocation( program_id, std::string( ( "point_light_color"   ) + std::to_string( id ) ).data() );
          
          if ( location_of_point_light_position not_eq -1 )
            glew::c::glUniform3fv( location_of_point_light_position, 1, &position[0] );
          if ( location_of_point_light_color not_eq -1 )
            glew::c::glUniform3fv( location_of_point_light_color, 1, &color[0] );
        }
        
      public:
        
        glm::vec3 position;
        glm::vec3 color;
        unsigned id;
        
        point_light_t
        ( const glm::vec3& position_
        , const glm::vec3& color_ = glm::vec3( 1.0f )
        , unsigned id_ = 0
        )
        : position( position_ )
        , color   ( color_    )
        { }
        
        point_light_t
        ( glm::vec3&& position_
        , glm::vec3&& color_ = glm::vec3( 1.0f )
        , unsigned id_ = 0
        )
        : position( std::move( position_ ) )
        , color   ( std::move( color_    ) )
        { }
        
        point_light_t()
        : position( glm::vec3( 0.0f ) )
        , color   ( glm::vec3( 1.0f ) )
        , id      ( 0 )
        { }
        
        virtual ~point_light_t(){}
      };
      
    }
  }
}