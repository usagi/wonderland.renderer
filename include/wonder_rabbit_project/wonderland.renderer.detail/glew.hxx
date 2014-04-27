#pragma once

#include <memory>

#include "glew.detail/c.hxx"
#include "glew.detail/gl_type.hxx"
#include "glew.detail/enum/ERROR.hxx"
#include "glew.detail/enum/SHADER.hxx"
#include "glew.detail/enum/VERTEX_ATTRIBUTE.hxx"
#include "glew.detail/enum/USAGE.hxx"
#include "glew.detail/enum/MODE.hxx"
#include "glew.detail/enum/CLEAR.hxx"
#include "glew.detail/basic_types.hxx"
#include "glew.detail/destruct_invoker.hxx"
#include "glew.detail/shader.hxx"
#include "glew.detail/program.hxx"
#include "glew.detail/vertex.hxx"
#include "glew.detail/model.hxx"

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      namespace glew
      {
        struct glew_t final
        {
            using this_type = glew_t;

            glew_t( const this_type& ) = delete;
            glew_t( this_type && )     = delete;
            this_type& operator=( const this_type& ) = delete;
            this_type& operator=( this_type && )     = delete;

            template<class T_SHADER>
            inline T_SHADER create_shader() const
            { return T_SHADER(); }
            
            template<class T_SHADER>
            inline T_SHADER create_shader( const std::string& source ) const
            {
              T_SHADER s;
              s.compile(source);
              return s;
            }
            
            template<class T_SHADER>
            inline T_SHADER create_shader( std::istream&& source ) const
            {
              T_SHADER s;
              s.compile( std::forward<std::istream>(source) );
              return s;
            }
            
            inline program create_program() const
            {
              return program();
            }
            
            template< class ... T_shaders >
            inline program create_program( T_shaders& ... shaders ) const
            {
              program pg;
              pg.attach( shaders ... );
              pg.link();
              return pg;
            }
            
            template < class T >
            inline auto create_model( T t )
              -> model_t
            { return model_t::create(t); }
            
            /*
            template
            < class TVERTEX, class TUSAGE, class TDEFAULT_INVOKE_MODE >
            inline model_vi
            < TVERTEX, TUSAGE, TDEFAULT_INVOKE_MODE >
            create_model
            (
              std::vector<TVERTEX> && data_vertices,
              std::vector<gl_type::GLuint> && data_indices
            ) const
            {
              return model_vi
                     <TVERTEX, TUSAGE, TDEFAULT_INVOKE_MODE>
                     ( std::move( data_vertices ), std::move( data_indices ) );
            }

            template < class TVERTEX, class TUSAGE>
            inline model_vi< TVERTEX, TUSAGE, mode_points >
            create_model
            (
              std::vector<TVERTEX> && data_vertices,
              std::vector<gl_type::GLuint> && data_indices
            ) const
            {
              return create_model
                     <TVERTEX, TUSAGE, mode_points>
                     ( std::move( data_vertices ), std::move( data_indices ) );
            }

            template <class TVERTEX>
            inline model_vi< TVERTEX, usage_static_draw, mode_points >
            create_model
            (
              std::vector<TVERTEX> && data_vertices,
              std::vector<gl_type::GLuint> && data_indices
            ) const
            {
              return create_model
                     <TVERTEX, usage_static_draw>
                     ( std::move( data_vertices ), std::move( data_indices ) );
            }

            template
            < class TVERTEX, class TUSAGE, class TDEFAULT_INVOKE_MODE >
            inline model_v
            < TVERTEX, TUSAGE, TDEFAULT_INVOKE_MODE >
            create_model( std::vector<TVERTEX> && data ) const
            {
              return model_v
                     <TVERTEX, TUSAGE, TDEFAULT_INVOKE_MODE>
                     ( std::move( data ) );
            }
            */

            inline destruct_invoker
            enable_vertex_attribute( gl_type::GLuint v )
            {
              std::vector<gl_type::GLuint> list;
              list.emplace_back( v );
              return enable_vertex_attributes( std::move( list ) );
            }

            inline destruct_invoker
            enable_vertex_attributes( std::vector<gl_type::GLuint> && list )
            {
              for ( const auto & n : list )
                c::glEnableVertexAttribArray( n );

              return
              {
                [list]()
                {
                  std::for_each(
                    list.crbegin(), list.crend(),
                    []( gl_type::GLuint n )
                  {
                    c::glDisableVertexAttribArray( n );
                  }
                  );
                }
              };
            }

            /*
            template < class TVERTEX, class TUSAGE>
            inline model_v< TVERTEX, TUSAGE, mode_points >
            create_model( std::vector<TVERTEX> && data ) const
            {
              return create_model
                     <TVERTEX, TUSAGE, mode_points>( std::move( data ) );
            }

            template < class TVERTEX>
            inline model_v< TVERTEX, usage_static_draw, mode_points >
            create_model( std::vector<TVERTEX> && data ) const
            {
              return create_model
                     <TVERTEX, usage_static_draw>( std::move( data ) );
            }
            */

            inline destruct_invoker use_program( const program& p ) const
            {
              c::glUseProgram( p._program );
              return { []{ c::glUseProgram( 0 ); } };
            }
            
            // tips: template specialization version is exitst on bottom
            template < class T >
            inline auto uniform( program program_, gl_type::GLint localtion )
              -> T
            { return program_.uniform< T >( localtion ); }
            
            inline auto uniform( program program_, const std::string name )
              -> gl_type::GLuint
            {
              const auto location = c::glGetUniformLocation( program_._program, name.data() );
              test_error();
              return location;
            }
            
            // float
            inline auto uniform( gl_type::GLint location, const gl_type::GLfloat value ) const -> void
            { c::glUniform1f( location, value ); }
            
            inline auto uniform( gl_type::GLint location, const glm::vec1& value) const -> void
            { c::glUniform1f( location, value.x ); }

            inline auto uniform( gl_type::GLint location, const std::vector<float>& values) const -> void
            { c::glUniform1fv( location, values.size(), values.data() ); }
            
            inline auto uniform( gl_type::GLint location, const std::vector<glm::vec1>& values) const -> void
            { c::glUniform1fv( location, values.size(), & values.data() -> x ); }
            
            inline auto uniform( gl_type::GLint location, const glm::vec2& value ) const -> void
            { c::glUniform2fv( location, 1, & value.x ); }

            inline auto uniform( gl_type::GLint location, const std::vector<glm::vec2>& values ) const -> void
            { c::glUniform2fv( location, values.size(), & values.data() -> x ); }
            
            inline auto uniform( gl_type::GLint location, const glm::vec3& value ) const -> void
            { c::glUniform3fv( location, 1, & value.x ); }

            inline auto uniform( gl_type::GLint location, const std::vector<glm::vec3>& values ) const -> void
            { c::glUniform3fv( location, values.size(), & values.data() -> x ); }
            
            inline auto uniform( gl_type::GLint location, const glm::vec4& value ) const -> void
            { c::glUniform4fv( location, 1, & value.x ); }

            inline auto uniform( gl_type::GLint location, const std::vector<glm::vec4>& values ) const -> void
            { c::glUniform4fv( location, values.size(), & values.data() -> x ); }
            
            inline auto uniform( gl_type::GLint location, const glm::mat4& value ) const -> void
            { c::glUniformMatrix4fv( location, 1, false, & value[0][0] ); }
            
            inline auto uniform( gl_type::GLint location, const std::vector<glm::mat4>& values ) const -> void
            { c::glUniformMatrix4fv( location, values.size(), false, & ( *values.data() )[0][0] ); }
            
            // int
            inline auto uniform( gl_type::GLint location, const gl_type::GLint value ) const -> void
            { c::glUniform1i( location, value ); }

            inline auto uniform( gl_type::GLint location, const glm::i32vec1 value ) const -> void
            { c::glUniform1i( location, value.x ); }
            
            inline void uniform( gl_type::GLint location, const std::vector<gl_type::GLint>& values ) const
            { c::glUniform1iv( location, values.size(), values.data() ); }

            inline void uniform( gl_type::GLint location, const std::vector<glm::i32vec1>& values ) const
            { c::glUniform1iv( location, values.size(), & values.data() -> x ); }
            
            inline void uniform( gl_type::GLint location, const glm::i32vec2& value ) const
            { c::glUniform2iv( location, 1, & value.x ); }

            inline void uniform( gl_type::GLint location, const std::vector<glm::i32vec2>& values ) const
            { c::glUniform2iv( location, values.size(), & values.data() -> x ); }
            
            inline void uniform( gl_type::GLint location, const glm::i32vec3& value ) const
            { c::glUniform3iv( location, 1, & value.x ); }
            
            inline void uniform( gl_type::GLint location, const std::vector<glm::i32vec3>& values ) const
            { c::glUniform3iv( location, values.size(), & values.data() -> x ); }
            
            inline void uniform( gl_type::GLint location, const glm::i32vec4& value ) const
            { c::glUniform4iv( location, 1, & value.x ); }
            
            inline void uniform( gl_type::GLint location, const std::vector<glm::i32vec4>& values ) const
            { c::glUniform4iv( location, values.size(), & values.data() -> x ); }
            
            // uint
            inline auto uniform( gl_type::GLint location, const gl_type::GLuint value ) const -> void
            { c::glUniform1ui( location, value ); }
            
            inline auto uniform( gl_type::GLint location, const glm::u32vec1 value ) const -> void
            { c::glUniform1ui( location, value.x ); }
            
            inline void uniform( gl_type::GLint location, const std::vector<gl_type::GLuint>& values ) const
            { c::glUniform1uiv( location, values.size(), values.data() ); }
            
            inline void uniform( gl_type::GLint location, const std::vector<glm::u32vec1>& values ) const
            { c::glUniform1uiv( location, values.size(), & values.data() -> x ); }
            
            inline void uniform( gl_type::GLint location, const glm::u32vec2& value ) const
            { c::glUniform2uiv( location, 1, & value.x ); }
            
            inline void uniform( gl_type::GLint location, const std::vector<glm::u32vec2>& values ) const
            { c::glUniform2uiv( location, values.size(), & values.data() -> x ); }
            
            inline void uniform( gl_type::GLint location, const glm::u32vec3& value ) const
            { c::glUniform3uiv( location, 1, & value.x ); }
            
            inline void uniform( gl_type::GLint location, const std::vector<glm::u32vec3>& values ) const
            { c::glUniform3uiv( location, values.size(), & values.data() -> x ); }
            
            inline void uniform( gl_type::GLint location, const glm::u32vec4& value ) const
            { c::glUniform4uiv( location, 1, & value.x ); }
            
            inline void uniform( gl_type::GLint location, const std::vector<glm::u32vec4>& values ) const
            { c::glUniform4uiv( location, values.size(), & values.data() -> x ); }
            
            /*
            template<class TMODE, class TMODEL>
            inline auto draw( const TMODEL& m ) const -> void
            { m.template draw<TMODE>(); }
            */
            
            inline auto draw( const model_t& m ) const -> void
            { m.draw(); }
            
            inline auto flush() const -> void
            { c::glFlush(); }
            
            inline auto clear_color( const glm::vec4& color ) const -> void
            { c::glClearColor( color.r, color.g, color.b, color.a ); }
            
            inline auto clear( CLEAR flag = CLEAR::ALL ) const -> void
            { c::glClear( gl_type::GLint( flag ) ); }

            inline destruct_invoker flusher() const
            {
              return { [this]{ this->flush(); } };
            }

            static auto instance() -> this_type*
            {
              static this_type i;
              //i -> initialize();
              return &i;
            }

            void initialize()
            {
              auto r = c::glewInit();
              
              if ( r != GLEW_OK )
              {
                auto message = std::string( "glewInit fail. code = " ) + std::to_string( r ) + " : " +  std::string( reinterpret_cast<const char*>( c::glewGetErrorString( r ) ) );
                throw std::runtime_error( message );
              }
            }
            
        private:

            glew_t()
            {
              //initialize();
            }

        };
        
      }
    }
  }
}