#pragma once

#include <memory>

#define GLM_SWIZZLE
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/constants.hpp>

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      
      class projection_t
        : public std::enable_shared_from_this< projection_t >
      {
      public:
        using shared_t = std::shared_ptr< projection_t >;
        
      private:
        float _fov_y;
        float _aspect_ratio;
        float _near_clip;
        float _far_clip;
        
        glm::mat4 _projection_transformation;
        
        auto _update_projection_transform()
          -> void
        { _projection_transformation = glm::perspective( _fov_y, _aspect_ratio, _near_clip, _far_clip ); }
        
      public:
        projection_t()
        { }
        
        auto projection_transformation() const
          -> glm::mat4
        {
          return _projection_transformation;
        }
        
        auto update()
          -> shared_t
        {
          _update_projection_transform();
          return shared_from_this();
        }
        
        auto fov_y( float fov_y_ )
          -> shared_t
        {
          _fov_y = fov_y_;
          return shared_from_this();
        }
        
        auto aspect_ratio( float aspect_ratio_ )
          -> shared_t
        {
          _aspect_ratio = aspect_ratio_;
          return shared_from_this();
        }
        
        auto near_clip( float near_clip_ )
          -> shared_t
        {
          _near_clip = near_clip_;
          return shared_from_this();
        }
        
        auto far_clip( float far_clip_ )
          -> shared_t
        {
          _far_clip = far_clip_;
          return shared_from_this();
        }
        
        auto fov_y() const
          -> float
        {
          return _fov_y;
        }
        
        auto aspect_ratio() const
          -> float
        {
          return _aspect_ratio;
        }
        
        auto near_clip() const
          -> float
        {
          return _near_clip;
        }
        
        auto far_clip() const
          -> float
        {
          return _far_clip;
        }
        
      };
    
    }
  }
}