#pragma once

#include <boost/variant.hpp>

#include "glew.detail/c.hxx"
#include "glew.detail/gl_type.hxx"
#include "glew.detail/error.hxx"
#include "glew.detail/wrapper.detail/sampler.hxx"

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      
      struct sampler_t
        : protected glew::sampler_t
      {
      protected:
#if defined( GL_VERSION_3_3 )
        const glew::gl_type::GLuint _sampler_id = 0;
#else
        using param_t = boost::variant
        < glew::gl_type::GLfloat
        , glew::gl_type::GLint
        , glm::vec4
        >
        ;
        std::unordered_map< glew::gl_type::GLenum, param_t > _param_pairs;
#endif
        
      public:
        
        sampler_t()
#if defined( GL_VERSION_3_3 )
          : _sampler_id( glew::sampler_t::generate_sampler() )
#endif
        {
#if defined( GL_VERSION_3_3 )
          glew::test_error( __FILE__, __LINE__ );
#endif
        }
        
        ~sampler_t()
        {
#if defined( GL_VERSION_3_3 )
          delete_sampler( _sampler_id );
#endif
        }
        
        template < typename glew::gl_type::GLenum T_target = GL_TEXTURE_2D >
        auto bind( glew::gl_type::GLuint unit = 0 )
          -> void
        {
#if defined( GL_VERSION_3_3 )
          bind_sampler( unit, _sampler_id );
#else
          for ( const auto& param_pair : _param_pairs )
          {
            texture_parameter
            ( T_target
            , param_pair.first
            , param_pair.second.which() ? boost::get< glew::gl_type::GLint   >( param_pair.second )
                                        : boost::get< glew::gl_type::GLfloat >( param_pair.second )
            );
            glew::test_error( __FILE__, __LINE__ );
          }
#endif
        }
        
        template < class T >
        auto parameter( glew::gl_type::GLenum pname, T param )
          -> decltype( this )
        {
#if defined( GL_VERSION_3_3 )
          sampler_parameter( _sampler_id, pname, param );
          glew::test_error( __FILE__, __LINE__ );
#else
          _param_pairs[ pname ] = param;
#endif
          return this;
        }
        
        template < class T >
        auto parameter_wrap_s( T param )
          -> decltype( this )
        {
          parameter( GL_TEXTURE_WRAP_S, param );
          return this;
        }
        
        template < class T >
        auto parameter_wrap_t( T param )
          -> decltype( this )
        {
          parameter( GL_TEXTURE_WRAP_T, param );
          return this;
        }
        
        template < class T >
        auto parameter_wrap_r( T param )
          -> decltype( this )
        {
          parameter( GL_TEXTURE_WRAP_R, param );
          return this;
        }
        
        template < class T >
        auto parameter_wrap_st( T param )
          -> decltype( this )
        {
          parameter( GL_TEXTURE_WRAP_S, param );
          parameter( GL_TEXTURE_WRAP_T, param );
          return this;
        }
        
        template < class T >
        auto parameter_wrap_sr( T param )
          -> decltype( this )
        {
          parameter( GL_TEXTURE_WRAP_S, param );
          parameter( GL_TEXTURE_WRAP_R, param );
          return this;
        }
        
        template < class T >
        auto parameter_wrap_tr( T param )
          -> decltype( this )
        {
          parameter( GL_TEXTURE_WRAP_T, param );
          parameter( GL_TEXTURE_WRAP_R, param );
          return this;
        }
        
        template < class T >
        auto parameter_wrap_str( T param )
          -> decltype( this )
        {
          parameter_wrap_st( param );
          parameter( GL_TEXTURE_WRAP_R, param );
          return this;
        }
        
        template < class T >
        auto parameter_min_filter( T param )
          -> decltype( this )
        {
          parameter( GL_TEXTURE_MIN_FILTER, param );
          return this;
        }
        
        template < class T >
        auto parameter_mag_filter( T param )
          -> decltype( this )
        {
          parameter( GL_TEXTURE_MAG_FILTER, param );
          return this;
        }
        
        template < class T >
        auto parameter_min_mag_filter( T param )
          -> decltype( this )
        {
          parameter( GL_TEXTURE_MIN_FILTER, param );
          parameter( GL_TEXTURE_MAG_FILTER, param );
          return this;
        }
        
        template < class T >
        auto parameter_lod_bias( T param )
          -> decltype( this )
        {
          parameter( GL_TEXTURE_LOD_BIAS, param );
          return this;
        }
        
        template < class T >
        auto parameter_min_max_lod( T param )
          -> decltype( this )
        {
          parameter( GL_TEXTURE_MIN_LOD, param );
          parameter( GL_TEXTURE_MAX_LOD, param );
          return this;
        }
        
        template < class T >
        auto parameter_min_lod( T param )
          -> decltype( this )
        {
          parameter( GL_TEXTURE_MIN_LOD, param );
          return this;
        }
        
        template < class T >
        auto parameter_max_lod( T param )
          -> decltype( this )
        {
          parameter( GL_TEXTURE_MAX_LOD, param );
          return this;
        }
        
        template < class T >
        auto parameter_border_color( T param )
          -> decltype( this )
        {
          parameter( GL_TEXTURE_BORDER_COLOR, param );
          return this;
        }
        
        template < class T >
        auto parameter_compare_mode( T param )
          -> decltype( this )
        {
          parameter( GL_TEXTURE_COMPARE_MODE, param );
          return this;
        }
        
        template < class T >
        auto parameter_compare_func( T param )
          -> decltype( this )
        {
          parameter( GL_TEXTURE_COMPARE_FUNC, param );
          return this;
        }
        
      };
      
    }
  }
}