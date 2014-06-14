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
#include "wonderland.renderer.detail/projection.hxx"
#include "wonderland.renderer.detail/camera.hxx"
#include "wonderland.renderer.detail/destruct_invoker.hxx"
#include "wonderland.renderer.detail/shader.hxx"
#include "wonderland.renderer.detail/program.hxx"
#include "wonderland.renderer.detail/model.hxx"
#include "wonderland.renderer.detail/light.hxx"
#include "wonderland.renderer.detail/sampler.hxx"
#include "wonderland.renderer.detail/frame_buffer.hxx"
#include "wonderland.renderer.detail/render_buffer.hxx"
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
        camera_t::shared_t     _camera;
        projection_t::shared_t _projection;
        
        renderer::program_t::const_shared_t     _default_program;
        std::list< std::shared_ptr< light_t > > _default_lights;
        
        // TODO: for debug
      public:
        bool _shadow_save;
      private:
        
        bool _shadow;
        
        static constexpr auto _shadow_mapping_texture_unit = 7;
        static constexpr auto _shadow_mapping_texture_internal_format
          = shader::glsl_version_ge( 300 )
            ? GL_DEPTH_COMPONENT32F
            : GL_DEPTH_COMPONENT32
            ;
        
        using shadow_mapping_texture_t
          = renderer::texture_cube_map_t< _shadow_mapping_texture_internal_format >;
        
        renderer::program_t::const_shared_t          _shadow_mapping_program;
        std::shared_ptr< shadow_mapping_texture_t >  _shadow_mapping_texture;
        std::shared_ptr< renderer::frame_buffer_t >  _shadow_mapping_frame_buffer;
        std::shared_ptr< renderer::render_buffer_t > _shadow_mapping_render_buffer;
        std::shared_ptr< renderer::sampler_t >       _shadow_mapping_sampler;
        projection_t::shared_t                       _shadow_projection;
        glm::mat4                                    _shadow_transformation;
        
        renderer::program_t::const_shared_t          _point_light_program;
        
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
        
        auto _initialize_shadow_mapping()
          -> void
        {
          // program
          ( _shadow_mapping_program = std::make_shared< renderer::program_t >() )
            -> attach( create_shader< vertex_shader_t   >( shader::shadow_mapping::vs_source() ) )
            -> attach( create_shader< fragment_shader_t >( shader::shadow_mapping::fs_source() ) )
            -> link()
            ;
          
          // fbo
          ( _shadow_mapping_frame_buffer = std::make_shared< renderer::frame_buffer_t >() )
            -> bind()
            ;
          
          // texture
          ( _shadow_mapping_texture = std::make_shared< shadow_mapping_texture_t >() )
            -> create( 2048 )
            ;
          
          // create sampler
          ( _shadow_mapping_sampler = std::make_shared< renderer::sampler_t>() )
            -> parameter_mag_filter( GL_LINEAR )
#ifndef EMSCRIPTEN
            -> parameter_min_filter( GL_LINEAR_MIPMAP_LINEAR )
            -> parameter_wrap_st( GL_CLAMP_TO_EDGE )
#else
            -> parameter_min_filter( GL_LINEAR )
            -> parameter_wrap_st( GL_CLAMP_TO_EDGE )
#endif
            ;
          
          _shadow_mapping_frame_buffer
            -> bind_texture( _shadow_mapping_texture, GL_TEXTURE_CUBE_MAP_POSITIVE_X )
            -> unbind()
            ;
          
          _shadow_mapping_texture
            -> unbind()
            ;
        }
        
        auto _shadow_on()
          -> void
        {
          if ( not _shadow_mapping_program )
            try
            {
              _initialize_shadow_mapping();
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
          {
            std::vector< destruct_invoker_t > scopers;
            scopers.reserve( 10 );
            
            scopers.emplace_back( scoped_viewport( _shadow_mapping_texture -> viewport() ) );
            scopers.emplace_back( _shadow_mapping_program -> scoped_use() );
            scopers.emplace_back( _shadow_mapping_frame_buffer -> scoped_bind() );
            scopers.emplace_back( scoped_enable< GL_CULL_FACE >() );
            scopers.emplace_back( scoped_cull_face() );
            scopers.emplace_back( scoped_color_mask() );
            scopers.emplace_back( scoped_enable< GL_POLYGON_OFFSET_FILL >() );
            scopers.emplace_back( scoped_polygon_offset( 1.1f, 4.0f ) );
            scopers.emplace_back( scoped_enable< GL_DEPTH_TEST >() );
            scopers.emplace_back( scoped_clear_color( 1.0f ) );
            
            _uniform_log_z_trick( _shadow_mapping_program -> program_id() );
            
            constexpr auto count_of_cube_map_textures = 6u;
            for ( auto target_surface_number = 0u; target_surface_number < count_of_cube_map_textures; ++target_surface_number )
            {
#ifndef EMSCRIPTEN
              auto bind_frame_buffer_texture = _shadow_mapping_frame_buffer
                -> scoped_bind_texture( _shadow_mapping_texture, GL_TEXTURE_CUBE_MAP_POSITIVE_X + target_surface_number )
                ;
#else
              _shadow_mapping_frame_buffer
                -> bind_texture( _shadow_mapping_texture, GL_TEXTURE_CUBE_MAP_POSITIVE_X + target_surface_number )
                ;
#endif
              
              clear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
              
              for ( const auto& draw_params : _draw_queue )
                _draw_shadow( draw_params, target_surface_number );
            }
          }
          

#ifndef EMSCRIPTEN
          {
            auto bind = _shadow_mapping_texture -> scoped_bind();
            _shadow_mapping_texture -> generate_mipmap();
          }
#else
          // Chromium cannot support mipmap with depth or stencil internal format type.
          // maybe: https://github.com/mirrors/chromium/blob/1f38aaea945f6bea1dc6d57f8fe5ff8cf92fb3bf/gpu/command_buffer/service/texture_manager.cc#L348
            
          // Not Chromium or Chrome
          if( emscripten_run_script_int( "navigator.userAgent.indexOf('Chrome') === -1" ) )
          {
            auto bind = _shadow_mapping_texture -> scoped_bind();
            _shadow_mapping_texture -> generate_mipmap();
          }
#endif

          
          // TODO: for debug
#ifndef EMSCRIPTEN
          //*
          if ( _shadow_save )
          {
            
            auto f = _shadow_mapping_frame_buffer -> scoped_bind();
            WRP_GLEW_TEST_ERROR
            
            constexpr auto count_of_cube_map_textures = 6;
            for ( auto n = 0u; n < count_of_cube_map_textures; ++n )
            {
              std::vector< float > data( _shadow_mapping_texture -> count_of_data_elements() );
              
              active_texture< _shadow_mapping_texture_unit >();
              auto t = _shadow_mapping_texture -> scoped_bind();
              //glew::c::glGenerateMipmap( GL_TEXTURE_CUBE_MAP );
              
              auto v = _shadow_mapping_texture -> viewport();
              glew::c::glGetTexImage( GL_TEXTURE_CUBE_MAP_POSITIVE_X + n, 0, GL_DEPTH_COMPONENT, GL_FLOAT, &data[0] );
              //glew::c::glReadPixels( v[0], v[1], v[2], v[3], GL_DEPTH_COMPONENT, GL_FLOAT, &data[0] );
              WRP_GLEW_TEST_ERROR
              
              pnm_t::save( data, v[2], "out_" + std::to_string( n ) + ".pgm" );
            }
            
            _shadow_save = false;
            
          }
          //*/
#endif
        }
        
        auto _invoke_draw_normal()
          -> void
        {
          constexpr auto shadow_mapping_texture_unit = _shadow_mapping_texture_unit;
          
          auto scoped_programe_use = _default_program -> scoped_use();
          
          active_texture< _shadow_mapping_texture_unit >();
          
          if ( _shadow )
          {
            _shadow_mapping_texture -> bind();
            WRP_GLEW_TEST_ERROR
            
            uniform( _default_program -> program_id(), "shadow_sampler", shadow_mapping_texture_unit );
            _shadow_mapping_sampler
              -> bind
                < decltype( _shadow_mapping_texture )::element_type::target >
              ( shadow_mapping_texture_unit )
              ;
          }
          else
            bind_texture< decltype( _shadow_mapping_texture )::element_type::target >();
          
          activate_lights();
          
          clear();
          
          for ( const auto& draw_params : _draw_queue )
            _draw_normal( draw_params );
          
          _draw_point_light();
        }
        
        auto _draw_point_light()
          -> void
        {
          // [ { (x,y,z), (r,g,b) }, .. ]
          std::vector< std::array< glm::vec3, 2 > > point_light_vertices;
          
          // pickup point lights
          for ( const auto& light : _default_lights )
          {
            const auto point_light = std::dynamic_pointer_cast< point_light_t >( light );
            
            if ( point_light )
              point_light_vertices.push_back
              ( { { point_light -> position
                  , point_light -> color
                  }
                }
              );
          }
          
          if ( point_light_vertices.empty() )
            return;
          
          constexpr glew::gl_type::GLenum    attribute = GL_FLOAT;
          constexpr glew::gl_type::GLboolean normalize_off = false;
          
          constexpr glew::gl_type::GLenum mode  = GL_POINTS;
          constexpr glew::gl_type::GLint  first = 0;
          
          auto p = _point_light_program -> scoped_use();
          
          const auto program_id = _point_light_program -> program_id();
          
#ifndef EMSCRIPTEN
          auto e_vps = scoped_enable< GL_VERTEX_PROGRAM_POINT_SIZE >();
          auto e_ps  = scoped_enable< GL_POINT_SPRITE >();
#endif
          
          auto e_b   = scoped_enable< GL_BLEND >();
          glew::c::glBlendFunc( GL_SRC_ALPHA, GL_ONE );
          WRP_GLEW_TEST_ERROR
          
          uniform( program_id, "view_projection_transformation", view_projection_transformation() );
          WRP_GLEW_TEST_ERROR
          const auto view_port_ = viewport();
          WRP_GLEW_TEST_ERROR
          uniform( program_id, "point_size", float(view_port_[3]) * 0.5f );
          WRP_GLEW_TEST_ERROR
          _uniform_log_z_trick( program_id );
          WRP_GLEW_TEST_ERROR
          
          // TODO: change to GL_DYNAMIC_DRAW
          WRP_GLEW_TEST_ERROR
          glew::gl_type::GLuint point_light_vertices_buffer = 0;
          
          glew::c::glGenBuffers( 1, &point_light_vertices_buffer );
          glew::c::glBindBuffer( GL_ARRAY_BUFFER, point_light_vertices_buffer );
          glew::c::glBufferData( GL_ARRAY_BUFFER, point_light_vertices.size() * sizeof( decltype( point_light_vertices )::value_type ), point_light_vertices.data(), GL_STREAM_DRAW );
          
          const auto location_of_vs_position = glew::c::glGetAttribLocation( program_id, "position" );
          const auto location_of_vs_color    = glew::c::glGetAttribLocation( program_id, "color"    );
          
          if ( location_of_vs_position not_eq -1 )
          {
            glew::c::glVertexAttribPointer( location_of_vs_position, 3, attribute, normalize_off, sizeof( float ) * 6, reinterpret_cast< void* >( 0 ) );
            glew::c::glEnableVertexAttribArray( location_of_vs_position );
          }
          
          if ( location_of_vs_color not_eq -1 )
          { 
            glew::c::glVertexAttribPointer( location_of_vs_color, 3, attribute, normalize_off, sizeof( float ) * 6, reinterpret_cast< void* >( sizeof( float ) * 3 ) );
            glew::c::glEnableVertexAttribArray( location_of_vs_color );
          }
          
          glew::c::glDrawArrays( mode, first, point_light_vertices.size() );
          
          glew::c::glBindBuffer( GL_ARRAY_BUFFER, 0 );
          glew::c::glDeleteBuffers( 1, &point_light_vertices_buffer );
          glew::c::glBlendFunc(GL_ONE, GL_ZERO);
        }
        
        auto _draw_shadow( const draw_params_t& draw_params, unsigned target_surface_number )
          -> void
        {
          const auto program_id = _shadow_mapping_program -> program_id();
          
          for ( const auto& light : _default_lights )
          {
            const auto point_light = std::dynamic_pointer_cast< point_light_t >( light );
            
            if ( point_light == nullptr )
              continue;
            
            auto shadow_camera_target = point_light -> position;
            auto shadow_camera_up     = glm::vec3( 0.0f, 1.0f, 0.0f );
            
            switch ( target_surface_number )
            { case 0: shadow_camera_target += glm::vec3( -1.0f,  0.0f,  0.0f ); break;
              case 1: shadow_camera_target += glm::vec3(  1.0f,  0.0f,  0.0f ); break;
              case 2: shadow_camera_target += glm::vec3(  0.0f, -1.0f,  0.0f ); shadow_camera_up = glm::vec3( 0.0f, 0.0f,  1.0f ); break;
              case 3: shadow_camera_target += glm::vec3(  0.0f,  1.0f,  0.0f ); shadow_camera_up = glm::vec3( 0.0f, 0.0f, -1.0f ); break;
              case 4: shadow_camera_target += glm::vec3(  0.0f,  0.0f,  1.0f ); break;
              case 5: shadow_camera_target += glm::vec3(  0.0f,  0.0f, -1.0f ); break;
            }
            
            const auto shadow_camera = std::make_shared< camera_t >
              ( point_light -> position
              , shadow_camera_target
              , shadow_camera_up
              );
            
            _shadow_projection
              -> fov_y( glm::pi<float>() / 2.f )
              -> aspect_ratio( 1.0f )
              -> near_clip( _projection -> near_clip() )
              -> far_clip ( _projection -> far_clip() )
              -> update()
              ;
            
            // TODO: for parallel light shadow
            //_shadow_transformation = glm::ortho( -10.f, 10.f, -10.f, 10.f, -10.f, 10.f );
            /*
            uniform
            ( program_id
            , "z_log_trick_far"
            , 10.f //_projection -> far_clip()
            );
            */
            
            const auto wvp
              = _shadow_projection -> projection_transformation()
              * shadow_camera -> view_transformation()
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
            = view_transformation()
            * draw_params.world_transformation
            ;
          
          const auto wvp = projection_transformation() * wv;
          
          // TODO: support multiple lights for shadow mapping
          const auto shadow_camera_position = std::dynamic_pointer_cast< point_light_t >( *_default_lights.cbegin() ) -> position;
          const auto shadow_camera = std::make_shared< camera_t >
            ( shadow_camera_position
            , shadow_camera_position + glm::vec3( 0.0f, 0.0f, -1.0f )
            , glm::vec3( 0.0f, 1.0f, 0.0f )
            );
          
          const auto program_id = current_program();
          
          uniform( program_id, "world_view_projection_transformation", wvp );
          uniform( program_id, "world_view_transformation", wv );
          uniform( program_id, "world_transformation", draw_params.world_transformation );
          uniform( program_id, "view_direction", _camera -> view_direction() );
          
          if ( _shadow )
          {
            const auto shadow_transformation
              = glm::translate( glm::mat4(), glm::vec3( 0.5f ) )
              * glm::scale    ( glm::mat4(), glm::vec3( 0.5f ) )
              * _shadow_projection -> projection_transformation()
              * shadow_camera -> view_transformation()
              * glm::inverse( view_transformation() )
              ;
            
            uniform( program_id, "shadow_transformation", shadow_transformation );
          }
          
          _uniform_log_z_trick( program_id );
          
          draw_params.model -> draw( draw_params.animation_states );
        }
        
        auto _initialize_point_light()
          -> void
        {
          ( _point_light_program = std::make_shared< renderer::program_t >() )
            -> attach( create_shader< vertex_shader_t   >( shader::point_light::vs_source() ) )
            -> attach( create_shader< fragment_shader_t >( shader::point_light::fs_source() ) )
            -> link()
            ;
        }
        
        auto _uniform_log_z_trick() const
          -> void
        { _uniform_log_z_trick( current_program() ); }
        
        auto _uniform_log_z_trick( const glew::gl_type::GLuint program_id ) const
          -> void
        {
          uniform
          ( program_id
          , "z_log_trick_far"
          , _projection -> far_clip()
          );
        }
        
      public:
        
        explicit renderer_t()
          : _camera( std::make_shared< camera_t >() )
          , _projection( std::make_shared< projection_t >() )
          , _shadow_projection( std::make_shared< projection_t >() )
        {
          glew::glew_init();
          
          // Wonderland.Renderer default enable GL features.
          disable< GL_DITHER >();
          enable< GL_BLEND >();
          enable< GL_CULL_FACE >();
          enable< GL_DEPTH_TEST >();
          glew::c::glDepthFunc(GL_LEQUAL);
          
          if ( multisample_capability() )
            multisample();
          
          // Wonderland.Renderer try enble shadow feature.
          shadow();
          
          _initialize_point_light();
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
        auto create_shader_from_embedded()
          -> std::shared_ptr< const T_shader >
        { throw std::logic_error( "create_shader_from_embedded: invalid template parameter." ); }
        
        template < class T_embedded_shader_type_tag = shader::constant >
        auto create_program_from_embedded()
          -> std::shared_ptr< const renderer::program_t >
        { throw std::logic_error( "create_program_from_embedded: invalid template parameter." ); }
        
        auto camera()
          -> camera_t::shared_t
        { return _camera; }
        
        auto view_transformation() const
          -> glm::mat4
        { return _camera -> view_transformation(); }
        
        auto projection()
          -> projection_t::shared_t
        { return _projection; }
        
        auto projection_transformation() const
          -> glm::mat4
        { return _projection -> projection_transformation(); }
        
        auto view_projection_transformation() const
          -> glm::mat4
        { return projection_transformation() * view_transformation(); }
        
        template<class T_shader>
        auto create_shader() const
          -> std::shared_ptr< T_shader >
        { return std::make_shared< T_shader >(); }
        
        template<class T_shader>
        auto create_shader( const std::string& source ) const
          -> std::shared_ptr< T_shader >
        { return std::make_shared< T_shader >() -> compile( source ); }
        
        template<class T_shader>
        auto create_shader( std::string&& source ) const
          -> std::shared_ptr< const T_shader >
        {
          return std::make_shared< T_shader >()
            -> compile( std::forward< std::string >( source ) )
            ;
        }
        
        template<class T_shader>
        auto create_shader( std::istream&& source ) const
          -> std::shared_ptr< const T_shader >
        {
          return std::make_shared< T_shader >()
            -> compile( std::forward< std::istream >( source ) )
            ;
        }
        
        auto create_program()
          -> std::shared_ptr< renderer::program_t >
        { return std::make_shared< renderer::program_t >(); }
        
        template< class ... T_shaders >
        auto create_program( T_shaders&& ... shaders )
          -> std::shared_ptr< const renderer::program_t >
        {
          return ( _default_program = create_program() )
            -> attach( shaders ... )
            -> link()
            ;
        }
        
        template < class T >
        auto create_model( T t )
          -> std::shared_ptr< model_t >
        { return model_t::create(t); }
        
        auto enable_vertex_attribute( glew::gl_type::GLuint v )
          -> destruct_invoker_t
        {
          std::vector<glew::gl_type::GLuint> list;
          list.emplace_back( v );
          return enable_vertex_attributes( std::move( list ) );
        }
        
        auto enable_vertex_attributes( std::vector<glew::gl_type::GLuint> && list )
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
        
        auto flusher() const
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
      
      // specialize to point_light
      template<>
      auto renderer_t::create_shader_from_embedded< vertex_shader_t, shader::point_light >()
        -> std::shared_ptr< const vertex_shader_t >
      { return create_shader< vertex_shader_t >( shader::point_light::vs_source() ); }
      
      template<>
      auto renderer_t::create_shader_from_embedded< fragment_shader_t, shader::point_light >()
        -> std::shared_ptr< const fragment_shader_t >
      { return create_shader< fragment_shader_t >( shader::point_light::fs_source() ); }
      
      template<>
      auto renderer_t::create_program_from_embedded< shader::point_light >()
        -> std::shared_ptr< const renderer::program_t >
      { return create_program( create_shader_from_embedded< vertex_shader_t, shader::point_light >(), create_shader_from_embedded< fragment_shader_t, shader::point_light >() ); }
      
    }
  }
}