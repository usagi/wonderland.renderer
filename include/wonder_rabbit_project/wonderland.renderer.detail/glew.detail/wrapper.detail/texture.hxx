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
          
          static constexpr auto base_internal_format( gl_type::GLenum internal_format )
            -> gl_type::GLenum
          {
            return
              (  internal_format == GL_DEPTH_COMPONENT16
              or internal_format == GL_DEPTH_COMPONENT24
              or internal_format == GL_DEPTH_COMPONENT32
              or internal_format == GL_DEPTH_COMPONENT32F
              )  ? GL_DEPTH_COMPONENT
            : (  internal_format == GL_DEPTH24_STENCIL8
              or internal_format == GL_DEPTH32F_STENCIL8
              )  ? GL_DEPTH_STENCIL
            : (  internal_format == GL_STENCIL_INDEX8
              )  ? GL_STENCIL
            : (  internal_format == GL_RGBA8
              )  ? GL_RGBA
            : (  internal_format == GL_RGB8
              )  ? GL_RGB
            : (  internal_format == GL_RG8
              )  ? GL_RG
            : (  internal_format == GL_R8
              )  ? GL_R
            : 0;
          }
          
          static constexpr auto type( gl_type::GLenum internal_format )
            -> gl_type::GLenum
          {
            return
                internal_format == GL_DEPTH_COMPONENT16  ? GL_UNSIGNED_SHORT
              : internal_format == GL_DEPTH_COMPONENT24  ? GL_UNSIGNED_INT
              : internal_format == GL_DEPTH_COMPONENT32  ? GL_UNSIGNED_INT
              : internal_format == GL_DEPTH_COMPONENT32F ? GL_FLOAT
              : internal_format == GL_DEPTH24_STENCIL8   ? GL_UNSIGNED_INT_24_8
              : internal_format == GL_DEPTH32F_STENCIL8  ? GL_FLOAT_32_UNSIGNED_INT_24_8_REV
              : (  internal_format == GL_RGBA8
                or internal_format == GL_RGB8
                or internal_format == GL_RG8
                or internal_format == GL_R8
                )  ? GL_UNSIGNED_BYTE
              : 0;
          }
          
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
#ifdef GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS
          { return get_t::get< GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS >(); }
#else
          { throw exception::unsupported_in_platform( "GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS is not supported in the platform." ); }
#endif
          
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
#ifdef GL_TEXTURE_BINDING_RECTANGLE
          { return get_t::get< GL_TEXTURE_BINDING_RECTANGLE >(); }
#else
          { throw exception::unsupported_in_platform( "GL_TEXTURE_BINDING_RECTANGLE is not supported in the platform." ); }
#endif
          
          template < class T = void >
          static inline auto texture_buffer_offset_alignment() -> gl_type::GLuint
#ifdef GL_TEXTURE_BUFFER_OFFSET_ALIGNMENT
          { return get_t::get< GL_TEXTURE_BUFFER_OFFSET_ALIGNMENT >(); }
#else
          { throw exception::unsupported_in_platform( "GL_TEXTURE_BUFFER_OFFSET_ALIGNMENT is not supported in the platform." ); }
