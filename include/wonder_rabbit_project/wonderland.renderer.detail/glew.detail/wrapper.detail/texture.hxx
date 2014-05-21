#pragma once

#include <array>

#include <glm/gtc/constants.hpp>

#include "../c.hxx"
#include "../gl_type.hxx"
#include "../../destruct_invoker.hxx"

#include "get.hxx"
#include "exception.hxx"

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      namespace glew
      {
        struct texture_t
        {
          template < class T = void >
          static inline auto max_texture_units() -> gl_type::GLuint
          {
#ifdef GL_MAX_TEXTURE_UNITS
            return get_t::get< GL_MAX_TEXTURE_UNITS >();
#else
            throw exception::unsupported_in_platform( "GL_MAX_TEXTURE_UNITS is not supported in the platform." );
#endif
          }
          
          template < class T = void >
          static inline auto max_array_texture_layers() -> gl_type::GLuint
          { return get_t::get< GL_MAX_ARRAY_TEXTURE_LAYERS >(); }
          
          template < class T = void >
          static inline auto max_texture_size() -> gl_type::GLuint
          {
#ifdef GL_MAX_TEXTURE_SIZE
            return get_t::get< GL_MAX_TEXTURE_SIZE >();
#else
            throw exception::unsupported_in_platform( "GL_MAX_TEXTURE_SIZE is not supported in the platform." );
#endif
          }
          
          template < class T = void >
          static inline auto max_3d_texture_size() -> gl_type::GLuint
          {
#ifdef GL_MAX_3D_TEXTURE_SIZE
            return get_t::get< GL_MAX_3D_TEXTURE_SIZE >();
#else
            throw exception::unsupported_in_platform( "GL_MAX_3D_TEXTURE_SIZE is not supported in the platform." );
#endif
          }
          
          // note: texture_number is [ 0, 1, 2, .. ]. it is not GL_TEXTURE0(=0x84C0).
          template < typename gl_type::GLenum T_texture_number>
          static inline auto active_texture() -> void
          {
            static_assert( T_texture_number < GL_TEXTURE0, "T_texture_number is [ 0, 1, 2, .. ]. it is not GL_TEXTURE0(=0x84C0)." );
            c::glActiveTexture( GL_TEXTURE0 + T_texture_number );
          }
          
          // note: texture_number is [ 0, 1, 2, .. ]. it is not GL_TEXTURE0(=0x84C0).
          template < class T = void >
          static inline auto active_texture( gl_type::GLenum texture_number ) -> void
          {
            assert( texture_number < GL_TEXTURE0 );
            c::glActiveTexture( GL_TEXTURE0 + texture_number );
          }
          
          template < class T = void >
          static inline auto max_compute_texture_image_units() -> gl_type::GLuint
          { return get_t::get< GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS >(); }
          
          template < class T = void >
          static inline auto max_color_texture_samples() -> gl_type::GLuint
          { return get_t::get< GL_MAX_COLOR_TEXTURE_SAMPLES >(); }
          
          template < class T = void >
          static inline auto max_combined_texture_image_units() -> gl_type::GLuint
          { return get_t::get< GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS >(); }
          
          template < class T = void >
          static inline auto max_cube_map_texture_size() -> gl_type::GLuint
          { return get_t::get< GL_MAX_CUBE_MAP_TEXTURE_SIZE >(); }
          
          template < class T = void >
          static inline auto max_depth_texture_samples() -> gl_type::GLuint
          { return get_t::get< GL_MAX_DEPTH_TEXTURE_SAMPLES >(); }
          
          template < class T = void >
          static inline auto max_geometry_texture_image_units() -> gl_type::GLuint
          { return get_t::get< GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS >(); }
          
          template < class T = void >
          static inline auto max_program_texel_offset() -> gl_type::GLuint
          { return get_t::get< GL_MAX_PROGRAM_TEXEL_OFFSET >(); }
          
          template < class T = void >
          static inline auto max_rectangle_texture_size() -> gl_type::GLuint
          { return get_t::get< GL_MAX_RECTANGLE_TEXTURE_SIZE >(); }
          
          template < class T = void >
          static inline auto max_texture_buffer_size() -> gl_type::GLuint
          { return get_t::get< GL_MAX_TEXTURE_BUFFER_SIZE >(); }
          
          template < class T = void >
          static inline auto max_texture_image_units() -> gl_type::GLuint
          { return get_t::get< GL_MAX_TEXTURE_IMAGE_UNITS >(); }
          
          template < class T = void >
          static inline auto max_texture_lod_bias() -> gl_type::GLuint
          { return get_t::get< GL_MAX_TEXTURE_LOD_BIAS >(); }
          
          template < class T = void >
          static inline auto max_vertex_texture_image_units() -> gl_type::GLuint
          { return get_t::get< GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS >(); }
          
          template < class T = void >
          static inline auto sampler_binding() -> gl_type::GLuint
          { return get_t::get< GL_SAMPLER_BINDING >(); }
          
          template < class T = void >
          static inline auto texture_binding_1d() -> gl_type::GLuint
          { return get_t::get< GL_TEXTURE_BINDING_1D >(); }
          
          template < class T = void >
          static inline auto texture_binding_1d_array() -> gl_type::GLuint
          { return get_t::get< GL_TEXTURE_BINDING_1D_ARRAY >(); }
          
          template < class T = void >
          static inline auto texture_binding_2d() -> gl_type::GLuint
          { return get_t::get< GL_TEXTURE_BINDING_2D >(); }
          
          template < class T = void >
          static inline auto texture_binding_2d_array() -> gl_type::GLuint
          { return get_t::get< GL_TEXTURE_BINDING_2D_ARRAY >(); }
          
          template < class T = void >
          static inline auto texture_binding_2d_multisample() -> gl_type::GLuint
          { return get_t::get< GL_TEXTURE_BINDING_2D_MULTISAMPLE >(); }
          
          template < class T = void >
          static inline auto texture_binding_2d_multisample_array() -> gl_type::GLuint
          { return get_t::get< GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY >(); }
          
          template < class T = void >
          static inline auto texture_binding_3d() -> gl_type::GLuint
          { return get_t::get< GL_TEXTURE_BINDING_3D >(); }
          
          template < class T = void >
          static inline auto texture_binding_buffer() -> gl_type::GLuint
          { return get_t::get< GL_TEXTURE_BINDING_BUFFER >(); }
          
          template < class T = void >
          static inline auto texture_binding_cube_map() -> gl_type::GLuint
          { return get_t::get< GL_TEXTURE_BINDING_CUBE_MAP >(); }
          
          template < class T = void >
          static inline auto texture_binding_rectangle() -> gl_type::GLuint
          { return get_t::get< GL_TEXTURE_BINDING_RECTANGLE >(); }
          
          template < class T = void >
          static inline auto texture_buffer_offset_alignment() -> gl_type::GLuint
          { return get_t::get< GL_TEXTURE_BUFFER_OFFSET_ALIGNMENT >(); }
          
          template < typename gl_type::GLsizei T_count_of_textures = 1 >
          static inline auto generate_textures()
            -> std::array< gl_type::GLuint, T_count_of_textures >
          {
            std::array< gl_type::GLuint, T_count_of_textures > textures;
            c::glGenTextures( T_count_of_textures, &textures[0] );
            return textures;
          }
          
          template < class T = void >
          static inline auto generate_texture()
            -> gl_type::GLuint
          {
            gl_type::GLuint texture;
            c::glGenTextures( 1, &texture );
            return texture;
          }
          
          template < typename gl_type::GLenum T_target = GL_TEXTURE_2D >
          static inline auto bind_texture( gl_type::GLuint texture_id = 0 )
            -> void
          { c::glBindTexture( T_target, texture_id ); }
          
          template < typename gl_type::GLenum T_target = GL_TEXTURE_2D >
          static inline auto scoped_bind_texture( gl_type::GLuint texture_id = 0 )
            -> destruct_invoker_t
          {
            bind_texture< T_target >( texture_id );
            return destruct_invoker_t( []{ bind_texture< T_target >(); } );
          }
          
          template < class T = void >
          static inline auto texture_image_1d
          ( gl_type::GLenum  T_target = GL_TEXTURE_2D
          , gl_type::GLint   T_level  = 0
          , gl_type::GLint   T_internal_format = GL_DEPTH_COMPONENT24
          , gl_type::GLsizei T_width  = 512
          , gl_type::GLint   T_border = 0
          , gl_type::GLenum  T_format = GL_DEPTH_COMPONENT
          , gl_type::GLenum  T_type   = GL_UNSIGNED_INT
          , const void*      data     = nullptr
          )
            -> void
          {
            c::glTexImage1D
            ( T_target, T_level, T_internal_format
            , T_width
            , T_border
            , T_format, T_type
            , data
            );
          }
          
          template < class T = void >
          static inline auto texture_image_2d
          ( gl_type::GLenum  T_target = GL_TEXTURE_2D
          , gl_type::GLint   T_level  = 0
          , gl_type::GLint   T_internal_format = GL_DEPTH_COMPONENT24
          , gl_type::GLsizei T_width  = 512
          , gl_type::GLsizei T_height = 512
          , gl_type::GLint   T_border = 0
          , gl_type::GLenum  T_format = GL_DEPTH_COMPONENT
          , gl_type::GLenum  T_type   = GL_UNSIGNED_INT
          , const void*      data     = nullptr
          )
            -> void
          {
            c::glTexImage2D
            ( T_target, T_level, T_internal_format
            , T_width, T_height
            , T_border
            , T_format, T_type
            , data
            );
          }
          
          template < class T = void >
          static inline auto texture_image_2d_multisample
          ( gl_type::GLenum  T_target = GL_TEXTURE_2D
          , gl_type::GLsizei T_sample = 0
          , gl_type::GLint   T_internal_format = GL_DEPTH_COMPONENT24
          , gl_type::GLsizei T_width  = 512
          , gl_type::GLsizei T_height = 512
          , gl_type::GLint   T_border = 0
          , gl_type::GLenum  T_format = GL_DEPTH_COMPONENT
          , gl_type::GLenum  T_type   = GL_UNSIGNED_INT
          , const void*      data     = nullptr
          )
            -> void
          {
            c::glTexImage2D
            ( T_target, T_sample, T_internal_format
            , T_width, T_height
            , T_border
            , T_format, T_type
            , data
            );
          }
          
          template < class T = void >
          static inline auto texture_image_3d
          ( gl_type::GLenum  T_target = GL_TEXTURE_3D
          , gl_type::GLint   T_level  = 0
          , gl_type::GLint   T_internal_format = GL_DEPTH_COMPONENT24
          , gl_type::GLsizei T_width  = 512
          , gl_type::GLsizei T_height = 512
          , gl_type::GLsizei T_depth  = 512
          , gl_type::GLint   T_border = 0
          , gl_type::GLenum  T_format = GL_DEPTH_COMPONENT
          , gl_type::GLenum  T_type   = GL_UNSIGNED_INT
          , const void*      data     = nullptr
          )
            -> void
          {
            c::glTexImage3D
            ( T_target, T_level, T_internal_format
            , T_width, T_height, T_depth
            , T_border
            , T_format, T_type
            , data
            );
          }
          
          template < class T = void >
          static inline auto texture_image_3d_multisample
          ( gl_type::GLenum  T_target = GL_TEXTURE_3D
          , gl_type::GLsizei T_sample = 0
          , gl_type::GLint   T_internal_format = GL_DEPTH_COMPONENT24
          , gl_type::GLsizei T_width  = 512
          , gl_type::GLsizei T_height = 512
          , gl_type::GLsizei T_depth  = 512
          , gl_type::GLint   T_border = 0
          , gl_type::GLenum  T_format = GL_DEPTH_COMPONENT
          , gl_type::GLenum  T_type   = GL_UNSIGNED_INT
          , const void*      data     = nullptr
          )
            -> void
          {
            c::glTexImage3D
            ( T_target, T_sample, T_internal_format
            , T_width, T_height, T_depth
            , T_border
            , T_format, T_type
            , data
            );
          }
          
          // has template specialize version
          template < typename gl_type::GLint T_internal_format = GL_DEPTH_COMPONENT24 >
          static inline auto texture_image_1d( gl_type::GLsizei width, const void* data = nullptr )
            -> void
          { throw std::logic_error( "texture_image_1d_depth: T_internal_format is not implemented" ); }
          
          template < typename gl_type::GLint T_internal_format = GL_DEPTH_COMPONENT24 >
          static inline auto texture_image_1d()
            -> void
          { texture_image_2d< T_internal_format >( max_texture_size() ); }
          
          // has template specialize version
          template < typename gl_type::GLint T_internal_format = GL_DEPTH_COMPONENT24 >
          static inline auto texture_image_2d( gl_type::GLsizei width, gl_type::GLsizei height, const void* data = nullptr )
            -> void
          { throw std::logic_error( "texture_image_2d: T_internal_format is not implemented" ); }
          
          template < typename gl_type::GLint T_internal_format = GL_DEPTH_COMPONENT24 >
          static inline auto texture_image_2d()
            -> void
          { texture_image_2d< T_internal_format >( max_texture_size() ); }

          template < typename gl_type::GLint T_internal_format = GL_DEPTH_COMPONENT24 >
          static inline auto texture_image_2d( gl_type::GLsizei size, const void* data = nullptr )
            -> void
          { texture_image_2d< T_internal_format >( size, size, data ); }
          
          // has template specialize version
          template < typename gl_type::GLint T_internal_format = GL_DEPTH_COMPONENT24 >
          static inline auto texture_image_2d_multisample( gl_type::GLsizei width, gl_type::GLsizei height, const void* data, gl_type::GLsizei samples )
            -> void
          { throw std::logic_error( "texture_image_2d_multisample: T_internal_format is not implemented" ); }
          
          template < typename gl_type::GLint T_internal_format = GL_DEPTH_COMPONENT24 >
          static inline auto texture_image_2d_multisample( gl_type::GLsizei width, gl_type::GLsizei height, gl_type::GLsizei size, const void* data, gl_type::GLsizei samples )
            -> void
          { texture_image_2d_multisample< T_internal_format >( width, height, data, glm::log2<float>( std::min( width, height ) ) ); }
          
          template < typename gl_type::GLint T_internal_format = GL_DEPTH_COMPONENT24 >
          static inline auto texture_image_2d_multisample( gl_type::GLsizei size, const void* data, gl_type::GLsizei samples )
            -> void
          { texture_image_2d_multisample< T_internal_format >( size, size, data, samples ); }
          
          template < typename gl_type::GLint T_internal_format = GL_DEPTH_COMPONENT24 >
          static inline auto texture_image_2d_multisample( gl_type::GLsizei size, const void* data )
            -> void
          { texture_image_2d_multisample< T_internal_format >( size, size, data, glm::log2<float>( size ) ); }
          
          // has template specialize version
          template < typename gl_type::GLint T_internal_format = GL_DEPTH_COMPONENT24 >
          static inline auto texture_image_3d( gl_type::GLsizei width, gl_type::GLsizei height, gl_type::GLsizei depth, const void* data = nullptr )
            -> void
          { throw std::logic_error( "texture_image_3d: T_internal_format is not implemented" ); }
          
          template < typename gl_type::GLint T_internal_format = GL_DEPTH_COMPONENT24 >
          static inline auto texture_image_3d()
            -> void
          { texture_image_3d< T_internal_format >( max_texture_size() ); }

          template < typename gl_type::GLint T_internal_format = GL_DEPTH_COMPONENT24 >
          static inline auto texture_image_3d( gl_type::GLsizei size, const void* data = nullptr )
            -> void
          { texture_image_3d< T_internal_format >( size, size, size, data ); }
          
          template < typename gl_type::GLint T_internal_format = GL_DEPTH_COMPONENT24 >
          static inline auto texture_image_3d( gl_type::GLsizei size, gl_type::GLsizei depth, const void* data = nullptr )
            -> void
          { texture_image_3d< T_internal_format >( size, size, depth, data ); }
          
          // has template specialize version
          template < typename gl_type::GLint T_internal_format = GL_DEPTH_COMPONENT24 >
          static inline auto texture_image_3d_multisample( gl_type::GLsizei width, gl_type::GLsizei height, gl_type::GLsizei depth, const void* data, gl_type::GLsizei samples )
            -> void
          { throw std::logic_error( "texture_image_3d_multisample: T_internal_format is not implemented" ); }
          
          template < typename gl_type::GLint T_internal_format = GL_DEPTH_COMPONENT24 >
          static inline auto texture_image_3d_multisample( gl_type::GLsizei width, gl_type::GLsizei height, gl_type::GLsizei depth, const void* data )
            -> void
          { texture_image_3d_multisample< T_internal_format >( width, height, depth, data, glm::log2<float>( std::min( std::min( width, height), depth ) ) ); }
          
          template < typename gl_type::GLint T_internal_format = GL_DEPTH_COMPONENT24 >
          static inline auto texture_image_3d_multisample( gl_type::GLsizei size, gl_type::GLsizei depth, const void* data )
            -> void
          { texture_image_3d_multisample< T_internal_format >( size, size, depth, data, glm::log2<float>( std::min( size, depth ) ) ); }
          
          template < typename gl_type::GLint T_internal_format = GL_DEPTH_COMPONENT24 >
          static inline auto texture_image_3d_multisample( gl_type::GLsizei size, const void* data, gl_type::GLsizei samples )
            -> void
          { texture_image_3d_multisample< T_internal_format >( size, size, size, data, glm::log2<float>( size ) ); }
          
          template < class T = void >
          static inline auto is_texture( gl_type::GLuint texture_id )
            -> bool
          { return c::glIsTexture( texture_id ); }
          
          template < class T = void >
          static inline auto delete_texture( gl_type::GLuint texture_id )
            -> void
          { c::glDeleteTextures( 1, &texture_id ); }
          
          template < class T = void >
          static inline auto delete_texture( T texture_ids )
            -> void
          { c::glDeleteTextures( texture_ids.size(), &texture_ids[0] ); }
          
          template < class T = void >
          static inline auto safe_delete_texture( gl_type::GLuint texture_id )
            -> void
          {
            if ( is_texture( texture_id ) )
              delete_texture( texture_id );
          }
          
        };
        
        // 1d
        
        template < >
        inline auto texture_t::texture_image_1d< GL_DEPTH_COMPONENT16 >( gl_type::GLsizei width, const void* data )
          -> void
        {
          texture_image_1d
          ( GL_TEXTURE_1D
          , 0, GL_DEPTH_COMPONENT16
          , width
          , 0
          , GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT
          , data
          );
        }
        
        template < >
        inline auto texture_t::texture_image_1d< GL_DEPTH_COMPONENT24 >( gl_type::GLsizei width, const void* data )
          -> void
        {
          texture_image_1d
          ( GL_TEXTURE_1D
          , 0, GL_DEPTH_COMPONENT24
          , width
          , 0
          , GL_DEPTH_COMPONENT, GL_UNSIGNED_INT
          , data
          );
        }
        
        template < >
        inline auto texture_t::texture_image_1d< GL_DEPTH_COMPONENT32 >( gl_type::GLsizei width, const void* data )
          -> void
        {
          texture_image_1d
          ( GL_TEXTURE_1D
          , 0, GL_DEPTH_COMPONENT32
          , width
          , 0
          , GL_DEPTH_COMPONENT, GL_UNSIGNED_INT
          , data
          );
        }
        
        template < >
        inline auto texture_t::texture_image_1d< GL_DEPTH_COMPONENT32F >( gl_type::GLsizei width, const void* data )
          -> void
        {
          texture_image_1d
          ( GL_TEXTURE_1D
          , 0, GL_DEPTH_COMPONENT32F
          , width
          , 0
          , GL_DEPTH_COMPONENT, GL_FLOAT
          , data
          );
        }
        
        template < >
        inline auto texture_t::texture_image_1d< GL_DEPTH24_STENCIL8 >( gl_type::GLsizei width, const void* data )
          -> void
        {
          texture_image_1d
          ( GL_TEXTURE_1D
          , 0, GL_DEPTH24_STENCIL8
          , width
          , 0
          , GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8
          , data
          );
        }
        
        template < >
        inline auto texture_t::texture_image_1d< GL_DEPTH32F_STENCIL8 >( gl_type::GLsizei width, const void* data )
          -> void
        {
          texture_image_1d
          ( GL_TEXTURE_1D
          , 0, GL_DEPTH32F_STENCIL8
          , width
          , 0
          , GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV
          , data
          );
        }
        
        // 2d
        
        template < >
        inline auto texture_t::texture_image_2d< GL_DEPTH_COMPONENT16 >( gl_type::GLsizei width, gl_type::GLsizei height, const void* data )
          -> void
        {
          texture_image_2d
          ( GL_TEXTURE_2D
          , 0, GL_DEPTH_COMPONENT16
          , width, height
          , 0
          , GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT
          , data
          );
        }
        
        template < >
        inline auto texture_t::texture_image_2d< GL_DEPTH_COMPONENT24 >( gl_type::GLsizei width, gl_type::GLsizei height, const void* data )
          -> void
        {
          texture_image_2d
          ( GL_TEXTURE_2D
          , 0, GL_DEPTH_COMPONENT24
          , width, height
          , 0
          , GL_DEPTH_COMPONENT, GL_UNSIGNED_INT
          , data
          );
        }
        
        template < >
        inline auto texture_t::texture_image_2d< GL_DEPTH_COMPONENT32 >( gl_type::GLsizei width, gl_type::GLsizei height, const void* data )
          -> void
        {
          texture_image_2d
          ( GL_TEXTURE_2D
          , 0, GL_DEPTH_COMPONENT32
          , width, height
          , 0
          , GL_DEPTH_COMPONENT, GL_UNSIGNED_INT
          , data
          );
        }
        
        template < >
        inline auto texture_t::texture_image_2d< GL_DEPTH_COMPONENT32F >( gl_type::GLsizei width, gl_type::GLsizei height, const void* data )
          -> void
        {
          texture_image_2d
          ( GL_TEXTURE_2D
          , 0, GL_DEPTH_COMPONENT32F
          , width, height
          , 0
          , GL_DEPTH_COMPONENT, GL_FLOAT
          , data
          );
        }
        
        template < >
        inline auto texture_t::texture_image_2d< GL_DEPTH24_STENCIL8 >( gl_type::GLsizei width, gl_type::GLsizei height, const void* data )
          -> void
        {
          texture_image_2d
          ( GL_TEXTURE_2D
          , 0, GL_DEPTH24_STENCIL8
          , width, height
          , 0
          , GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8
          , data
          );
        }
        
        template < >
        inline auto texture_t::texture_image_2d< GL_DEPTH32F_STENCIL8 >( gl_type::GLsizei width, gl_type::GLsizei height, const void* data )
          -> void
        {
          texture_image_2d
          ( GL_TEXTURE_2D
          , 0, GL_DEPTH32F_STENCIL8
          , width, height
          , 0
          , GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV
          , data
          );
        }
        
        template < >
        inline auto texture_t::texture_image_2d_multisample< GL_DEPTH_COMPONENT16 >( gl_type::GLsizei width, gl_type::GLsizei height, const void* data, gl_type::GLsizei samples )
          -> void
        {
          texture_image_2d_multisample
          ( GL_TEXTURE_2D
          , samples, GL_DEPTH_COMPONENT16
          , width, height
          , 0
          , GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT
          , data
          );
        }
        
        template < >
        inline auto texture_t::texture_image_2d_multisample< GL_DEPTH_COMPONENT24 >( gl_type::GLsizei width, gl_type::GLsizei height, const void* data, gl_type::GLsizei samples )
          -> void
        {
          texture_image_2d_multisample
          ( GL_TEXTURE_2D
          , samples, GL_DEPTH_COMPONENT24
          , width, height
          , 0
          , GL_DEPTH_COMPONENT, GL_UNSIGNED_INT
          , data
          );
        }
        
        template < >
        inline auto texture_t::texture_image_2d_multisample< GL_DEPTH_COMPONENT32 >( gl_type::GLsizei width, gl_type::GLsizei height, const void* data, gl_type::GLsizei samples )
          -> void
        {
          texture_image_2d_multisample
          ( GL_TEXTURE_2D
          , samples, GL_DEPTH_COMPONENT32
          , width, height
          , 0
          , GL_DEPTH_COMPONENT, GL_UNSIGNED_INT
          , data
          );
        }
        
        template < >
        inline auto texture_t::texture_image_2d_multisample< GL_DEPTH_COMPONENT32F >( gl_type::GLsizei width, gl_type::GLsizei height, const void* data, gl_type::GLsizei samples )
          -> void
        {
          texture_image_2d_multisample
          ( GL_TEXTURE_2D
          , samples, GL_DEPTH_COMPONENT32F
          , width, height
          , 0
          , GL_DEPTH_COMPONENT, GL_FLOAT
          , data
          );
        }
        
        template < >
        inline auto texture_t::texture_image_2d_multisample< GL_DEPTH24_STENCIL8 >( gl_type::GLsizei width, gl_type::GLsizei height, const void* data, gl_type::GLsizei samples )
          -> void
        {
          texture_image_2d_multisample
          ( GL_TEXTURE_2D
          , samples, GL_DEPTH24_STENCIL8
          , width, height
          , 0
          , GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8
          , data
          );
        }
        
        template < >
        inline auto texture_t::texture_image_2d_multisample< GL_DEPTH32F_STENCIL8 >( gl_type::GLsizei width, gl_type::GLsizei height, const void* data, gl_type::GLsizei samples )
          -> void
        {
          texture_image_2d_multisample
          ( GL_TEXTURE_2D
          , samples, GL_DEPTH32F_STENCIL8
          , width, height
          , 0
          , GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV
          , data
          );
        }
        
        // 3d
        
        template < >
        inline auto texture_t::texture_image_3d< GL_DEPTH_COMPONENT16 >( gl_type::GLsizei width, gl_type::GLsizei height, gl_type::GLsizei depth, const void* data )
          -> void
        {
          texture_image_3d
          ( GL_TEXTURE_3D
          , 0, GL_DEPTH_COMPONENT16
          , width, height, depth
          , 0
          , GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT
          , data
          );
        }
        
        template < >
        inline auto texture_t::texture_image_3d< GL_DEPTH_COMPONENT24 >( gl_type::GLsizei width, gl_type::GLsizei height, gl_type::GLsizei depth, const void* data )
          -> void
        {
          texture_image_3d
          ( GL_TEXTURE_3D
          , 0, GL_DEPTH_COMPONENT24
          , width, height, depth
          , 0
          , GL_DEPTH_COMPONENT, GL_UNSIGNED_INT
          , data
          );
        }
        
        template < >
        inline auto texture_t::texture_image_3d< GL_DEPTH_COMPONENT32 >( gl_type::GLsizei width, gl_type::GLsizei height, gl_type::GLsizei depth, const void* data )
          -> void
        {
          texture_image_3d
          ( GL_TEXTURE_3D
          , 0, GL_DEPTH_COMPONENT32
          , width, height, depth
          , 0
          , GL_DEPTH_COMPONENT, GL_UNSIGNED_INT
          , data
          );
        }
        
        template < >
        inline auto texture_t::texture_image_3d< GL_DEPTH_COMPONENT32F >( gl_type::GLsizei width, gl_type::GLsizei height, gl_type::GLsizei depth, const void* data )
          -> void
        {
          texture_image_3d
          ( GL_TEXTURE_3D
          , 0, GL_DEPTH_COMPONENT32F
          , width, height, depth
          , 0
          , GL_DEPTH_COMPONENT, GL_FLOAT
          , data
          );
        }
        
        template < >
        inline auto texture_t::texture_image_3d< GL_DEPTH24_STENCIL8 >( gl_type::GLsizei width, gl_type::GLsizei height, gl_type::GLsizei depth, const void* data )
          -> void
        {
          texture_image_3d
          ( GL_TEXTURE_3D
          , 0, GL_DEPTH24_STENCIL8
          , width, height, depth
          , 0
          , GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8
          , data
          );
        }
        
        template < >
        inline auto texture_t::texture_image_3d< GL_DEPTH32F_STENCIL8 >( gl_type::GLsizei width, gl_type::GLsizei height, gl_type::GLsizei depth, const void* data )
          -> void
        {
          texture_image_3d
          ( GL_TEXTURE_3D
          , 0, GL_DEPTH32F_STENCIL8
          , width, height, depth
          , 0
          , GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV
          , data
          );
        }
        
        template < >
        inline auto texture_t::texture_image_3d_multisample< GL_DEPTH_COMPONENT16 >( gl_type::GLsizei width, gl_type::GLsizei height, gl_type::GLsizei depth, const void* data, gl_type::GLsizei samples )
          -> void
        {
          texture_image_3d_multisample
          ( GL_TEXTURE_3D
          , samples, GL_DEPTH_COMPONENT16
          , width, height, depth
          , 0
          , GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT
          , data
          );
        }
        
        template < >
        inline auto texture_t::texture_image_3d_multisample< GL_DEPTH_COMPONENT24 >( gl_type::GLsizei width, gl_type::GLsizei height, gl_type::GLsizei depth, const void* data, gl_type::GLsizei samples )
          -> void
        {
          texture_image_3d_multisample
          ( GL_TEXTURE_3D
          , samples, GL_DEPTH_COMPONENT24
          , width, height, depth
          , 0
          , GL_DEPTH_COMPONENT, GL_UNSIGNED_INT
          , data
          );
        }
        
        template < >
        inline auto texture_t::texture_image_3d_multisample< GL_DEPTH_COMPONENT32 >( gl_type::GLsizei width, gl_type::GLsizei height, gl_type::GLsizei depth, const void* data, gl_type::GLsizei samples )
          -> void
        {
          texture_image_3d_multisample
          ( GL_TEXTURE_3D
          , samples, GL_DEPTH_COMPONENT32
          , width, height, depth
          , 0
          , GL_DEPTH_COMPONENT, GL_UNSIGNED_INT
          , data
          );
        }
        
        template < >
        inline auto texture_t::texture_image_3d_multisample< GL_DEPTH_COMPONENT32F >( gl_type::GLsizei width, gl_type::GLsizei height, gl_type::GLsizei depth, const void* data, gl_type::GLsizei samples )
          -> void
        {
          texture_image_3d_multisample
          ( GL_TEXTURE_3D
          , samples, GL_DEPTH_COMPONENT32F
          , width, height, depth
          , 0
          , GL_DEPTH_COMPONENT, GL_FLOAT
          , data
          );
        }
        
        template < >
        inline auto texture_t::texture_image_3d_multisample< GL_DEPTH24_STENCIL8 >( gl_type::GLsizei width, gl_type::GLsizei height, gl_type::GLsizei depth, const void* data, gl_type::GLsizei samples )
          -> void
        {
          texture_image_3d_multisample
          ( GL_TEXTURE_3D
          , samples, GL_DEPTH24_STENCIL8
          , width, height, depth
          , 0
          , GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8
          , data
          );
        }
        
        template < >
        inline auto texture_t::texture_image_3d_multisample< GL_DEPTH32F_STENCIL8 >( gl_type::GLsizei width, gl_type::GLsizei height, gl_type::GLsizei depth, const void* data, gl_type::GLsizei samples )
          -> void
        {
          texture_image_3d_multisample
          ( GL_TEXTURE_3D
          , samples, GL_DEPTH32F_STENCIL8
          , width, height, depth
          , 0
          , GL_DEPTH_STENCIL, GL_FLOAT_32_UNSIGNED_INT_24_8_REV
          , data
          );
        }
        
      }
    }
  }
}