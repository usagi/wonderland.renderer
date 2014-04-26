#pragma once

#include "c.hxx"
#include "gl_type.hxx"
#include "program_uniform_delegate.hxx"

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      namespace glew
      {
        
        struct program
        {
            friend glew;

            using this_type = program;

            program( const this_type& ) = delete;
            program( this_type && o )
              : program_( std::move( o.program_ ) )
              , finalizer_( std::move( o.finalizer_ ) )
            { }

            this_type& operator=( const this_type& ) = delete;
            this_type& operator=( this_type && )     = delete;

            inline void cancel()
            {
              finalizer_ = [] {};
            }

            template<class T>
            inline void attach( const T& shader_ ) const
            {
              c::glAttachShader( program_, shader_.shader_ );
              test_error();
            }

            template<class T, class ... TS>
            inline void attach( const T& shader_, const TS& ... ts ) const
            {
              attach( shader_ );
              attach( ts ... );
            }

            inline void bind_vs( const gl_type::GLuint index, const std::string& name )
            const
            {
              c::glBindAttribLocation( program_, index, name.data() );
            }

            template<class ... TS>
            inline void bind_vs( const std::string& name, const TS& ... ts )
            const
            {
              bind_vs_( 0, name, ts ... );
            }

            inline void bind_fs( const gl_type::GLuint index, const std::string& name )
            const
            {
              c::glBindFragDataLocation( program_, index, name.data() );
            }

            template<class ... TS>
            inline void bind_fs( const std::string& name, const TS& ... ts )
            const
            {
              bind_fs_( 0, name, ts ... );
            }

            inline void link() const
            {
              c::glLinkProgram( program_ );
              {
                c::GLint result = false;
                c::glGetProgramiv( program_, GL_LINK_STATUS, &result );

                if ( ! result )
                {
                  std::string error_log;
                  c::GLint size = 0;
                  c::glGetProgramiv( program_, GL_INFO_LOG_LENGTH, &size );
                  error_log.resize( size );
                  c::GLint size_written = 0;
                  c::glGetProgramInfoLog(
                    program_, size, &size_written,
                    const_cast<char*>( error_log.data() )
                  );
                  throw std::runtime_error( error_log );
                }
              }
            }

            inline gl_type::GLint active_attributes() const
            {
              gl_type::GLint r;
              c::glGetProgramiv( program_, GL_ACTIVE_ATTRIBUTES, &r );
              return r;
            }

            inline gl_type::GLuint where_bind_vs( const std::string& s ) const
            {
              auto r = c::glGetAttribLocation( program_, s.data() );

              if ( r == -1 )
              {
                auto message = std::string( "glGetAttribLocation(" )
                               + std::to_string( program_ ) + "," + s + ") return " + std::to_string( r );
                throw std::runtime_error( message );
              }

              return r;
            }

            inline gl_type::GLuint where_bind_fs( const std::string& s ) const
            {
              auto r = c::glGetFragDataLocation( program_, s.data() );

              if ( r == -1 )
              {
                auto message = std::string( "glGetFragDataLocation(" )
                               + std::to_string( program_ ) + "," + s + ") return " + std::to_string( r );
                throw std::runtime_error( message );
              }

              return r;
            }

            inline gl_type::GLuint where_uniform( std::string && name ) const
            {
              return c::glGetUniformLocation( program_, name.data() );
            }

            template<class T, size_t N = 1>
            decltype( program_uniform_delegate<T, N>()( gl_type::GLuint( 0 ), gl_type::GLint( 0 ) ) )
            uniform( gl_type::GLint location )
            {
              return program_uniform_delegate<T, N>()( program_, location );
            }

          private:
            program()
              : program_( c::glCreateProgram() )
              , finalizer_( [this] { c::glDeleteProgram( this->program_ ); } )
            { }

            inline void attach() const { }

            inline void bind_vs_( size_t ) const {}

            template<class ... TS>
            inline void bind_vs_( size_t index, const std::string& name, const TS& ... ts )
            const
            {
              bind_vs( index, name );
              bind_vs_( index + 1, ts ... );
            }

            inline void bind_fs_( size_t ) const {}

            template<class ... TS>
            inline void bind_fs_( size_t index, const std::string& name, const TS& ... ts )
            const
            {
              bind_fs( index, name );
              bind_fs_( index + 1, ts ... );
            }

            gl_type::GLuint program_;
            std::function<void()> finalizer_;

        };
      }
    }
  }
}