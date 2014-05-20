#pragma once

#include <memory>

#define GLM_SWIZZLE
#include <glm/vec3.hpp>

#include "../glew.hxx"
#include "light.hxx"

#include "../glew.detail/wrapper.detail/uniform.hxx"

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      
      class point_light_t
        : public std::enable_shared_from_this< point_light_t >
        , public light_t
        , glew::uniform_t
      {
      public:
        using shared_t       = std::shared_ptr<       point_light_t >;
        using const_shared_t = std::shared_ptr< const point_light_t >;
        
      private:
        auto activate_impl( glew::gl_type::GLint program_id ) const
          -> void override
        {
          const auto id_string = std::to_string( id );
          uniform( program_id, std::string( "point_light_position"              ) + id_string , position );
          uniform( program_id, std::string( "point_light_color"                 ) + id_string , color    );
          uniform( program_id, std::string( "point_light_constant_attenuation"  ) + id_string , constant_attenuation  );
          uniform( program_id, std::string( "point_light_linear_attenuation"    ) + id_string , linear_attenuation    );
          uniform( program_id, std::string( "point_light_quadratic_attenuation" ) + id_string , quadratic_attenuation );
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