#pragma once

#include <array>
#include <sstream>
#include <iostream>

#include "../c.hxx"
#include "../gl_type.hxx"
#include "../../destruct_invoker.hxx"

#include "get.hxx"
#include "texture.hxx"

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      namespace glew
      {
        struct render_buffer_t
        {
          template < typename gl_type::GLsizei T_count_of_render_buffers = 1 >
          static inline auto generate_render_buffers()
            -> std::array< gl_type::GLuint, T_count_of_render_buffers >
          {
            std::array< gl_type::GLuint, T_count_of_render_buffers > render_buffers;
            c::glGenRenderbuffers( T_count_of_render_buffers, &render_buffers[0] );
            return render_buffers;
          }
          
          template < class T = void >
          auto generate_render_buffer()
            -> gl_type::GLuint
          {
            gl_type::GLuint render_buffer;
            c::glGenRenderbuffers( 1, &render_buffer );
            return render_buffer;
          }
          
          template < typename gl_type::GLenum T_target = GL_RENDERBUFFER >
          static inline auto bind_render_buffer( gl_type::GLuint render_buffer_id = 0 )
            -> void
          { c::glBindRenderbuffer( T_target, render_buffer_id ); }
          
          template < typename gl_type::GLenum T_target = GL_RENDERBUFFER >
          static inline auto scoped_bind_render_buffer( gl_type::GLuint render_buffer_id )
            -> destruct_invoker_t
          {
            bind_render_buffer< T_target >( render_buffer_id );
            return destruct_invoker_t( [ ]{ bind_render_buffer(); } );
          }
          
          template < class T = void >
          auto render_buffer_storage( glew::gl_type::GLenum internal_format, glew::gl_type::GLsizei width, glew::gl_type::GLsizei height )
            -> void
          { c::glRenderbufferStorage( GL_RENDERBUFFER, internal_format, width, height ); }
          
          template < class T = void >
          static inline auto is_render_buffer( gl_type::GLuint render_buffer_id )
            -> bool
          { return c::glIsRenderbuffer( render_buffer_id ); }
          
          template < class T = void >
          static inline auto delete_render_buffer( gl_type::GLuint render_buffer_id )
            -> void
          { c::glDeleteRenderbuffers( 1, &render_buffer_id ); }
          
          template < class T >
          static inline auto delete_render_buffers( T render_buffer_ids )
            -> void
          { c::glDeleteRenderbuffers( render_buffer_ids.size(), &render_buffer_ids[0] ); }
          
          template < class T = void >
          static inline auto safe_delete_render_buffer( gl_type::GLuint render_buffer_id )
            -> void
          {
            if ( is_render_buffer( render_buffer_id ) )
              delete_render_buffer( render_buffer_id );
          }
          
        };
      }
    }
  }
}