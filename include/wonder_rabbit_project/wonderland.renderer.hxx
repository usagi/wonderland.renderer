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
        camera_t  _camera; // to view transformation
        glm::mat4 _projection_transformation;
        boost::optional< const renderer::program_t& > _default_program;
        std::list< const light_t* > _default_lights;
        
        bool _shadow;
        std::unique_ptr< renderer::program_t > _shadow_mapping_program;
        
        struct draw_params_t
        {
          model_t& model;
          const glm::mat4 world_transformation;
          const model::animation_states_t animation_states;
          
          draw_params_t
          ( model_t& model_
          , const glm::mat4& world_transformation_
          , const model::animation_states_t& animation_states_
          )
            : model( model_ )
            , world_transformation( world_transformation_ )
            , animation_states( animation_states_ )
          { }
        };
        
        std::list< draw_params_t > _draw_queue;
        
        auto _create_shadow_mapping_program() -> void
        {
          _shadow_mapping_program.reset( new renderer::program_t() );
          _shadow_mapping_program -> attach( create_shader< vertex_shader_t   >( shader::shadow_mapping::vs_source() ) );
          _shadow_mapping_program -> attach( create_shader< fragment_shader_t >( shader::shadow_mapping::fs_source() ) );
          _shadow_mapping_program -> link();
        }
        
        auto _shadow_on() -> void
        {
          if ( not _shadow_mapping_program )
            try
            { _create_shadow_mapping_program(); }
            catch ( ... )
            {
              std::cerr << "warn: cannot use shadow program. Wonderland.Renderer shadow feature to off.";
              _shadow_off();
              return;
            }
          
          _shadow = true;
        }
        
        auto _shadow_off() -> void
        {
          _shadow = false;
        }
        
        auto _invoke_draw() -> void
        {
          if ( _shadow )
            for ( const auto& draw_params : _draw_queue )
              _draw_shadow( draw_params );
          
          for ( const auto& draw_params : _draw_queue )
            _draw( draw_params );
          
          _draw_queue.clear();
        }
        
        auto _draw_shadow( const draw_params_t& draw_params )
          -> void
        {
          const auto wv
            = _camera.view_transformation()
            * draw_params.world_transformation
            ;
            
          const auto wvp = _projection_transformation * wv;
          
          const auto program_id = current_program();
          
          uniform( program_id, "world_view_projection_transformation", wvp );
          uniform( program_id, "world_view_transformation", wv );
          uniform( program_id, "world_transformation", draw_params.world_transformation );
          uniform( program_id, "view_direction", _camera.view_direction() );
          
          draw_params.model.draw( draw_params.animation_states );
        }
        
        auto _draw( const draw_params_t& draw_params )
          -> void
        {
          const auto wv
            = _camera.view_transformation()
            * draw_params.world_transformation
            ;
            
          const auto wvp = _projection_transformation * wv;
          
          const auto program_id = current_program();
          
          uniform( program_id, "world_view_projection_transformation", wvp );
          uniform( program_id, "world_view_transformation", wv );
          uniform( program_id, "world_transformation", draw_params.world_transformation );
          uniform( program_id, "view_direction", _camera.view_direction() );
          
          draw_params.model.draw( draw_params.animation_states );
        }
        
      public:
        
        explicit renderer_t()
          : _camera()
          , _projection_transformation( glm::mat4() )
        {
          glew::glew_init();
          blend();
          cull_face();
          depth_test();
          if ( multisample_capability() )
            multisample();
          
          shadow();
        }
        
        template < class T_type, class ... T_prams >
        auto create_light( T_prams ... params ) -> T_type
        {
          static_assert( std::is_base_of<light_t, T_type>(), "create_light need class of a based on light_t." );
          auto light = T_type( params ... );
          add_default_light( light );
          return light;
        }
        
        template <class T_shader , class T_embedded_shader_type_tag = shader::constant >
        inline auto create_shader_from_embedded() -> T_shader
        { throw std::logic_error( "create_shader_from_embedded: invalid template parameter." ); }
        
        template <class T_embedded_shader_type_tag = shader::constant >
        inline auto create_program_from_embedded() -> renderer::program_t
        { throw std::logic_error( "create_program_from_embedded: invalid template parameter." ); }
        
        inline auto view_projection_transformation() -> glm::mat4
        { return _projection_transformation * _camera.view_transformation(); }
        
        inline auto projection_transformation() -> const glm::mat4&
        { return _projection_transformation; }
        
        inline auto projection_transformation( const glm::mat4& v) -> void
        { _projection_transformation = v; }
        
        inline auto camera() -> camera_t&
        { return _camera; }
        
        inline auto view_transformation() -> glm::mat4
        { return _camera.view_transformation(); }
        
        template<class T_shader>
        inline auto create_shader() const
          -> T_shader
        { return T_shader(); }
        
        template<class T_shader>
        inline auto create_shader( const std::string& source ) const
          -> T_shader 
        {
          T_shader s;
          s.compile( source );
          return s;
        }
        
        template<class T_shader>
        inline auto create_shader( std::string&& source ) const
          -> T_shader
        {
          T_shader s;
          s.compile( std::forward< std::string >( source ) );
          return s;
        }
        
        template<class T_shader>
        inline auto create_shader( std::istream&& source ) const
          -> T_shader
        {
          T_shader s;
          s.compile( std::forward< std::istream >( source ) );
          return s;
        }
        
        inline auto create_program()
          -> wonder_rabbit_project::wonderland::renderer::program_t
        {
          return wonder_rabbit_project::wonderland::renderer::program_t();
        }
        
        template< class ... T_shaders >
        inline auto create_program( T_shaders&& ... shaders )
          -> wonder_rabbit_project::wonderland::renderer::program_t
        {
          wonder_rabbit_project::wonderland::renderer::program_t pg;
          pg.attach( shaders ... );
          pg.link();
          _default_program = pg;
          return pg;
        }
        
        template < class T >
        inline auto create_model( T t )
          -> model_t
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
        
        inline auto use_program( const renderer::program_t& p ) const
          -> destruct_invoker_t
        {
          glew::wrapper_t::use_program( p._program );
          return { [ ]{ glew::wrapper_t::use_program(); } };
        }
        
        auto draw
        ( model_t& model
        , const glm::mat4& world_transformation = glm::mat4( 1.0f )
        , const model::animation_states_t& animation_states = { }
        )
          -> void
        { _draw_queue.emplace_back( model, world_transformation, animation_states ); }
        
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
        { _default_lights.push_back( &light ); }
        
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
        { _default_lights.remove( &light ); }
        
        auto activate_lights() const
          -> void
        {
          for ( const auto* light : _default_lights )
            light -> activate();
        }
        
        auto default_program( const renderer::program_t& p )
          -> void
        { _default_program = p; }
        
        auto default_program()
          -> const renderer::program_t&
        {
          if( _default_program )
            return _default_program.get();
          
          throw std::logic_error( "default program is not set yet." );
        }
        
        auto invoker()
          -> std::array< destruct_invoker_t, 3 >
        { return invoker( _default_program.get() ); }
        
        auto invoker( const renderer::program_t& p )
          -> std::array< destruct_invoker_t, 3 >
        {
          clear();
          auto rf = this -> flusher();
          auto rp = this -> use_program( p );
          activate_lights();
          return
          { { std::move( rf )
            , std::move( rp )
            , destruct_invoker_t( [ this ]{ _invoke_draw(); } )
            }
          };
        }
        
        inline auto flusher() const -> destruct_invoker_t
        { return { [ ]{ glew::wrapper_t::flush(); } }; }
        
        auto shadow( bool enable = true) -> void
        { enable ? _shadow_on() : _shadow_off(); }
      };
      
      // specialize to constant
      template<>
      auto renderer_t::create_shader_from_embedded< vertex_shader_t, shader::constant >() -> vertex_shader_t
      { return create_shader< vertex_shader_t >( shader::constant::vs_source() ); }
      
      template<>
      auto renderer_t::create_shader_from_embedded< fragment_shader_t, shader::constant >() -> fragment_shader_t
      { return create_shader< fragment_shader_t >( shader::constant::fs_source() ); }
      
      template<>
      auto renderer_t::create_program_from_embedded< shader::constant >() -> renderer::program_t
      { return create_program( create_shader_from_embedded< vertex_shader_t, shader::constant >(), create_shader_from_embedded< fragment_shader_t, shader::constant >() ); }
      
      // specialize to phong
      template<>
      auto renderer_t::create_shader_from_embedded< vertex_shader_t, shader::phong >() -> vertex_shader_t
      { return create_shader< vertex_shader_t >( shader::phong::vs_source() ); }
      
      template<>
      auto renderer_t::create_shader_from_embedded< fragment_shader_t, shader::phong >() -> fragment_shader_t
      { return create_shader< fragment_shader_t >( shader::phong::fs_source() ); }
      
      template<>
      auto renderer_t::create_program_from_embedded< shader::phong >() -> renderer::program_t
      { return create_program( create_shader_from_embedded< vertex_shader_t, shader::phong >(), create_shader_from_embedded< fragment_shader_t, shader::phong >() ); }
      
      // specialize to cartoon
      template<>
      auto renderer_t::create_shader_from_embedded< vertex_shader_t, shader::cartoon >() -> vertex_shader_t
      { return create_shader< vertex_shader_t >( shader::cartoon::vs_source() ); }
      
      template<>
      auto renderer_t::create_shader_from_embedded< fragment_shader_t, shader::cartoon >() -> fragment_shader_t
      { return create_shader< fragment_shader_t >( shader::cartoon::fs_source() ); }
      
      template<>
      auto renderer_t::create_program_from_embedded< shader::cartoon >() -> renderer::program_t
      { return create_program( create_shader_from_embedded< vertex_shader_t, shader::cartoon >(), create_shader_from_embedded< fragment_shader_t, shader::cartoon >() ); }
      
      // specialize to shadow_mapping
      template<>
      auto renderer_t::create_shader_from_embedded< vertex_shader_t, shader::shadow_mapping >() -> vertex_shader_t
      { return create_shader< vertex_shader_t >( shader::shadow_mapping::vs_source() ); }
      
      template<>
      auto renderer_t::create_shader_from_embedded< fragment_shader_t, shader::shadow_mapping >() -> fragment_shader_t
      { return create_shader< fragment_shader_t >( shader::shadow_mapping::fs_source() ); }
      
      template<>
      auto renderer_t::create_program_from_embedded< shader::shadow_mapping >() -> renderer::program_t
      { return create_program( create_shader_from_embedded< vertex_shader_t, shader::shadow_mapping >(), create_shader_from_embedded< fragment_shader_t, shader::shadow_mapping >() ); }
      
    }
  }
}
