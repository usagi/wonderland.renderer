#pragma once

#include <array>

#include "../c.hxx"
#include "../gl_type.hxx"
#include "../../destruct_invoker.hxx"

#include "texture.hxx"

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      namespace glew
      {
        struct frame_buffer_t
        {
          
          template < typename gl_type::GLsizei T_count_of_frame_buffers = 1 >
          static inline auto generate_frame_buffers()
            -> std::array< gl_type::GLuint, T_count_of_frame_buffers >
          {
            std::array< gl_type::GLuint, T_count_of_frame_buffers > frame_buffers;
            c::glGenFramebuffers( T_count_of_frame_buffers, &frame_buffers[0] );
            return frame_buffers;
          }
          
          template < class T = void >
          auto generate_frame_buffer()
            -> gl_type::GLuint
          {
            gl_type::GLuint frame_buffer;
            c::glGenFramebuffers( 1, &frame_buffer );
            return frame_buffer;
          }
          
          template < class T = void >
          auto bind_frame_buffer( gl_type::GLuint frame_buffer_id = 0 )
            -> void
          { c::glBindFramebuffer( GL_FRAMEBUFFER, frame_buffer_id ); }
          
          template < class T = void >
          auto scoped_bind_frame_buffer( gl_type::GLuint frame_buffer_id )
            -> destruct_invoker_t
          {
            bind_frame_buffer( frame_buffer_id );
            return destruct_invoker_t( [ this ]{ bind_frame_buffer(); } );
          }
          
          template < typename gl_type::GLenum T_attachment >
          auto frame_buffer_texture_1d( gl_type::GLuint texture_id )
            -> void
          { c::glFramebufferTexture1D( GL_FRAMEBUFFER, T_attachment, GL_TEXTURE_1D, texture_id, 0 ); }
          
          template < typename gl_type::GLenum T_attachment >
          auto frame_buffer_texture_2d( gl_type::GLuint texture_id )
            -> void
          { c::glFramebufferTexture2D( GL_FRAMEBUFFER, T_attachment, GL_TEXTURE_2D, texture_id, 0 ); }
          
          template < typename gl_type::GLenum T_attachment >
          auto frame_buffer_texture_3d( gl_type::GLuint texture_id )
            -> void
          { c::glFramebufferTexture3D( GL_FRAMEBUFFER, T_attachment, GL_TEXTURE_3D, texture_id, 0, 0 ); }

          template < class T = void >
          auto is_frame_buffer( gl_type::GLuint frame_buffer_id )
            -> bool
          { return c::glIsFramebuffer( frame_buffer_id ); }
          
          template < class T = void >
          auto delete_frame_buffer( gl_type::GLuint frame_buffer_id )
            -> void
          { c::glDeleteFramebuffers( 1, &frame_buffer_id ); }
          
          template < class T >
          auto delete_frame_buffers( T frame_buffer_ids )
            -> void
          { c::glDeleteFramebuffers( frame_buffer_ids.size(), &frame_buffer_ids[0] ); }
          
          template < class T = void >
          auto safe_delete_frame_buffer( gl_type::GLuint frame_buffer_id )
            -> void
          {
            if ( is_frame_buffer( frame_buffer_id ) )
              delete_frame_buffer( frame_buffer_id );
          }
          
        };
        
      }
    }
  }
}