#pragma once

#define GLM_SWIZZLE
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/vec1.hpp>

#include "glew.detail/c.hxx"
#include "glew.detail/gl_type.hxx"

#include "glew.detail/wrapper.detail/texture.hxx"
#include <boost/iterator/iterator_concepts.hpp>

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      
      // common part
      template
      < typename glew::gl_type::GLenum T_target
      , typename glew::gl_type::GLenum T_internal_format
      >
      class texture_base_t
        : protected glew::texture_t
      {
      public:
        static constexpr glew::gl_type::GLenum target               = GL_TEXTURE_2D;
        static constexpr glew::gl_type::GLenum internal_format      = T_internal_format;
        static constexpr glew::gl_type::GLenum base_internal_format
          = (  internal_format == GL_DEPTH_COMPONENT16
            or internal_format == GL_DEPTH_COMPONENT24
            or internal_format == GL_DEPTH_COMPONENT32
            or internal_format == GL_DEPTH_COMPONENT32F
            )  ? GL_DEPTH_COMPONENT
          : (  internal_format == GL_DEPTH24_STENCIL8
            or internal_format == GL_DEPTH32F_STENCIL8
            )  ? GL_DEPTH_STENCIL
          : (  internal_format == GL_STENCIL_INDEX8
            )  ? GL_STENCIL
          : 0;
        
      protected:
        const glew::gl_type::GLuint _texture_id = 0;
        glew::gl_type::GLenum       _target     = 0;
        
        glm::i32vec4 _viewport;
        
      public:
        texture_base_t()
          : _texture_id( generate_texture() )
          , _viewport( glm::i32vec4( 0 ) )
        { }
        
        virtual ~texture_base_t()
        { safe_delete_texture( _texture_id ); }
        
        auto texture_id() -> decltype( _texture_id )
        { return _texture_id; }
        
        auto bind() -> void
        { bind_texture< T_target >( _texture_id ); }
        
        auto scoped_bind() -> decltype( scoped_bind_texture( _texture_id ) )
        { return scoped_bind_texture< T_target >( _texture_id ); }
        
        auto viewport() -> const glm::i32vec4&
        { return _viewport; }
        
        auto count_of_data_elements() -> const std::size_t
        { return _viewport[2] * _viewport[3]; }
        
      };
      
      // has class template partial specialization
      template
      < typename glew::gl_type::GLenum T_target
      , typename glew::gl_type::GLenum T_internal_format
      >
      class texture_t
        : public texture_base_t< T_target, T_internal_format >
        , public std::enable_shared_from_this< texture_t< T_target, T_internal_format > >
      { };
      
      // specialize to GL_TEXTURE_1D
      template
      < typename glew::gl_type::GLenum T_internal_format
      >
      class texture_t< GL_TEXTURE_1D, T_internal_format >
        : public texture_base_t< GL_TEXTURE_1D, T_internal_format >
      {
        using parent_t = texture_base_t< GL_TEXTURE_1D, T_internal_format >;
        
      public:
        auto image_1d( glew::gl_type::GLsizei width, const void* data = nullptr )
          -> void
        {
          parent_t::scoped_bind();
          glew::texture_t::texture_image_1d< T_internal_format >( width, data );
          parent_t::_viewport[2] = width;
          parent_t::_viewport[3] = 1;
        }
        
        auto image_1d()
          -> void
        {
          parent_t::scoped_bind();
          glew::texture_t::texture_image_1d< T_internal_format >();
          parent_t::_viewport[2] = glew::texture_t::max_texture_size();
          parent_t::_viewport[3] = 1;
        }
        
      };
      
      // specialize to GL_TEXTURE_2D
      template
      < typename glew::gl_type::GLenum T_internal_format
      >
      class texture_t< GL_TEXTURE_2D, T_internal_format >
        : public texture_base_t< GL_TEXTURE_2D, T_internal_format >
      {
        using parent_t = texture_base_t< GL_TEXTURE_2D, T_internal_format >;
        
      public:
        auto image_2d( glew::gl_type::GLsizei width, glew::gl_type::GLsizei height, const void* data = nullptr )
           -> void
       {
          parent_t::scoped_bind();
          glew::texture_t::texture_image_2d< T_internal_format >( width, height, data );
          parent_t::_viewport[2] = width;
          parent_t::_viewport[3] = height;
        }
        
        auto image_2d( glew::gl_type::GLsizei size, const void* data = nullptr )
          -> void
        { image_2d( size, size, data ); }
        
        auto image_2d()
          -> void
        {
          parent_t::scoped_bind();
          glew::texture_t::texture_image_2d< T_internal_format >();
          parent_t::_viewport[2] = parent_t::_viewport[3] = glew::texture_t::max_texture_size();
        }
        
        auto image_2d_multisample( glew::gl_type::GLsizei width, glew::gl_type::GLsizei height, const void* data, glew::gl_type::GLsizei samples )
          -> void
        {
          parent_t::scoped_bind();
          glew::texture_t::texture_image_2d_multisample< T_internal_format >( width, height, data, samples );
          parent_t::_viewport[2] = width;
          parent_t::_viewport[3] = height;
        }
        
        auto image_2d_multisample( glew::gl_type::GLsizei size, const void* data, glew::gl_type::GLsizei samples )
          -> void
        { image_2d_multisample( size, size, data, samples ); }
        
        auto image_2d_multisample( glew::gl_type::GLsizei width, glew::gl_type::GLsizei height, const void* data )
          -> void
        {
          parent_t::scoped_bind();
          glew::texture_t::texture_image_2d_multisample< T_internal_format >( width, height, data );
          parent_t::_viewport[2] = width;
          parent_t::_viewport[3] = height;
        }
        
        auto image_2d_multisample( glew::gl_type::GLsizei size, const void* data )
          -> void
        { image_2d_multisample( size, size, data ); }
        
      };
      
      // specialize to GL_TEXTURE_3D
      template
      < typename glew::gl_type::GLenum T_internal_format
      >
      class texture_t< GL_TEXTURE_3D, T_internal_format >
        : public texture_base_t< GL_TEXTURE_3D, T_internal_format >
      {
        using parent_t = texture_base_t< GL_TEXTURE_3D, T_internal_format >;
        
      public:
        auto image_3d( glew::gl_type::GLsizei width, glew::gl_type::GLsizei height, glew::gl_type::GLsizei depth, const void* data = nullptr )
          -> void
        {
          parent_t::scoped_bind();
          glew::texture_t::texture_image_3d< T_internal_format >( width, height, depth, data );
          parent_t::_viewport[2] = width;
          parent_t::_viewport[3] = height;
        }
        
        auto image_3d( glew::gl_type::GLsizei size, glew::gl_type::GLsizei depth, const void* data = nullptr )
          -> void
        { image_3d( size, size, depth, data ); }
        
        auto image_3d( glew::gl_type::GLsizei size, const void* data = nullptr )
          -> void
        { image_3d( size, size, size, data ); }
        
        auto image_3d()
          -> void
        {
          parent_t::scoped_bind();
          glew::texture_t::texture_image_3d< T_internal_format >();
          parent_t::_viewport[2] = parent_t::_viewport[3] = glew::texture_t::max_texture_size();
        }
        
        auto image_3d_multisample( glew::gl_type::GLsizei width, glew::gl_type::GLsizei height, glew::gl_type::GLsizei depth, const void* data, glew::gl_type::GLsizei samples )
          -> void
        {
          parent_t::scoped_bind();
          glew::texture_t::texture_image_3d_multisample< T_internal_format >( width, height, depth, data, samples );
          parent_t::_viewport[2] = width;
          parent_t::_viewport[3] = height;
        }
        
        auto image_3d_multisample( glew::gl_type::GLsizei size, glew::gl_type::GLsizei depth, const void* data, glew::gl_type::GLsizei samples )
          -> void
        { image_3d_multisample( size, size, depth, data, samples ); }
        
        auto image_3d_multisample( glew::gl_type::GLsizei size, const void* data, glew::gl_type::GLsizei samples  )
          -> void
        { image_3d_multisample( size, size, size, data, samples ); }
        
        auto image_3d_multisample( glew::gl_type::GLsizei width, glew::gl_type::GLsizei height, glew::gl_type::GLsizei depth, const void* data )
          -> void
        {
          parent_t::scoped_bind();
          glew::texture_t::texture_image_3d_multisample< T_internal_format >( width, height, depth, data );
          parent_t::_viewport[2] = width;
          parent_t::_viewport[3] = height;
        }
        
        auto image_3d_multisample( glew::gl_type::GLsizei size, glew::gl_type::GLsizei depth, const void* data )
          -> void
        { image_3d_multisample( size, size, depth, data ); }
        
        auto image_3d_multisample( glew::gl_type::GLsizei size, const void* data )
          -> void
        { image_3d_multisample( size, size, size, data ); }
        
      };
      
      template
      < typename glew::gl_type::GLenum T_internal_format
      >
      using texture1d_t = texture_t< GL_TEXTURE_1D, T_internal_format >;
      
      template
      < typename glew::gl_type::GLenum T_internal_format
      >
      using texture2d_t = texture_t< GL_TEXTURE_2D, T_internal_format >;
      
      template
      < typename glew::gl_type::GLenum T_internal_format
      >
      using texture3d_t = texture_t< GL_TEXTURE_3D, T_internal_format >;
      
    }
  }
}