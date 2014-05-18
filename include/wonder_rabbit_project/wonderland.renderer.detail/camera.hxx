
#include <memory>

#define GLM_SWIZZLE
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      
      class camera_t
      {
        glm::vec3 _eye;
        glm::vec3 _target;
        glm::vec3 _up;
        glm::mat4 _view_transformation_cache;
        bool _changed;
      
      public:
        
        camera_t
        ( const glm::vec3& eye_
        , const glm::vec3& target_
        , const glm::vec3& up_
        )
          : _eye   ( eye_    )
          , _target( target_ )
          , _up    ( up_     )
          , _changed( true )
        { }
        
        camera_t
        ( glm::vec3&& eye_
        , glm::vec3&& target_
        , glm::vec3&& up_
        )
          : _eye   ( std::move( eye_    ) )
          , _target( std::move( target_ ) )
          , _up    ( std::move( up_     ) )
          , _changed( true )
        { }
        
        camera_t()
          : _eye   ( glm::vec3( 0.0f, 0.0f, 100.0f ) )
          , _target( glm::vec3( 0.0f, 0.0f,   0.0f ) )
          , _up    ( glm::vec3( 0.0f, 1.0f,   0.0f ) )
          , _changed( true )
        { }
        
        auto eye   (const glm::vec3& v) -> void { _eye    = v; _changed = true; }
        auto target(const glm::vec3& v) -> void { _target = v; _changed = true; }
        auto up    (const glm::vec3& v) -> void { _up     = v; _changed = true; }
        
        auto eye   () const -> const glm::vec3& { return _eye; }
        auto target() const -> const glm::vec3& { return _target; }
        auto up    () const -> const glm::vec3& { return _up; }
        
        auto eye_from_orbit_of_target( float theta, float phi, float distance )
          -> void
        {
          eye
          ( ( glm::mat4_cast( glm::quat( glm::vec3( phi, theta, 0.0f ) ) )
            * glm::scale( glm::mat4(), glm::vec3( 0.0f, 0.0f, distance ) )
            * glm::vec4( 0.0f, 0.0f, 1.0f, 1.0f )
            ).xyz()
          );
        }
        
        auto view_transformation() -> glm::mat4
        {
          if ( _changed )
          {
            _view_transformation_cache = glm::lookAt( _eye, _target, _up );
            _changed = false;
          }
          return _view_transformation_cache;
        }
        
        auto view_direction()
          -> glm::vec3 
        { return glm::normalize( _eye - _target ); }
        
      };
      
    }
  }
}