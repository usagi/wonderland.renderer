#pragma once

#include <memory>
#include <list>
#include <type_traits>

#define GLM_SWIZZLE
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/vec1.hpp>

#include "wonderland.renderer.detail/glew.hxx"
#include "wonderland.renderer.detail/camera.hxx"
#include "wonderland.renderer.detail/destruct_invoker.hxx"
#include "wonderland.renderer.detail/shader.hxx"
#include "wonderland.renderer.detail/program.hxx"
#include "wonderland.renderer.detail/model.hxx"
#include "wonderland.renderer.detail/light.hxx"
#include "wonderland.renderer.detail/sampler.hxx"
#include "wonderland.renderer.detail/frame_buffer.hxx"
#include "wonderland.renderer.detail/texture.hxx"
#include "wonderland.renderer.detail/pnm.hxx"

#include "wonderland.renderer.detail/shader.detail/all.hxx"

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      class renderer_t
        : public glew::wrapper_t
      {
        camera_t::shared_t _camera;
        glm::mat4          _projection_transformation;
        
        renderer::program_t::const_shared_t     _default_program;
        std::list< std::shared_ptr< light_t > > _default_lights;
        
        // TODO: for debug
      public:
        bool _shadow_save;
      private:
        
        bool _shadow;
        
        static constexpr auto _shadow_mapping_texture_unit = 0;
        static constexpr auto _shadow_mapping_texture_internal_format
          = GL_DEPTH_COMPONENT32F;
          //= GL_DEPTH32F_STENCIL8;
        using shadow_mapping_texture_t = renderer::texture2d_t< _shadow_mapping_texture_internal_format >;
        
        renderer::program_t::const_shared_t         _shadow_mapping_program;
        std::shared_ptr< shadow_mapping_texture_t > _shadow_mapping_texture;
        std::shared_ptr< renderer::frame_buffer_t > _shadow_mapping_frame_buffer;
        std::shared_ptr< renderer::sampler_t >      _shadow_mapping_sampler;
        
        struct draw_params_t
        {
          renderer::program_t::const_shared_t program;
          model_t::shared_t                   model;
          const glm::mat4                     world_transformation;
          const model::animation_states_t     animation_states;
          
          draw_params_t
          ( renderer::program_t::const_shared_t program_
          , model_t::shared_t                   model_
          , const glm::mat4&                    world_transformation_
          , const model::animation_states_t&    animation_states_
          )
            : program             ( program_ )
            , model               ( model_ )
            , world_transformation( world_transformation_ )
            , animation_states    ( animation_states_ )
          { }
        };
        
        std::list< draw_params_t > _draw_queue;
        
        auto _create_shadow_mapping_program()
          -> void
        {
          ( _shadow_mapping_program = std::make_shared< renderer::program_t >() )
            -> attach( create_shader< vertex_shader_t   >( shader::shadow_mapping::vs_source() ) )
            -> attach( create_shader< fragment_shader_t >( shader::shadow_mapping::fs_source() ) )
            -> link()
            ;
        }
        
        auto _create_shadow_mapping_texture()
          -> void
        {
          // create texture for shadow mapping
          ( _shadow_mapping_texture = std::make_shared< shadow_mapping_texture_t >() )
            // TODO: for debug 256. to release, no params(system maximum size automatically).
            -> image_2d( 512 )
            ;
          
          // create sampler
          ( _shadow_mapping_sampler = std::make_shared< renderer::sampler_t>() )
            -> parameter_wrap_st( GL_CLAMP_TO_BORDER )
            -> parameter_min_mag_filter( GL_NEAREST )
            //-> parameter_compare_mode( GL_COMPARE_REF_TO_TEXTURE )
            //-> parameter_compare_func( GL_LESS )
            //-> parameter_mode( GL_LUMINANCE )
            -> parameter_border_color( glm::vec4( 1.0f ) )
            ;
          
          // bind samplar
          {
            auto p = _shadow_mapping_program -> scoped_use();
            active_texture< _shadow_mapping_texture_unit >();
            _shadow_mapping_sampler -> bind();
          }
        }
        
        auto _create_shadow_mapping_buffer()
          -> void
        {
          // create frame buffer for shadow mapping
          ( _shadow_mapping_frame_buffer = std::make_shared< renderer::frame_buffer_t >() )
            -> bind_texture( _shadow_mapping_texture )
            ;
        }
        
        auto _shadow_on()
          -> void
        {
          if ( not _shadow_mapping_program )
            try
            {
              _create_shadow_mapping_program();
              _create_shadow_mapping_texture();
              _create_shadow_mapping_buffer();
            }
            catch ( const std::exception& e )
            {
              std::cerr << "warn: cannot use shadow program( to disable shadow ): " << e.what() << "\n";
              _shadow_off();
              return;
            }
          
          _shadow = true;
        }
        
        auto _shadow_off()
          -> void
        { _shadow = false; }
        
        auto _invoke_draw()
          -> void
        {
          if ( _shadow )
            _invoke_draw_shadow();
          
          _invoke_draw_normal();
          
          _draw_queue.clear();
        }
        
        auto _invoke_draw_shadow()
          -> void 
        {
          std::vector< destruct_invoker_t > scopers;
          scopers.reserve( 10 );
          
          scopers.emplace_back( scoped_viewport( _shadow_mapping_texture -> viewport() ) );
          scopers.emplace_back( _shadow_mapping_program -> scoped_use() );
          scopers.emplace_back( _shadow_mapping_frame_buffer -> scoped_bind() );
          _shadow_mapping_frame_buffer -> bind_texture( _shadow_mapping_texture );
          
          // TODO: glDrawBuffer and glReadBuffer is not supported on Emscripten(GLES2)!
          scopers.emplace_back( scoped_draw_buffer() );
          scopers.emplace_back( scoped_read_buffer() );
          
          WRP_GLEW_CHECK_FRAME_BUFFER_STATUS
          
          scopers.emplace_back( scoped_cull_face() );
          scopers.emplace_back( scoped_color_mask() );
          
          scopers.emplace_back( scoped_enable< GL_POLYGON_OFFSET_FILL >() );
          scopers.emplace_back( scoped_polygon_offset( 1.1f, 4.0f ) );
          
          scopers.emplace_back( scoped_enable< GL_DEPTH_TEST >() );
          
          scopers.emplace_back( scoped_clear_color( 1.0f ) );
          clear
          ( GL_DEPTH_BUFFER_BIT
          & GL_COLOR_BUFFER_BIT
          );
          
          for ( const auto& draw_params : _draw_queue )
            _draw_shadow( draw_params );
          
          // TODO: for debug
          {
            std::vector< float > data( _shadow_mapping_texture -> count_of_data_elements() );
            
            active_texture< _shadow_mapping_texture_unit >();
            auto t = _shadow_mapping_texture -> scoped_bind();
            
            auto v = _shadow_mapping_texture -> viewport();
            glew::c::glReadPixels( v[0], v[1], v[2], v[3], GL_DEPTH_COMPONENT, GL_FLOAT, &data[0] );
            WRP_GLEW_TEST_ERROR
            
            std::cerr
              << "shadow texture data sum(size=" << std::distance( data.cbegin(), data.cend() ) << "): "
              << std::accumulate( data.cbegin(), data.cend(), 0.0 )
              << "\n"
              ;
            
            if ( _shadow_save )
            {
              pnm_t::save( data, v[2] );
              _shadow_save = false;
            }
          }
        }
        
        auto _invoke_draw_normal()
          -> void
        {
          auto scoped_programe_use = _default_program -> scoped_use();
          
          //enable< GL_TEXTURE_2D >();
          
          constexpr auto shadow_mapping_texture_unit = 0;
          
          active_texture< shadow_mapping_texture_unit >();
          auto scoped_texture_bind = _shadow_mapping_texture -> scoped_bind();
          
          _shadow_mapping_sampler
            -> bind< decltype(_shadow_mapping_texture)::element_type::target >
              ( shadow_mapping_texture_unit )
            ;
          
          activate_lights();
          
          clear();
          
          for ( const auto& draw_params : _draw_queue )
            _draw_normal( draw_params );
        }
        
        auto _draw_shadow( const draw_params_t& draw_params )
          -> void
        {
          const auto program_id = _shadow_mapping_program -> program_id();
          
          for ( const auto& light : _default_lights )
          {
            const auto point_light = std::dynamic_pointer_cast< point_light_t >( light );
            
            if ( point_light == nullptr )
              continue;
            
            const auto shadow_camera = std::make_shared< camera_t >
              ( point_light -> position
              , _camera -> target()
              , _camera -> up()
              );
            
            const auto fov_y = glm::pi<float>() / 4.0f;
            const auto screen_aspect_ratio = 1.0f;//960.f / 540.f ;
            const auto near_clip = 1.0e+0f;//1.0e-1f;
            const auto far_clip  = 1.0e+2f;//1.0e+3f;
            
            const auto wvp
              = glm::perspective( fov_y, screen_aspect_ratio, near_clip, far_clip )
              //= _projection_transformation
              // TODO: for debug
              * _camera -> view_transformation()
              //* shadow_camera -> view_transformation()
              * draw_params.world_transformation
              ;
            
            uniform( program_id, "world_view_projection_transformation", wvp );
            
            draw_params.model -> draw( draw_params.animation_states );
            
            // TODO: support multiple lights
            break;
          }
        }
        
        auto _draw_normal( const draw_params_t& draw_params )
          -> void
        {
          auto scoped_programe_use
            = ( _default_program not_eq draw_params.program )
              ? draw_params.program -> scoped_use()
              : destruct_invoker_t([]{})
              ;
          
          const auto wv
            = _camera -> view_transformation()
            * draw_params.world_transformation
            ;
          
          const auto wvp = _projection_transformation * wv;
          
          // TODO: support multiple lights for shadow mapping
          const auto shadow_camera = std::make_shared< camera_t >
            ( std::dynamic_pointer_cast< point_light_t >( *_default_lights.cbegin() ) -> position
            , _camera -> target()
            , _camera -> up()
            );
          
          const auto program_id = current_program();
          
          uniform( program_id, "world_view_projection_transformation", wvp );
          uniform( program_id, "world_view_transformation", wv );
          uniform( program_id, "world_transformation", draw_params.world_transformation );
          uniform( program_id, "view_direction", _camera -> view_direction() );
          uniform( program_id, "shadow_transformation", shadow_camera -> view_transformation() );
          
          draw_params.model -> draw( draw_params.animation_states );
        }
        
      public:
        
        explicit renderer_t()
          : _camera( std::make_shared< camera_t >() )
          , _projection_transformation( glm::mat4() )
        {
          glew::glew_init();
          
          // Wonderland.Renderer default enable GL features.
          enable< GL_BLEND >();
          enable< GL_DEPTH_TEST >();
          enable< GL_CULL_FACE >();
          
          if ( multisample_capability() )
            multisample();
          
          // Wonderland.Renderer try enble shadow feature.
          shadow();
        }
        
        template < class T_type, class ... T_prams >
        auto create_light( T_prams ... params )
          -> std::shared_ptr< T_type >
        {
          static_assert( std::is_base_of<light_t, T_type>(), "create_light need class of a based on light_t." );
          auto light = std::make_shared< T_type >( params ... );
          add_default_light( light );
          return light;
        }
        
        template < class T_shader , class T_embedded_shader_type_tag = shader::constant >
        inline auto create_shader_from_embedded()
          -> std::shared_ptr< const T_shader >
        { throw std::logic_error( "create_shader_from_embedded: invalid template parameter." ); }
        
        template < class T_embedded_shader_type_tag = shader::constant >
        inline auto create_program_from_embedded()
          -> std::shared_ptr< const renderer::program_t >
        { throw std::logic_error( "create_program_from_embedded: invalid template parameter." ); }
        
        inline auto view_projection_transformation()
          -> glm::mat4
        { return _projection_transformation * _camera -> view_transformation(); }
        
        inline auto projection_transformation()
          -> const glm::mat4&
        { return _projection_transformation; }
        
        inline auto projection_transformation( const glm::mat4& v)
          -> void
        { _projection_transformation = v; }
        
        inline auto camera()
          -> std::shared_ptr< camera_t >
        { return _camera; }
        
        inline auto view_transformation()
          -> glm::mat4
        { return _camera -> view_transformation(); }
        
        template<class T_shader>
        inline auto create_shader() const
          -> std::shared_ptr< T_shader >
        { return std::make_shared< T_shader >(); }
        
        template<class T_shader>
        inline auto create_shader( const std::string& source ) const
          -> std::shared_ptr< T_shader >
        { return std::make_shared< T_shader >() -> compile( source ); }
        
        template<class T_shader>
        inline auto create_shader( std::string&& source ) const
          -> std::shared_ptr< const T_shader >
        {
          return std::make_shared< T_shader >()
            -> compile( std::forward< std::string >( source ) )
            ;
        }
        
        template<class T_shader>
        inline auto create_shader( std::istream&& source ) const
          -> std::shared_ptr< const T_shader >
        {
          return std::make_shared< T_shader >()
            -> compile( std::forward< std::istream >( source ) )
            ;
        }
        
        inline auto create_program()
          -> std::shared_ptr< renderer::program_t >
        { return std::make_shared< renderer::program_t >(); }
        
        template< class ... T_shaders >
        inline auto create_program( T_shaders&& ... shaders )
          -> std::shared_ptr< const renderer::program_t >
        {
          return ( _default_program = create_program() )
            -> attach( shaders ... )
            -> link()
            ;
        }
        
        template < class T >
        inline auto create_model( T t )
          -> std::shared_ptr< model_t >
        { return model_t::create(t); }
        
        inline auto enable_vertex_attribute( glew::gl_type::GLuint v )
          -> destruct_invoker_t
        {
          std::vector<glew::gl_type::GLuint> list;
          list.emplace_back( v );
          return enable_vertex_attributes( std::move( list ) );
        }
        
        inline auto enable_vertex_attributes( std::vector<glew::gl_type::GLuint> && list )
          -> destruct_invoker_t
        {
          for ( const auto & n : list )
            glew::c::glEnableVertexAttribArray( n );
          
          return
          {
            [list]()
            {
              std::for_each
              ( list.crbegin(), list.crend()
              , []( glew::gl_type::GLuint n )
                {
                  glew::c::glDisableVertexAttribArray( n );
                }
              );
            }
          };
        }
        
        auto draw
        ( std::shared_ptr< renderer::program_t > program
        , std::shared_ptr< model_t >             model
        , const glm::mat4&                       world_transformation = glm::mat4( 1.0f )
        , const model::animation_states_t&       animation_states     = { }
        )
          -> void
        { _draw_queue.emplace_back( program, model, world_transformation, animation_states ); }
        
        auto draw
        ( std::shared_ptr< model_t >       model
        , const glm::mat4&                 world_transformation = glm::mat4( 1.0f )
        , const model::animation_states_t& animation_states     = { }
        )
          -> void
        { _draw_queue.emplace_back( _default_program, model, world_transformation, animation_states ); }
        
        template < class ... Ts >
        auto default_lights( const Ts& ... ls )
          -> void
        {
          _default_lights.clear();
          add_default_lights( ls ... );
        }
        
        template < class T, class ... Ts >
        auto add_default_lights( const T& light, const Ts& ... ls )
          -> void
        {
          add_default_light( light );
          add_default_lights( ls ... );
        }
        
        template < class T>
        auto add_default_light( const T& light )
          -> void
        { _default_lights.push_back( light ); }
        
        template < class T, class ... Ts >
        auto remove_default_lights( const T& light, const Ts& ... ls )
          -> void
        {
          remove_default_light( light );
          remove_default_lights( ls ... );
        }
        
        template < class T>
        auto remove_default_light( const T& light )
          -> void
        { _default_lights.remove( light ); }
        
        auto activate_lights() const
          -> void
        {
          for ( const auto& light : _default_lights )
            light -> activate();
        }
        
        auto default_program( renderer::program_t::const_shared_t p )
          -> void
        { _default_program = p; }
        
        auto default_program() const
          -> renderer::program_t::const_shared_t
        {
          if( _default_program )
            return _default_program;
          
          throw std::logic_error( "default program is not set yet." );
        }
        
        auto invoker()
          -> std::array< destruct_invoker_t, 2 >
        {
          return
          { { this -> flusher()
            , destruct_invoker_t( [ this ]{ _invoke_draw(); } )
            }
          };
        }
        
        inline auto flusher() const
          -> destruct_invoker_t
        { return { [ ]{ glew::wrapper_t::flush(); } }; }
        
        auto shadow( bool enable = true)
          -> void
        { enable ? _shadow_on() : _shadow_off(); }
      };
      
      // specialize to constant
      template<>
      auto renderer_t::create_shader_from_embedded< vertex_shader_t, shader::constant >()
        -> std::shared_ptr< const vertex_shader_t >
      { return create_shader< vertex_shader_t >( shader::constant::vs_source() ); }
      
      template<>
      auto renderer_t::create_shader_from_embedded< fragment_shader_t, shader::constant >()
        -> std::shared_ptr< const fragment_shader_t >
      { return create_shader< fragment_shader_t >( shader::constant::fs_source() ); }
      
      template<>
      auto renderer_t::create_program_from_embedded< shader::constant >()
        -> std::shared_ptr< const renderer::program_t >
      { return create_program( create_shader_from_embedded< vertex_shader_t, shader::constant >(), create_shader_from_embedded< fragment_shader_t, shader::constant >() ); }
      
      // specialize to phong
      template<>
      auto renderer_t::create_shader_from_embedded< vertex_shader_t, shader::phong >()
        -> std::shared_ptr< const vertex_shader_t >
      { return create_shader< vertex_shader_t >( shader::phong::vs_source() ); }
      
      template<>
      auto renderer_t::create_shader_from_embedded< fragment_shader_t, shader::phong >()
        -> std::shared_ptr< const fragment_shader_t >
      { return create_shader< fragment_shader_t >( shader::phong::fs_source() ); }
      
      template<>
      auto renderer_t::create_program_from_embedded< shader::phong >()
        -> std::shared_ptr< const renderer::program_t >
      { return create_program( create_shader_from_embedded< vertex_shader_t, shader::phong >(), create_shader_from_embedded< fragment_shader_t, shader::phong >() ); }
      
      // specialize to cartoon
      template<>
      auto renderer_t::create_shader_from_embedded< vertex_shader_t, shader::cartoon >()
        -> std::shared_ptr< const vertex_shader_t >
      { return create_shader< vertex_shader_t >( shader::cartoon::vs_source() ); }
      
      template<>
      auto renderer_t::create_shader_from_embedded< fragment_shader_t, shader::cartoon >()
        -> std::shared_ptr< const fragment_shader_t >
      { return create_shader< fragment_shader_t >( shader::cartoon::fs_source() ); }
      
      template<>
      auto renderer_t::create_program_from_embedded< shader::cartoon >()
        -> std::shared_ptr< const renderer::program_t >
      { return create_program( create_shader_from_embedded< vertex_shader_t, shader::cartoon >(), create_shader_from_embedded< fragment_shader_t, shader::cartoon >() ); }
      
      // specialize to shadow_mapping
      template<>
      auto renderer_t::create_shader_from_embedded< vertex_shader_t, shader::shadow_mapping >()
        -> std::shared_ptr< const vertex_shader_t >
      { return create_shader< vertex_shader_t >( shader::shadow_mapping::vs_source() ); }
      
      template<>
      auto renderer_t::create_shader_from_embedded< fragment_shader_t, shader::shadow_mapping >()
        -> std::shared_ptr< const fragment_shader_t >
      { return create_shader< fragment_shader_t >( shader::shadow_mapping::fs_source() ); }
      
      template<>
      auto renderer_t::create_program_from_embedded< shader::shadow_mapping >()
        -> std::shared_ptr< const renderer::program_t >
      { return create_program( create_shader_from_embedded< vertex_shader_t, shader::shadow_mapping >(), create_shader_from_embedded< fragment_shader_t, shader::shadow_mapping >() ); }
      
    }
  }
}