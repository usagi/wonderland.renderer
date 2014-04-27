#pragma once

#include "c.hxx"
#include "gl_type.hxx"
#include "enum/ERROR.hxx"
//#include "program_uniform_delegate.hxx"

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
            friend glew_t;

            using this_type = program;

            program( const this_type& ) = delete;
            program( this_type && o )
              : _program( std::move( o._program ) )
              , _finalizer( std::move( o._finalizer ) )
            { }

            this_type& operator=( const this_type& ) = delete;
            this_type& operator=( this_type && )     = delete;

            inline void cancel()
            {
              _finalizer = [] {};
            }

            template<class T>
            inline void attach( const T& shader_ ) const
            {
              c::glAttachShader( _program, shader_._shader );
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
              c::glBindAttribLocation( _program, index, name.data() );
            }

            template<class ... TS>
            inline void bind_vs( const std::string& name, const TS& ... ts )
            const
            {
              _bind_vs( 0, name, ts ... );
            }

            inline void bind_fs( const gl_type::GLuint index, const std::string& name )
            const
            {
              c::glBindFragDataLocation( _program, index, name.data() );
            }

            template<class ... TS>
            inline void bind_fs( const std::string& name, const TS& ... ts )
            const
            {
              _bind_fs( 0, name, ts ... );
            }

            inline void link() const
            {
              c::glLinkProgram( _program );
              {
                c::GLint result = false;
                c::glGetProgramiv( _program, GL_LINK_STATUS, &result );

                if ( ! result )
                {
                  std::string error_log;
                  c::GLint size = 0;
                  c::glGetProgramiv( _program, GL_INFO_LOG_LENGTH, &size );
                  error_log.resize( size );
                  c::GLint size_written = 0;
                  c::glGetProgramInfoLog(
                    _program, size, &size_written,
                    const_cast<char*>( error_log.data() )
                  );
                  throw std::runtime_error( error_log );
                }
              }
            }

            inline gl_type::GLint active_attributes() const
            {
              gl_type::GLint r;
              c::glGetProgramiv( _program, GL_ACTIVE_ATTRIBUTES, &r );
              return r;
            }

            inline gl_type::GLuint bind_vs( const std::string& s ) const
            {
              auto r = c::glGetAttribLocation( _program, s.data() );
              test_error();
              return r;
            }

            inline gl_type::GLuint bind_fs( const std::string& s ) const
            {
              auto r = c::glGetFragDataLocation( _program, s.data() );
              test_error();
              return r;
            }

            inline gl_type::GLuint uniform( const std::string& name ) const
            {
              const auto location = c::glGetUniformLocation( _program, name.data() );
              test_error();
              return location;
            }
            
            // tips: template specialization version is exitst on bottom
            template < class T >
            inline auto uniform( gl_type::GLint localtion )
            -> T
            { throw std::logic_error("unsupported data type on uniform(getter)"); }

          private:
            program()
              : _program( c::glCreateProgram() )
              , _finalizer( [this] { c::glDeleteProgram( this->_program ); } )
            { }

            inline void attach() const { }

            inline void _bind_vs( size_t ) const {}

            template<class ... TS>
            inline void _bind_vs( size_t index, const std::string& name, const TS& ... ts )
            const
            {
              bind_vs( index, name );
              _bind_vs( index + 1, ts ... );
            }

            inline void _bind_fs( size_t ) const {}

            template<class ... TS>
            inline void _bind_fs( size_t index, const std::string& name, const TS& ... ts )
            const
            {
              bind_fs( index, name );
              _bind_fs( index + 1, ts ... );
            }

            gl_type::GLuint _program;
            std::function<void()> _finalizer;

        };
        
        template < >
        inline auto program::uniform < float > ( gl_type::GLint location )
          -> float
        {
          float v;
          c::glGetUniformfv( _program, location, &v );
          return v;
        }
        
#define WRP_TMP( WRP_TMP_X ) \
        template < > \
        inline auto program::uniform < WRP_TMP_X > (gl_type::GLint location ) \
          -> WRP_TMP_X \
        { \
          WRP_TMP_X v; \
          c::glGetUniformfv( _program, location, &v.x ); \
          return v; \
        }
        
        WRP_TMP( glm::vec4 )
        WRP_TMP( glm::vec3 )
        WRP_TMP( glm::vec2 )
        WRP_TMP( glm::vec1 )
        
#undef WRP_TMP
        
#define WRP_TMP( WRP_TMP_X ) \
        template < > \
        inline auto program::uniform < WRP_TMP_X > (gl_type::GLint location ) \
          -> WRP_TMP_X \
        { \
          WRP_TMP_X v; \
          c::glGetUniformfv( _program, location, &v[0][0] ); \
          return v; \
        }
        
        WRP_TMP( glm::mat4 )
        WRP_TMP( glm::mat3 )
        WRP_TMP( glm::mat2 )
        
#undef WRP_TMP
        
        
#define WRP_TMP( WRP_TMP_X ) \
        template < > \
        inline auto program::uniform < WRP_TMP_X > (gl_type::GLint location ) \
          -> WRP_TMP_X \
        { \
          WRP_TMP_X v; \
          c::glGetUniformiv( _program, location, &v.x ); \
          return v; \
        }
        
        WRP_TMP( glm::i32vec4 )
        WRP_TMP( glm::i32vec3 )
        WRP_TMP( glm::i32vec2 )
        WRP_TMP( glm::i32vec1 )
        
#undef WRP_TMP
        
#define WRP_TMP( WRP_TMP_X ) \
        template < > \
        inline auto program::uniform < WRP_TMP_X > (gl_type::GLint location ) \
          -> WRP_TMP_X \
        { \
          WRP_TMP_X v; \
          c::glGetUniformuiv( _program, location, &v.x ); \
          return v; \
        }
        
        WRP_TMP( glm::u32vec4 )
        WRP_TMP( glm::u32vec3 )
        WRP_TMP( glm::u32vec2 )
        WRP_TMP( glm::u32vec1 )
        
#undef WRP_TMP
        
#define WRP_TMP( WRP_TMP_X ) \
        template < > \
        inline auto program::uniform < WRP_TMP_X > (gl_type::GLint location ) \
          -> WRP_TMP_X \
        { \
          WRP_TMP_X v; \
          c::glGetUniformdv( _program, location, &v.x ); \
          return v; \
        }
        
        WRP_TMP( glm::f64vec4 )
        WRP_TMP( glm::f64vec3 )
        WRP_TMP( glm::f64vec2 )
        WRP_TMP( glm::f64vec1 )
        
#undef WRP_TMP
        
#define WRP_TMP( WRP_TMP_X ) \
        template < > \
        inline auto program::uniform < WRP_TMP_X > (gl_type::GLint location ) \
          -> WRP_TMP_X \
        { \
          WRP_TMP_X v; \
          c::glGetUniformdv( _program, location, &v[0][0] ); \
          return v; \
        }
        
        WRP_TMP( glm::f64mat4 )
        WRP_TMP( glm::f64mat3 )
        WRP_TMP( glm::f64mat2 )
        
#undef WRP_TMP
      
      }
    }
  }
}