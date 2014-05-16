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
        inline auto location( glew::gl_type::GLint program_id, const std::string& key ) const
          -> glew::gl_type::GLuint
        { return glew::c::glGetUniformLocation( program_id, ( key + std::to_string( id ) ).data() ); }
        
        inline auto uniform( glew::gl_type::GLuint location, const glm::vec3& value ) const
          -> void
        {
          if ( location not_eq -1 )
            glew::c::glUniform3fv( location, 1, &value[0] );
        }
        
        inline auto uniform( glew::gl_type::GLuint location, const float value ) const
          -> void
        {
          if ( location not_eq -1 )
            glew::c::glUniform1f( location, value );
        }
        
        auto activate_impl( glew::gl_type::GLint program_id ) const
          -> void override
        {
          const auto location_of_point_light_position = location( program_id, "point_light_position");
          const auto location_of_point_light_color    = location( program_id, "point_light_color"   );
          const auto location_of_point_light_constant_attenuation  = location( program_id, "point_light_constant_attenuation" );
          const auto location_of_point_light_linear_attenuation    = location( program_id, "point_light_linear_attenuation" );
          const auto location_of_point_light_quadratic_attenuation = location( program_id, "point_light_quadratic_attenuation" );
          
          uniform( location_of_point_light_position, position );
          uniform( location_of_point_light_color, color );
          uniform( location_of_point_light_constant_attenuation, constant_attenuation );
          uniform( location_of_point_light_linear_attenuation, linear_attenuation );
          uniform( location_of_point_light_quadratic_attenuation, quadratic_attenuation );
        }
        
      public:
        
        glm::vec3 position;
        glm::vec3 color;
        float constant_attenuation;
        float linear_attenuation;
        float quadratic_attenuation;
        unsigned id;
        
        point_light_t
        ( const glm::vec3& position_
        , const glm::vec3& color_ = glm::vec3( 1.0f )
        , const float constant_attenuation_  = 1.0f
        , const float linear_attenuation_    = 0.0f
        , const float quadratic_attenuation_ = 0.0f
        , unsigned id_ = 0
        )
        : position( position_ )
        , color   ( color_    )
        , constant_attenuation ( constant_attenuation_  )
        , linear_attenuation   ( linear_attenuation_    )
        , quadratic_attenuation( quadratic_attenuation_ )
        , id( id_ )
        { }
        
        point_light_t
        ( glm::vec3&& position_
        , glm::vec3&& color_ = glm::vec3( 1.0f )
        , const float constant_attenuation_  = 1.0f
        , const float linear_attenuation_    = 0.0f
        , const float quadratic_attenuation_ = 0.0f
        , unsigned id_ = 0
        )
        : position( std::move( position_ ) )
        , color   ( std::move( color_    ) )
        , constant_attenuation ( constant_attenuation_  )
        , linear_attenuation   ( linear_attenuation_    )
        , quadratic_attenuation( quadratic_attenuation_ )
        , id( id_ )
        { }
        
        point_light_t()
        : position( glm::vec3( 0.0f ) )
        , color   ( glm::vec3( 1.0f ) )
        , constant_attenuation ( 1.0f )
        , linear_attenuation   ( 0.0f )
        , quadratic_attenuation( 0.0f )
        , id      ( 0 )
        { }
        
        virtual ~point_light_t(){}
      };
      
    }
  }
}