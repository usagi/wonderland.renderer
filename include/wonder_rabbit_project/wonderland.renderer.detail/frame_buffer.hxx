#pragma once

#include <type_traits>

#include "glew.detail/c.hxx"
#include "glew.detail/gl_type.hxx"
#include "glew.detail/wrapper.detail/frame_buffer.hxx"

#include "texture.hxx"

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      
      struct frame_buffer_t
        : protected glew::frame_buffer_t
      {
      protected:
        const glew::gl_type::GLuint _frame_buffer_id = 0;
        
        static constexpr auto attachment( const glew::gl_type::GLenum base_internal_format )
          -> glew::gl_type::GLenum
        {
          return
              base_internal_format == GL_DEPTH_COMPONENT
            ? GL_DEPTH_ATTACHMENT
            : base_internal_format == GL_DEPTH_STENCIL
            ? GL_DEPTH_STENCIL_ATTACHMENT
            : base_internal_format == GL_STENCIL_INDEX
            ? GL_STENCIL_ATTACHMENT
            : GL_COLOR_ATTACHMENT0;
        }
        
        template < class T >
        auto _texture( T texture ) -> void
        {
          switch( T::target )
          {
            case GL_TEXTURE_1D:
              frame_buffer_texture_1d< attachment( T::base_internal_format ) >( texture.texture_id() );
              return;
            case GL_TEXTURE_2D:
              frame_buffer_texture_2d< attachment( T::base_internal_format ) >( texture.texture_id() );
              return;
            case GL_TEXTURE_3D:
              frame_buffer_texture_3d< attachment( T::base_internal_format ) >( texture.texture_id() );
              return;
          }
        }
        
        template < class T, class ... Ts >
        auto _bind_textures_bind( T texture, Ts ... textures ) -> void
        {
          _texture( texture );
          _bind_textures_bind( textures ... );
        }
        
      public:
        frame_buffer_t()
          : _frame_buffer_id( generate_frame_buffer() )
        { }
        
        ~frame_buffer_t()
        { safe_delete_frame_buffer( _frame_buffer_id ); }
        
        auto frame_buffer_id() -> decltype( _frame_buffer_id )
        { return _frame_buffer_id; }
        
        auto bind() -> void
        { bind_frame_buffer( _frame_buffer_id ); }
        
        auto scoped_bind() -> destruct_invoker_t
        { return scoped_bind_frame_buffer( _frame_buffer_id ); }
        
        template < class ... Ts >
        auto bind_textures( Ts ... textures ) -> void
        {
          scoped_bind();
          _bind_textures_bind( textures ... );
        }
        
        template < class T >
        auto bind_texture( T texture ) -> void
        {
          scoped_bind();
          _texture( texture );
        }
        
      };
      
    }
  }
}