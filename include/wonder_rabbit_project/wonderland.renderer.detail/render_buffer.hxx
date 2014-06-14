#pragma once

#include <type_traits>

#include <boost/optional.hpp>

#include "glew.detail/c.hxx"
#include "glew.detail/gl_type.hxx"
#include "glew.detail/error.hxx"
#include "glew.detail/wrapper.detail/render_buffer.hxx"

#include "texture.hxx"

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      
      struct render_buffer_t
        : protected glew::render_buffer_t
        , public std::enable_shared_from_this< render_buffer_t >
      {
      protected:
        const glew::gl_type::GLuint _render_buffer_id = 0;
        boost::optional< glew::gl_type::GLenum > _internal_format;
        
      public:
        
        render_buffer_t()
          : _render_buffer_id( generate_render_buffer() )
        { }
        
        ~render_buffer_t()
        { safe_delete_render_buffer( _render_buffer_id ); }
        
        auto render_buffer_id()
          -> glew::gl_type::GLuint
        { return _render_buffer_id; }
        
        auto bind()
          -> std::shared_ptr< render_buffer_t >
        {
          bind_render_buffer( _render_buffer_id );
          return shared_from_this();
        }
        
        auto scoped_bind()
          -> destruct_invoker_t
        {
          bind();
          return destruct_invoker_t( []{ bind_render_buffer(); } );
        }
        
        auto storage( glew::gl_type::GLenum internal_format, glew::gl_type::GLsizei width, glew::gl_type::GLsizei height )
          -> void
        { render_buffer_storage( *( _internal_format = internal_format ), width, height ); }
        
        template < class T >
        auto storage( std::shared_ptr< T > texture )
          -> void
        {
          const auto v = texture -> viewport();
          constexpr auto f = T::internal_format;
          render_buffer_storage( *( _internal_format = f ), v[2], v[3] );
        }
        
        auto internal_format() const
          -> glew::gl_type::GLenum
        {
          if ( _internal_format )
            return *_internal_format;
          
          throw std::logic_error( "render_buffer_t::internal_format: _internal_format is not set yet." );
        }
        
        template < class T >
        auto internal_format()
          -> void
        {
          constexpr auto f = T::internal_format;
          _internal_format = f;
        }
        
      };
      
    }
  }
}