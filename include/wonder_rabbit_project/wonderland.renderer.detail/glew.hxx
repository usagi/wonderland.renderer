#pragma once

#include <memory>

#include "glew.detail/c.hxx"
#include "glew.detail/gl_type.hxx"
#include "glew.detail/enum/ERROR.hxx"
#include "glew.detail/enum/SHADER.hxx"
#include "glew.detail/enum/VERTEX_ATTRIBUTE.hxx"
#include "glew.detail/enum/USAGE.hxx"
#include "glew.detail/enum/MODE.hxx"
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
        struct glew final
        {
            using this_type = glew;

            glew( const this_type& ) = delete;
            glew( this_type && )      = delete;
            this_type& operator=( const this_type& ) = delete;
            this_type& operator=( this_type && )     = delete;

            template<class TSHADER>
            inline TSHADER create_shader() const
            {
              return TSHADER();
            }

            inline program create_program() const
            {
              return program();
            }

            template
            <
            class TVERTEX,
                  class TUSAGE,
                  class TDEFAULT_INVOKE_MODE
                  >
            inline model_vi
            <
            TVERTEX, TUSAGE, TDEFAULT_INVOKE_MODE
            >
            create_model(
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
            create_model(
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
            create_model(
              std::vector<TVERTEX> && data_vertices,
              std::vector<gl_type::GLuint> && data_indices
            ) const
            {
              return create_model
                     <TVERTEX, usage_static_draw>
                     ( std::move( data_vertices ), std::move( data_indices ) );
            }

            template
            <
            class TVERTEX,
                  class TUSAGE,
                  class TDEFAULT_INVOKE_MODE
                  >
            inline model_v
            <
            TVERTEX, TUSAGE, TDEFAULT_INVOKE_MODE
            >
            create_model(
              std::vector<TVERTEX> && data
            ) const
            {
              return model_v
                     <TVERTEX, TUSAGE, TDEFAULT_INVOKE_MODE>
                     ( std::move( data ) );
            }

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

            inline void use_program( const program& p ) const
            {
              c::glUseProgram( p.program_ );
            }

            // float
            inline void uniform(
              gl_type::GLint location,
              const gl_type::GLfloat value
            ) const
            {
              c::glUniform1f( location, value );
            }

            template<size_t N>
            inline void uniform(
              gl_type::GLint location,
              const vec<vec<gl_type::GLfloat, 1>, N>& values
            ) const
            {
              c::glUniform1fv( location, N, values.data()->data() );
            }

            inline void uniform(
              gl_type::GLint location,
              const u_vec2<gl_type::GLfloat>& values
            ) const
            {
              c::glUniform2fv( location, 1, values.data() );
            }

            template<size_t N>
            inline void uniform(
              gl_type::GLint location,
              const vec<u_vec2<gl_type::GLfloat>, N>& values
            ) const
            {
              c::glUniform2fv( location, N, values.data()->data() );
            }

            inline void uniform(
              gl_type::GLint location,
              const u_vec3<gl_type::GLfloat>& values
            ) const
            {
              c::glUniform3fv( location, 1, values.data() );
            }

            template<size_t N>
            inline void uniform(
              gl_type::GLint location,
              const vec<u_vec3<gl_type::GLfloat>, N>& values
            ) const
            {
              c::glUniform3fv( location, N, values.data()->data() );
            }

            inline void uniform(
              gl_type::GLint location,
              const u_vec4<gl_type::GLfloat>& values
            ) const
            {
              c::glUniform4fv( location, 1, values.data() );
            }

            template<size_t N>
            inline void uniform(
              gl_type::GLint location,
              const vec<u_vec4<gl_type::GLfloat>, N>& values
            ) const
            {
              c::glUniform4fv( location, N, values.data()->data() );
            }

            inline void uniform(
              gl_type::GLint location,
              const u_matrix44<gl_type::GLfloat>& values
            ) const
            {
              c::glUniformMatrix4fv( location, 1, false, values.data() );
            }

            template<size_t N>
            inline void uniform(
              gl_type::GLint location,
              const vec<u_matrix44<gl_type::GLfloat>, N>& values
            ) const
            {
              c::glUniformMatrix4fv( location, N, false, values.data()->data() );
            }

            // int
            inline void uniform(
              gl_type::GLint location,
              const gl_type::GLint value
            ) const
            {
              c::glUniform1i( location, value );
            }

            template<size_t N>
            inline void uniform(
              gl_type::GLint location,
              const vec<vec<gl_type::GLint, 1>, N>& values
            ) const
            {
              c::glUniform1iv( location, N, values.data()->data() );
            }

            inline void uniform(
              gl_type::GLint location,
              const u_vec2<gl_type::GLint>& values
            ) const
            {
              c::glUniform2iv( location, 1, values.data() );
            }

            template<size_t N>
            inline void uniform(
              gl_type::GLint location,
              const vec<u_vec2<gl_type::GLint>, N>& values
            ) const
            {
              c::glUniform2iv( location, N, values.data()->data() );
            }

            inline void uniform(
              gl_type::GLint location,
              const u_vec3<gl_type::GLint>& values
            ) const
            {
              c::glUniform3iv( location, 1, values.data() );
            }

            template<size_t N>
            inline void uniform(
              gl_type::GLint location,
              const vec<u_vec3<gl_type::GLint>, N>& values
            ) const
            {
              c::glUniform3iv( location, N, values.data()->data() );
            }

            inline void uniform(
              gl_type::GLint location,
              const u_vec4<gl_type::GLint>& values
            ) const
            {
              c::glUniform4iv( location, 1, values.data() );
            }

            template<size_t N>
            inline void uniform(
              gl_type::GLint location,
              const vec<u_vec4<gl_type::GLint>, N>& values
            ) const
            {
              c::glUniform4iv( location, N, values.data()->data() );
            }

            // uint
            inline void uniform(
              gl_type::GLint location,
              const gl_type::GLuint value
            ) const
            {
              c::glUniform1i( location, value );
            }

            template<size_t N>
            inline void uniform(
              gl_type::GLint location,
              const vec<vec<gl_type::GLuint, 1>, N>& values
            ) const
            {
              c::glUniform1uiv( location, N, values.data()->data() );
            }

            inline void uniform(
              gl_type::GLint location,
              const u_vec2<gl_type::GLuint>& values
            ) const
            {
              c::glUniform2uiv( location, 1, values.data() );
            }

            template<size_t N>
            inline void uniform(
              gl_type::GLint location,
              const vec<u_vec2<gl_type::GLuint>, N>& values
            ) const
            {
              c::glUniform2uiv( location, N, values.data()->data() );
            }

            inline void uniform(
              gl_type::GLint location,
              const u_vec3<gl_type::GLuint>& values
            ) const
            {
              c::glUniform3uiv( location, 1, values.data() );
            }

            template<size_t N>
            inline void uniform(
              gl_type::GLint location,
              const vec<u_vec3<gl_type::GLuint>, N>& values
            ) const
            {
              c::glUniform3uiv( location, N, values.data()->data() );
            }

            inline void uniform(
              gl_type::GLint location,
              const u_vec4<gl_type::GLuint>& values
            ) const
            {
              c::glUniform4uiv( location, 1, values.data() );
            }

            template<class TMODE, class TMODEL>
            inline void invoke( const TMODEL& m ) const
            {
              m.template invoke<TMODE>();
            }

            inline void invoke( const model& m ) const
            {
              m.invoke();
            }

            inline void flush() const
            {
              c::glFlush();
            }

            inline destruct_invoker flusher() const
            {
              return { [this]{ this->flush(); } };
            }

            static std::shared_ptr<this_type> instance()
            {
              static std::shared_ptr<this_type> i;
              return i;
            }

          private:

            glew()
            {
              initialize();
            }

            void initialize()
            {
              auto r = c::glewInit();

              if ( r != GLEW_OK )
              {
                auto message = std::string( "flewInit fail. code = " ) + std::to_string( r );
                throw std::runtime_error( message );
              }
            }

        };

      }
    }
  }
}