#endif
          
          template < typename gl_type::GLsizei T_count_of_textures = 1 >
          static inline auto generate_textures()
            -> std::vector< gl_type::GLuint >
          {
            std::vector< gl_type::GLuint > textures( T_count_of_textures );
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
          
          template < typename gl_type::GLenum T_target = GL_TEXTURE_2D >
          static inline auto generate_mipmap()
            -> void
          { c::glGenerateMipmap( T_target ); }
          
          template < class T = void >
          static inline auto generate_mipmap( gl_type::GLenum target )
            -> void
          { c::glGenerateMipmap( target ); }
          
          // has template specializing
          template
          < typename gl_type::GLenum T_target
          , class T_size
          >
          static inline auto texture_storage
          ( gl_type::GLsizei
          , gl_type::GLenum
          , T_size
          )
            -> void
          {
            throw std::logic_error( "texture_storage: unkown T_target or T_size" );
          }
          
          // has template specializing
          template
          < typename gl_type::GLenum T_target
          , class T_size
          >
          static inline auto texture_storage
          ( gl_type::GLenum
          , T_size
          )
            -> void
          {
            throw std::logic_error( "texture_storage: unkown T_target or T_size" );
          }
          
          template
          < typename gl_type::GLenum T_target
          , class T_size
          >
          static inline auto texture_sub_image
          ( gl_type::GLenum
          , T_size
          , const void*
          )
            -> void
          {
            throw std::logic_error( "texture_sub_image: unkown T_target" );
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
          
          template < typename gl_type::GLint T_internal_format = GL_DEPTH_COMPONENT24 >
          static inline auto texture_image_1d( gl_type::GLsizei width, const void* data = nullptr )
            -> void
          {
            texture_image_1d
            ( GL_TEXTURE_1D
            , 0
#ifdef EMSCRIPTEN
            , base_internal_format( T_internal_format )
#else
            , T_internal_format
#endif
            , width
            , 0
            , base_internal_format( T_internal_format )
            , type( T_internal_format )
            , data
            );
          }
          
          template < typename gl_type::GLint T_internal_format = GL_DEPTH_COMPONENT24 >
          static inline auto texture_image_1d()
            -> void
          { texture_image_2d< T_internal_format >( max_texture_size() ); }
          
          template < typename gl_type::GLint T_internal_format = GL_DEPTH_COMPONENT24 >
          static inline auto texture_image_2d( gl_type::GLsizei width, gl_type::GLsizei height, const void* data = nullptr )
            -> void
          {
            texture_image_2d
            ( GL_TEXTURE_2D
            , 0
#ifdef EMSCRIPTEN
            , base_internal_format( T_internal_format )
#else
            , T_internal_format
#endif
            , width, height
            , 0
            , base_internal_format( T_internal_format )
            , type( T_internal_format )
            , data
            );
          }
          
          template < typename gl_type::GLint T_internal_format = GL_DEPTH_COMPONENT24 >
          static inline auto texture_image_2d()
            -> void
          { texture_image_2d< T_internal_format >( max_texture_size() ); }

          template < typename gl_type::GLint T_internal_format = GL_DEPTH_COMPONENT24 >
          static inline auto texture_image_2d( gl_type::GLsizei size, const void* data = nullptr )
            -> void
          { texture_image_2d< T_internal_format >( size, size, data ); }
          
          template < typename gl_type::GLint T_internal_format = GL_DEPTH_COMPONENT24 >
          static inline auto texture_image_2d_multisample( gl_type::GLsizei width, gl_type::GLsizei height, const void* data, gl_type::GLsizei samples )
            -> void
          {
            texture_image_2d_multisample
            ( GL_TEXTURE_2D
            , samples
#ifdef EMSCRIPTEN
            , base_internal_format( T_internal_format )
#else
            , T_internal_format
#endif
            , width, height
            , 0
            , base_internal_format( T_internal_format )
            , type( T_internal_format )
            , data
            );
          }
          
          template < typename gl_type::GLint T_internal_format = GL_DEPTH_COMPONENT24 >
          static inline auto texture_image_2d_multisample( gl_type::GLsizei width, gl_type::GLsizei height, const void* data )
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
          
          template < typename gl_type::GLint T_internal_format = GL_DEPTH_COMPONENT24 >
          static inline auto texture_image_3d( gl_type::GLsizei width, gl_type::GLsizei height, gl_type::GLsizei depth, const void* data = nullptr )
            -> void
          {
            texture_image_3d
            ( GL_TEXTURE_3D
            , 0
#ifdef EMSCRIPTEN
            , base_internal_format( T_internal_format )
#else
            , T_internal_format
#endif
            , width, height, depth
            , 0
            , base_internal_format( T_internal_format )
            , type( T_internal_format )
            , data
            );
          }
          
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
          
          template < typename gl_type::GLint T_internal_format = GL_DEPTH_COMPONENT24 >
          static inline auto texture_image_3d_multisample( gl_type::GLsizei width, gl_type::GLsizei height, gl_type::GLsizei depth, const void* data, gl_type::GLsizei samples )
            -> void
          {
            texture_image_3d_multisample
            ( GL_TEXTURE_3D
            , samples
#ifdef EMSCRIPTEN
            , base_internal_format( T_internal_format )
#else
            , T_internal_format
#endif
            , width, height, depth
            , 0
            , base_internal_format( T_internal_format )
            , type( T_internal_format )
            , data
            );
          }
          
          template < typename gl_type::GLint T_internal_format = GL_DEPTH_COMPONENT24 >
          static inline auto texture_image_3d_multisample( gl_type::GLsizei width, gl_type::GLsizei height, gl_type::GLsizei depth, const void* data )
            -> void
          { texture_image_3d_multisample< T_internal_format >( width, height, depth, data, glm::log2<float>( std::min( std::min( width, height), depth ) ) ); }
          
          template < typename gl_type::GLint T_internal_format = GL_DEPTH_COMPONENT24 >
          static inline auto texture_image_3d_multisample( gl_type::GLsizei size, gl_type::GLsizei depth, const void* data )
            -> void
          { texture_image_3d_multisample< T_internal_format >( size, size, depth, data, glm::log2<float>( std::min( size, depth ) ) ); }
          
          template < typename gl_type::GLint T_internal_format = GL_DEPTH_COMPONENT24 >
          static inline auto texture_image_3d_multisample( gl_type::GLsizei size, const void* data )
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
          
          template < class T = void >
          static inline auto safe_delete_textures( std::vector< gl_type::GLuint >& texture_ids )
            -> void
          {
            for ( auto tid : texture_ids )
              safe_delete_texture( tid );
            
            texture_ids.clear();
          }
          
        };
        
        // template specialization
        
#ifdef GL_VERSION_4_2
        
        // storage series
        
        // 1D
        
        template < >
        inline auto texture_t::texture_storage < GL_TEXTURE_1D, glm::u32vec1 >
        ( gl_type::GLsizei levels
        , gl_type::GLenum internal_format
        , glm::u32vec1 size
        )
          -> void
        {
          c::glTexStorage1D
          ( GL_TEXTURE_1D
          , levels
          , internal_format
          , size[0]
          );
        }
        
        template < >
        inline auto texture_t::texture_storage < GL_TEXTURE_1D, const glm::u32vec1& >
        ( gl_type::GLenum internal_format
        , const glm::u32vec1& size
        )
          -> void
        { texture_storage< GL_TEXTURE_1D, const glm::u32vec1& >( gl_type::GLsizei( glm::log2< float >( size[0] ) ), internal_format, size ); }
        
        template < >
        inline auto texture_t::texture_storage < GL_TEXTURE_1D, gl_type::GLuint >
        ( gl_type::GLsizei levels
        , gl_type::GLenum internal_format
        , gl_type::GLuint size
        )
          -> void
        { texture_storage< GL_TEXTURE_1D, const glm::u32vec1& >( levels, internal_format, glm::u32vec1( size ) ); }
        
        template < >
        inline auto texture_t::texture_storage < GL_TEXTURE_1D, gl_type::GLuint >
        ( gl_type::GLenum internal_format
        , gl_type::GLuint size
        )
          -> void
        { texture_storage< GL_TEXTURE_1D, const glm::u32vec1& >( internal_format, glm::u32vec1( size ) ); }
        
        // 2D
        
        template < >
        inline auto texture_t::texture_storage < GL_TEXTURE_2D, const glm::u32vec2& >
        ( gl_type::GLsizei levels
        , gl_type::GLenum internal_format
        , const glm::u32vec2& size
        )
          -> void
        {
          c::glTexStorage2D
          ( GL_TEXTURE_2D
          , levels
          , internal_format
          , size[0], size[1]
          );
        }
        
        template < >
        inline auto texture_t::texture_storage < GL_TEXTURE_2D, const glm::u32vec2& >
        ( gl_type::GLenum internal_format
        , const glm::u32vec2& size
        )
          -> void
        { texture_storage< GL_TEXTURE_2D, const glm::u32vec2& >( gl_type::GLsizei( glm::log2< float >( std::min( size[0], size[1] ) ) ), internal_format, size ); }
        
        template < >
        inline auto texture_t::texture_storage < GL_TEXTURE_2D, gl_type::GLuint >
        ( gl_type::GLenum internal_format
        , gl_type::GLuint size
        )
          -> void
        { texture_storage< GL_TEXTURE_2D, const glm::u32vec2& >( internal_format, glm::u32vec2( size ) ); }
        
        template < >
        inline auto texture_t::texture_storage < GL_TEXTURE_2D, gl_type::GLuint >
        ( gl_type::GLsizei levels
        , gl_type::GLenum internal_format
        , gl_type::GLuint size
        )
          -> void
        { texture_storage< GL_TEXTURE_2D, const glm::u32vec2& >( levels, internal_format, glm::u32vec2( size ) ); }
        
        // 3D
        
        template < >
        inline auto texture_t::texture_storage < GL_TEXTURE_3D, const glm::u32vec3& >
        ( gl_type::GLsizei levels
        , gl_type::GLenum internal_format
        , const glm::u32vec3& size
        )
          -> void
        {
          c::glTexStorage3D
          ( GL_TEXTURE_3D
          , levels
          , internal_format
          , size[0], size[1], size[2]
          );
        }
        
        template < >
        inline auto texture_t::texture_storage < GL_TEXTURE_3D, gl_type::GLuint >
        ( gl_type::GLsizei levels
        , gl_type::GLenum internal_format
        , gl_type::GLuint size
        )
          -> void
        { texture_storage< GL_TEXTURE_3D, const glm::u32vec3& >( levels, internal_format, glm::u32vec3( size ) ); }
        
        template < >
        inline auto texture_t::texture_storage < GL_TEXTURE_3D, const glm::u32vec3& >
        ( gl_type::GLenum internal_format
        , const glm::u32vec3& size
        )
          -> void
        { texture_storage< GL_TEXTURE_3D, const glm::u32vec3& >( gl_type::GLsizei( glm::log2< float >( std::min( std::min( size[0], size[1] ), size[2] ) ) ), internal_format, size ); }
        
        template < >
        inline auto texture_t::texture_storage < GL_TEXTURE_3D, gl_type::GLuint >
        ( gl_type::GLenum internal_format
        , gl_type::GLuint size
        )
          -> void
        { texture_storage< GL_TEXTURE_3D, const glm::u32vec3& >( internal_format, glm::u32vec3( size ) ); }
        
#endif

#ifdef GL_VERSION_1_2
        
        // sub image series
        
        // 1D
        
        template < >
        inline auto texture_t::texture_sub_image < GL_TEXTURE_1D, gl_type::GLuint >
        ( gl_type::GLenum internal_format
        , gl_type::GLuint size
        , const void* data
        )
          -> void
        { texture_sub_image< GL_TEXTURE_1D, glm::vec1 >( internal_format, glm::vec1( size ), data ); }
        
        template < >
        inline auto texture_t::texture_sub_image < GL_TEXTURE_1D, const glm::u32vec1& >
        ( gl_type::GLenum internal_format
        , const glm::u32vec1& size
        , const void* data
        )
          -> void
        { c::glTexSubImage1D( GL_TEXTURE_1D, 0, 0, size[0], internal_format, type( internal_format ), data ); }
        
        // 2D
        
        template < >
        inline auto texture_t::texture_sub_image < GL_TEXTURE_2D, const glm::u32vec2& >
        ( gl_type::GLenum internal_format
        , const glm::u32vec2& size
        , const void* data
        )
          -> void
        { c::glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, size[0], size[1], base_internal_format( internal_format ), type( internal_format ), data ); }
        
        // 3D
        
        template < >
        inline auto texture_t::texture_sub_image < GL_TEXTURE_3D, const glm::u32vec3& >
        ( gl_type::GLenum internal_format
        , const glm::u32vec3& size
        , const void* data
        )
          -> void
        { c::glTexSubImage3D( GL_TEXTURE_3D, 0, 0, 0, 0, size[0], size[1], size[2], internal_format, type( internal_format ), data ); }
        
#endif
        
      }
    }
  }
}