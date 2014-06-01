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
      
      // common part: base
      class texture_base_t
        : protected glew::texture_t
      {
      protected:
        std::vector< glew::gl_type::GLuint > _texture_ids;
        glm::i32vec4 _viewport;
        
        template < typename glew::gl_type::GLenum T_target >
        auto _bind( const unsigned n = 0 )
          -> void
        { bind_texture< T_target >( _texture_ids[ n ] ); }
        
        template < typename glew::gl_type::GLenum T_target >
        auto _unbind()
          -> void
        { bind_texture< T_target >(); }
        
        template < typename glew::gl_type::GLenum T_target >
        auto _scoped_bind( const unsigned n = 0 )
          -> destruct_invoker_t
        { return scoped_bind_texture< T_target >( _texture_ids[ n ] ); }
        
        template < typename glew::gl_type::GLsizei T_count_of_textures = 1 >
        auto _generate_textures_impl()
          -> void
        { _texture_ids = generate_textures< T_count_of_textures >(); }
        
        virtual auto _generate_textures()
          -> void
        { _generate_textures_impl(); }
        
      public:
        texture_base_t()
        { this -> _generate_textures(); }
        
        virtual ~texture_base_t()
        { safe_delete_textures( _texture_ids ); }
        
        virtual auto create() -> void = 0;
        
        virtual auto bind()   -> void = 0;
        
        virtual auto unbind() -> void = 0;
        
        virtual auto scoped_bind()
          -> destruct_invoker_t
        = 0;
        
        virtual auto texture_id( const unsigned n = 0 )
          -> decltype( _texture_ids )::value_type
        { return _texture_ids[ n ]; }
        
        virtual auto texture_ids()
          -> const decltype( _texture_ids )&
        { return _texture_ids; }
        
        virtual auto viewport() -> const glm::i32vec4&
        { return _viewport; }
        
        auto viewport( const glm::i32vec4& viewport_ )
          -> void
        { _viewport = viewport_; }
        
        virtual auto count_of_data_elements()
          -> const std::size_t
        { return _viewport[2] * _viewport[3] * _texture_ids.size(); }
        
      };
      
      // common part: target
      template < typename glew::gl_type::GLenum T_target = GL_TEXTURE_2D >
      struct texture_base_target_t
      { static constexpr glew::gl_type::GLenum target = T_target; };
      
      // common part: format
      template < typename glew::gl_type::GLenum T_internal_format = GL_RGBA8 >
      struct texture_base_format_t
      {
        static constexpr glew::gl_type::GLenum internal_format
          = T_internal_format;
        static constexpr glew::gl_type::GLenum base_internal_format
          = glew::texture_t::base_internal_format( T_internal_format );
        static constexpr glew::gl_type::GLenum type
          = glew::texture_t::type( T_internal_format );
      };
      
      template
      < typename glew::gl_type::GLenum T_target
      , typename glew::gl_type::GLenum T_internal_format
      >
      class texture_t
        : public std::enable_shared_from_this< texture_t< T_target, T_internal_format > >
        , public texture_base_t
        , public texture_base_target_t< T_target >
        , public texture_base_format_t< T_internal_format >
      {
      public:
        
        auto bind()
          -> void override
        { _bind< T_target >(); }
        
        auto unbind()
          -> void override
        { _unbind< T_target >(); }
        
        auto scoped_bind()
          -> destruct_invoker_t override
        { return _scoped_bind< T_target >(); }
      };
      
      template < typename glew::gl_type::GLenum T_internal_format >
      class texture_1d_t
        : public texture_t< GL_TEXTURE_1D, T_internal_format >
      {
      public:
        static constexpr auto internal_format = T_internal_format;
        
        auto create()
          -> void override
        { create( glew::texture_t::max_texture_size() ); }
        
        auto create( glew::gl_type::GLsizei width, const void* data = nullptr )
          -> void
        {
          const auto max_size = glew::texture_t::max_texture_size();
          
          if ( width > max_size )
          {
            width = max_size;
            std::cerr << "warn: texture_t::create: width( " << width << " ) > max_size( " << max_size << " ), fit to the max_size.\n";
          }
          
          auto bind = this -> scoped_bind();
          
#if defined( GL_VERSION_4_2 )
          const auto size = glm::u32vec1( width );
          glew::texture_t::texture_storage< GL_TEXTURE_1D >( internal_format, size );
          WRP_GLEW_TEST_ERROR
          if ( data )
          {
            glew::texture_t::texture_sub_image< GL_TEXTURE_1D >( internal_format, size, data );
            WRP_GLEW_TEST_ERROR
          }
#elif defined( GL_VERSION_3_0 )
          glew::texture_t::texture_image_1d< T_internal_format >( width, data );
          WRP_GLEW_TEST_ERROR
#endif
          
          this -> _viewport[2] = width;
          this -> _viewport[3] = 1;
        }
        
      };
      
      template < typename glew::gl_type::GLenum T_internal_format >
      class texture_2d_t
        : public texture_t< GL_TEXTURE_2D, T_internal_format >
      {
      public:
        static constexpr auto internal_format = T_internal_format;
        
        auto create()
          -> void override
        {
          const auto max_size = glew::texture_t::max_texture_size();
          create( max_size, max_size );
        }
        
        auto create( glew::gl_type::GLsizei size, const void* data = nullptr )
          -> void
        { create( size, size, data ); }
        
        auto create( glew::gl_type::GLsizei width, glew::gl_type::GLsizei height, const void* data = nullptr )
          -> void
        {
          const auto max_size = glew::texture_t::max_texture_size();
          
          if ( width > max_size )
          {
            width = max_size;
            std::cerr << "warn: texture_t::create: width( " << width << " ) > max_size( " << max_size << " ), fit to the max_size.\n";
          }
          
          if ( height > max_size )
          {
            height = max_size;
            std::cerr << "warn: texture_t::create: height( " << height << " ) > max_size( " << max_size << " ), fit to the max_size.\n";
          }
          
          auto bind = this -> scoped_bind();
          
#if defined( GL_VERSION_4_2 )
          const auto size = glm::u32vec2( width, height );
          glew::texture_t::texture_storage< GL_TEXTURE_2D, const glm::u32vec2& >( internal_format, size );
          WRP_GLEW_TEST_ERROR
          if ( data )
          {
            glew::texture_t::texture_sub_image< GL_TEXTURE_2D >( internal_format, size, data );
            WRP_GLEW_TEST_ERROR
          }
#elif defined( GL_VERSION_3_0 )
          glew::texture_t::texture_image_2d< T_internal_format >( width, height, data );
#endif
          
          this -> _viewport[2] = width;
          this -> _viewport[3] = height;
        }
        
      };
      
      template < typename glew::gl_type::GLenum T_internal_format >
      class texture_3d_t
        : public texture_t< GL_TEXTURE_3D, T_internal_format >
      {
      public:
        static constexpr auto internal_format = T_internal_format;
        
        auto create()
          -> void override
        {
          const auto max_size = glew::texture_t::max_texture_size();
          create( max_size, max_size, max_size );
        }
        
        auto create( glew::gl_type::GLsizei size, const void* data = nullptr )
          -> void
        { create( size, size, size, data ); }
        
        auto create( glew::gl_type::GLsizei width, glew::gl_type::GLsizei height, glew::gl_type::GLsizei depth, const void* data = nullptr )
          -> void
        {
          const auto max_size = glew::texture_t::max_texture_size();
          
          if ( width > max_size )
          {
            width = max_size;
            std::cerr << "warn: texture_t::create: width( " << width << " ) > max_size( " << max_size << " ), fit to the max_size.\n";
          }
          
          if ( height > max_size )
          {
            height = max_size;
            std::cerr << "warn: texture_t::create: height( " << height << " ) > max_size( " << max_size << " ), fit to the max_size.\n";
          }
          
          if ( depth > max_size )
          {
            depth = max_size;
            std::cerr << "warn: texture_t::create: depth( " << depth << " ) > max_size( " << max_size << " ), fit to the max_size.\n";
          }
          
          auto bind = this -> scoped_bind();
          
#if defined( GL_VERSION_4_2 )
          const auto size = glm::u32vec3( width, height, depth );
          glew::texture_t::texture_storage< GL_TEXTURE_3D >( internal_format, size );
          WRP_GLEW_TEST_ERROR
          if ( data )
          {
            glew::texture_t::texture_sub_image< GL_TEXTURE_3D >( internal_format, size, data );
            WRP_GLEW_TEST_ERROR
          }
#elif defined( GL_VERSION_3_0 )
          glew::texture_t::texture_image_3d< T_internal_format >( width, height, depth, data );
#endif
          
          this -> _viewport[2] = width;
          this -> _viewport[3] = height;
        }
        
      };
      /*
      // specialize to GL_TEXTURE_CUBE_MAP
      template
      < typename glew::gl_type::GLenum T_internal_format
      >
      class texture_t< GL_TEXTURE_CUBE_MAP, T_internal_format >
        : public texture_base_t< GL_TEXTURE_CUBE_MAP, T_internal_format >
      {
        //using base_t = texture_base_t< GL_TEXTURE_CUBE_MAP, T_internal_format >;
        
        std::vector<  >
        std::shared_ptr< texture_t< GL_TEXTURE_CUBE_MAP_NEGATIVE_X, T_internal_format > > _nx;
        std::shared_ptr< texture_t< GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, T_internal_format > > _ny;
        std::shared_ptr< texture_t< GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, T_internal_format > > _nz;
        std::shared_ptr< texture_t< GL_TEXTURE_CUBE_MAP_POSITIVE_X, T_internal_format > > _px;
        std::shared_ptr< texture_t< GL_TEXTURE_CUBE_MAP_POSITIVE_Y, T_internal_format > > _py;
        std::shared_ptr< texture_t< GL_TEXTURE_CUBE_MAP_POSITIVE_Z, T_internal_format > > _pz;
        
      public:
        
        auto create( glew::gl_type::GLsizei width, glew::gl_type::GLsizei height, const void* data = nullptr )
          -> void
        {
          const auto max_size = glew::texture_t::max_texture_size();
          
          if ( width > max_size )
          {
            width = max_size;
            std::cerr << "warn: texture_t::create: width( " << width << " ) > max_size( " << max_size << " ), fit to the max_size.\n";
          }
          
          if ( height > max_size )
          {
            height = max_size;
            std::cerr << "warn: texture_t::create: height( " << height << " ) > max_size( " << max_size << " ), fit to the max_size.\n";
          }
#if defined( GL_VERSION_4_2 )
          storage_2d( width, height, data );
#elif defined( GL_VERSION_3_0 )
          image_2d( width, height, data );
#endif
        }
        
        auto create( glew::gl_type::GLsizei size, const void* data = nullptr )
          -> void
        { create( size, size, data ); }
        
        auto create()
          -> void
        { create( glew::texture_t::max_texture_size() ); }
        
      };
      */
    }
  }
}