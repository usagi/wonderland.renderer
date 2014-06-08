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
        struct frame_buffer_t
        {
          
          static constexpr auto attachment( gl_type::GLenum base_internal_format )
            -> gl_type::GLenum
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
          
#define WRP_GLEW_CHECK_FRAME_BUFFER_STATUS check_frame_buffer_status( __FILE__, __LINE__ );
          
          template < class T = void >
          static inline auto check_frame_buffer_status( const std::string& file = "?", const unsigned line = 0 )
            -> void
          {
            const auto status = c::glCheckFramebufferStatus( GL_FRAMEBUFFER );
            if ( status != GL_FRAMEBUFFER_COMPLETE )
            {
              std::stringstream message;
              message << "glCheckFramebufferStatus ( from " << file << " +" << line << " ) : " << status << " ";
              switch ( status )
              {
#define WRP_TMP( x ) \
                case x:\
                  message << # x ;\
                  break;
                WRP_TMP( GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT         )
                WRP_TMP( GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT )
                WRP_TMP( GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER        )
                WRP_TMP( GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER        )
                WRP_TMP( GL_FRAMEBUFFER_UNSUPPORTED                   )
                WRP_TMP( GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE        )
                WRP_TMP( GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS      )
#undef WRP_TMP
                default: message << "(unexpected status)";
              }
              throw std::runtime_error( message.str() );
            }
          }
          
          template < class T = void >
          static inline auto draw_buffer( const gl_type::GLenum flag = GL_NONE )
            -> void
          { c::glDrawBuffer( flag ); }
          
          template < class T = void >
          static inline auto scoped_draw_buffer( const gl_type::GLenum flag = GL_NONE )
            -> destruct_invoker_t
          {
            const auto backup = get_t::get< GL_DRAW_BUFFER >();
            draw_buffer( flag );
            return destruct_invoker_t( [ backup ]{ draw_buffer( backup ); } );
          }
          
          template < class T = void >
          static inline auto read_buffer( const gl_type::GLenum flag = GL_NONE )
            -> void
          { c::glReadBuffer( flag ); }
          
          template < class T = void >
          static inline auto scoped_read_buffer( const gl_type::GLenum flag = GL_NONE )
            -> destruct_invoker_t
          {
            const auto backup = get_t::get< GL_READ_BUFFER >();
            read_buffer( flag );
            return destruct_invoker_t( [ backup ]{ read_buffer( backup ); } );
          }
          
          template < typename gl_type::GLsizei T_count_of_frame_buffers = 1 >
          static inline auto generate_frame_buffers()
            -> std::array< gl_type::GLuint, T_count_of_frame_buffers >
          {
            std::array< gl_type::GLuint, T_count_of_frame_buffers > frame_buffers;
            c::glGenFramebuffers( T_count_of_frame_buffers, &frame_buffers[0] );
            return frame_buffers;
          }
          
          template < class T = void >
          static inline auto generate_frame_buffer()
            -> gl_type::GLuint
          {
            gl_type::GLuint frame_buffer;
            c::glGenFramebuffers( 1, &frame_buffer );
            return frame_buffer;
          }
          
          template < typename gl_type::GLenum T_target = GL_FRAMEBUFFER >
          static inline auto bind_frame_buffer( gl_type::GLuint frame_buffer_id = 0 )
            -> void
          { c::glBindFramebuffer( T_target, frame_buffer_id ); }
          
          template < typename gl_type::GLenum T_target = GL_FRAMEBUFFER >
          static inline auto scoped_bind_frame_buffer( gl_type::GLuint frame_buffer_id )
            -> destruct_invoker_t
          {
            const auto backup = get_t::get< GL_FRAMEBUFFER_BINDING >();
            bind_frame_buffer< T_target >( frame_buffer_id );
            return destruct_invoker_t( [ backup ]{ bind_frame_buffer( backup ); } );
          }
          
#ifdef GL_VERSION_3_2
          template < typename gl_type::GLenum T_attachment >
          static inline auto frame_buffer_texture( gl_type::GLuint texture_id = 0 )
            -> void
          { c::glFramebufferTexture( GL_FRAMEBUFFER, T_attachment, texture_id, 0 ); }
#endif
          
          template < typename gl_type::GLenum T_attachment >
          static inline auto frame_buffer_texture_1d( gl_type::GLuint texture_id = 0 )
            -> void
          { c::glFramebufferTexture1D( GL_FRAMEBUFFER, T_attachment, GL_TEXTURE_1D, texture_id, 0 ); }
          
          template < typename gl_type::GLenum T_attachment >
          static inline auto frame_buffer_texture_2d( gl_type::GLuint texture_id = 0, gl_type::GLenum texture_target = GL_TEXTURE_2D )
            -> void
          { c::glFramebufferTexture2D( GL_FRAMEBUFFER, T_attachment, texture_target, texture_id, 0 ); }
          
          template < typename gl_type::GLenum T_attachment >
          static inline auto frame_buffer_texture_3d( gl_type::GLuint texture_id = 0 )
            -> void
          { c::glFramebufferTexture3D( GL_FRAMEBUFFER, T_attachment, GL_TEXTURE_3D, texture_id, 0, 0 ); }

          template < class T = void >
          static inline auto is_frame_buffer( gl_type::GLuint frame_buffer_id )
            -> bool
          { return c::glIsFramebuffer( frame_buffer_id ); }
          
          template < class T = void >
          static inline auto delete_frame_buffer( gl_type::GLuint frame_buffer_id )
            -> void
          { c::glDeleteFramebuffers( 1, &frame_buffer_id ); }
          
          template < class T >
          static inline auto delete_frame_buffers( T frame_buffer_ids )
            -> void
          { c::glDeleteFramebuffers( frame_buffer_ids.size(), &frame_buffer_ids[0] ); }
          
          template < class T = void >
          static inline auto safe_delete_frame_buffer( gl_type::GLuint frame_buffer_id )
            -> void
          {
            if ( is_frame_buffer( frame_buffer_id ) )
              delete_frame_buffer( frame_buffer_id );
          }
          
          template < class T = void >
          static inline auto frame_buffer_render_buffer( gl_type::GLenum attachment, gl_type::GLuint render_buffer_id )
            -> void
          { c::glFramebufferRenderbuffer( GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, render_buffer_id ); }
          
        };
        
      }
    }
  }
}