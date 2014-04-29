#pragma once

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

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      
      class renderer_t
      {
        camera_t  _camera; // to view transformation
        glm::mat4 _projection_transformation;
        
      public:
        
        explicit renderer_t()
          : _camera()
          , _projection_transformation( glm::mat4() )
        {
          glew::glew_init();
          cull_face();
          depth_test();
          if ( multisample_capability() )
            multisample();
        }
        
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
        inline T_shader create_shader() const
        { return T_shader(); }
        
        template<class T_shader>
        inline T_shader create_shader( const std::string& source ) const
        {
          T_shader s;
          s.compile(source);
          return s;
        }
        
        template<class T_shader>
        inline T_shader create_shader( std::istream&& source ) const
        {
          T_shader s;
          s.compile( std::forward<std::istream>(source) );
          return s;
        }
        
        inline program_t create_program() const
        {
          return program_t();
        }
        
        template< class ... T_shaders >
        inline program_t create_program( T_shaders& ... shaders ) const
        {
          program_t pg;
          pg.attach( shaders ... );
          pg.link();
          return pg;
        }
        
        template < class T >
        inline auto create_model( T t )
        -> model_t
        { return model_t::create(t); }
        
        inline destruct_invoker_t
        enable_vertex_attribute( glew::gl_type::GLuint v )
        {
          std::vector<glew::gl_type::GLuint> list;
          list.emplace_back( v );
          return enable_vertex_attributes( std::move( list ) );
        }
        
        inline destruct_invoker_t
        enable_vertex_attributes( std::vector<glew::gl_type::GLuint> && list )
        {
          for ( const auto & n : list )
            glew::c::glEnableVertexAttribArray( n );
          
          return
          {
            [list]()
            {
              std::for_each(
                list.crbegin(), list.crend(),
                            []( glew::gl_type::GLuint n )
                            {
                              glew::c::glDisableVertexAttribArray( n );
                            }
              );
            }
          };
        }
        
        inline destruct_invoker_t use_program( const program_t& p ) const
        {
          glew::c::glUseProgram( p._program );
          return { []{ glew::c::glUseProgram( 0 ); } };
        }
        
        template < class T >
        inline auto uniform( program_t program_, glew::gl_type::GLint localtion )
        -> T
        { return program_.uniform< T >( localtion ); }
        
        inline auto uniform( program_t program_, const std::string name )
        -> glew::gl_type::GLuint
        { return uniform( program_._program, name ); }
        
        inline auto uniform( glew::gl_type::GLint program_id, const std::string name )
        -> glew::gl_type::GLuint
        {
          const auto location = glew::c::glGetUniformLocation( program_id, name.data() );
          return location;
        }
        
        // float
        inline auto uniform( glew::gl_type::GLint location, const glew::gl_type::GLfloat value ) const -> void
        { glew::c::glUniform1f( location, value ); }
        
        inline auto uniform( glew::gl_type::GLint location, const glm::vec1& value) const -> void
        { glew::c::glUniform1f( location, value.x ); }
        
        inline auto uniform( glew::gl_type::GLint location, const std::vector<float>& values) const -> void
        { glew::c::glUniform1fv( location, values.size(), values.data() ); }
        
        inline auto uniform( glew::gl_type::GLint location, const std::vector<glm::vec1>& values) const -> void
        { glew::c::glUniform1fv( location, values.size(), & values.data() -> x ); }
        
        inline auto uniform( glew::gl_type::GLint location, const glm::vec2& value ) const -> void
        { glew::c::glUniform2fv( location, 1, & value.x ); }
        
        inline auto uniform( glew::gl_type::GLint location, const std::vector<glm::vec2>& values ) const -> void
        { glew::c::glUniform2fv( location, values.size(), & values.data() -> x ); }
        
        inline auto uniform( glew::gl_type::GLint location, const glm::vec3& value ) const -> void
        { glew::c::glUniform3fv( location, 1, & value.x ); }
        
        inline auto uniform( glew::gl_type::GLint location, const std::vector<glm::vec3>& values ) const -> void
        { glew::c::glUniform3fv( location, values.size(), & values.data() -> x ); }
        
        inline auto uniform( glew::gl_type::GLint location, const glm::vec4& value ) const -> void
        { glew::c::glUniform4fv( location, 1, & value.x ); }
        
        inline auto uniform( glew::gl_type::GLint location, const std::vector<glm::vec4>& values ) const -> void
        { glew::c::glUniform4fv( location, values.size(), & values.data() -> x ); }
        
        inline auto uniform( glew::gl_type::GLint location, const glm::mat4& value ) const -> void
        { glew::c::glUniformMatrix4fv( location, 1, false, & value[0][0] ); }
        
        inline auto uniform( glew::gl_type::GLint location, const std::vector<glm::mat4>& values ) const -> void
        { glew::c::glUniformMatrix4fv( location, values.size(), false, & ( *values.data() )[0][0] ); }
        
        // int
        inline auto uniform( glew::gl_type::GLint location, const glew::gl_type::GLint value ) const -> void
        { glew::c::glUniform1i( location, value ); }
        
        inline auto uniform( glew::gl_type::GLint location, const glm::i32vec1 value ) const -> void
        { glew::c::glUniform1i( location, value.x ); }
        
        inline void uniform( glew::gl_type::GLint location, const std::vector<glew::gl_type::GLint>& values ) const
        { glew::c::glUniform1iv( location, values.size(), values.data() ); }
        
        inline void uniform( glew::gl_type::GLint location, const std::vector<glm::i32vec1>& values ) const
        { glew::c::glUniform1iv( location, values.size(), & values.data() -> x ); }
        
        inline void uniform( glew::gl_type::GLint location, const glm::i32vec2& value ) const
        { glew::c::glUniform2iv( location, 1, & value.x ); }
        
        inline void uniform( glew::gl_type::GLint location, const std::vector<glm::i32vec2>& values ) const
        { glew::c::glUniform2iv( location, values.size(), & values.data() -> x ); }
        
        inline void uniform( glew::gl_type::GLint location, const glm::i32vec3& value ) const
        { glew::c::glUniform3iv( location, 1, & value.x ); }
        
        inline void uniform( glew::gl_type::GLint location, const std::vector<glm::i32vec3>& values ) const
        { glew::c::glUniform3iv( location, values.size(), & values.data() -> x ); }
        
        inline void uniform( glew::gl_type::GLint location, const glm::i32vec4& value ) const
        { glew::c::glUniform4iv( location, 1, & value.x ); }
        
        inline void uniform( glew::gl_type::GLint location, const std::vector<glm::i32vec4>& values ) const
        { glew::c::glUniform4iv( location, values.size(), & values.data() -> x ); }
        
        // uint
        inline auto uniform( glew::gl_type::GLint location, const glew::gl_type::GLuint value ) const -> void
        { glew::c::glUniform1ui( location, value ); }
        
        inline auto uniform( glew::gl_type::GLint location, const glm::u32vec1 value ) const -> void
        { glew::c::glUniform1ui( location, value.x ); }
        
        inline void uniform( glew::gl_type::GLint location, const std::vector<glew::gl_type::GLuint>& values ) const
        { glew::c::glUniform1uiv( location, values.size(), values.data() ); }
        
        inline void uniform( glew::gl_type::GLint location, const std::vector<glm::u32vec1>& values ) const
        { glew::c::glUniform1uiv( location, values.size(), & values.data() -> x ); }
        
        inline void uniform( glew::gl_type::GLint location, const glm::u32vec2& value ) const
        { glew::c::glUniform2uiv( location, 1, & value.x ); }
        
        inline void uniform( glew::gl_type::GLint location, const std::vector<glm::u32vec2>& values ) const
        { glew::c::glUniform2uiv( location, values.size(), & values.data() -> x ); }
        
        inline void uniform( glew::gl_type::GLint location, const glm::u32vec3& value ) const
        { glew::c::glUniform3uiv( location, 1, & value.x ); }
        
        inline void uniform( glew::gl_type::GLint location, const std::vector<glm::u32vec3>& values ) const
        { glew::c::glUniform3uiv( location, values.size(), & values.data() -> x ); }
        
        inline void uniform( glew::gl_type::GLint location, const glm::u32vec4& value ) const
        { glew::c::glUniform4uiv( location, 1, & value.x ); }
        
        inline void uniform( glew::gl_type::GLint location, const std::vector<glm::u32vec4>& values ) const
        { glew::c::glUniform4uiv( location, values.size(), & values.data() -> x ); }
        
        /*
         *            template<class TMODE, class TMODEL>
         *            inline auto draw( const TMODEL& m ) const -> void
         *            { m.template draw<TMODE>(); }
         */
        
        inline auto draw( const model_t& m, const glm::mat4& world_transformation = glm::mat4() ) -> void
        {
          const auto wvp = _projection_transformation * _camera.view_transformation() * world_transformation;
          
          glew::gl_type::GLint program_id;
          glew::c::glGetIntegerv( GL_CURRENT_PROGRAM, &program_id );
          
          if ( program_id )
          {
            const auto location_of_world_view_projection_transformation = uniform( program_id, "world_view_projection_transformation" );
            const auto location_of_world_transformation = uniform( program_id, "world_transformation" );
          
            if ( location_of_world_view_projection_transformation not_eq -1 )
              uniform( location_of_world_view_projection_transformation, wvp );
            if ( location_of_world_transformation not_eq -1 )
              uniform( location_of_world_transformation, world_transformation );
          }
          
          m.draw();
        }
        
        inline auto flush() const -> void
        { glew::c::glFlush(); }
        
        inline auto clear_color( const glm::vec4& color ) const -> void
        { glew::c::glClearColor( color.r, color.g, color.b, color.a ); }
        
        inline auto clear( glew::gl_type::GLbitfield flag = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT ) const -> void
        { glew::c::glClear( glew::gl_type::GLint( flag ) ); }
        
        inline auto flusher() const -> destruct_invoker_t
        { return { [this]{ this->flush(); } }; }
        
#define WRP_TMP( WRP_TMP_X, WRP_TMP_Y ) \
        inline auto WRP_TMP_X ( bool enable = true ) -> void \
        { if( enable ) glew::c::glEnable( WRP_TMP_Y ); else glew::c::glDisable( WRP_TMP_Y ); }
        
        WRP_TMP( blend       , GL_BLEND )
        WRP_TMP( cull_face   , GL_CULL_FACE )
        WRP_TMP( debug_output, GL_DEBUG_OUTPUT )
        WRP_TMP( depth_test  , GL_DEPTH_TEST )
        
#undef WRP_TMP
        
        inline auto multisample_capability() -> bool
        {
          glew::gl_type::GLint buffers, samples;
          
          glew::c::glGetIntegerv(GL_SAMPLE_BUFFERS, &buffers);
          glew::c::glGetIntegerv(GL_SAMPLES, &samples);
          
          return buffers >= 1 and samples >= 2;
        }
        
        inline auto multisample( bool enable = true ) -> void
        {
          if ( not enable )
          {
#ifndef EMSCRIPTEN
            glew::c::glDisable( GL_MULTISAMPLE );
#endif
            glew::c::glDisable( GL_SAMPLE_ALPHA_TO_COVERAGE );
            return;
          }
          
          if ( not multisample_capability() )
            throw std::runtime_error( "cannot use multi sample." );
#ifndef EMSCRIPTEN
          // need OpenGL API
          //  but on GLES2 MULTISAMPLE is true default and not support glEnable/glDisable it.
          glew::c::glEnable( GL_MULTISAMPLE );
#endif
          glew::c::glEnable( GL_SAMPLE_ALPHA_TO_COVERAGE );
          
        }
        
      };
    }
  }
}