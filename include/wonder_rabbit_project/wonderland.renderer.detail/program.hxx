#pragma once

#include "glew.detail/c.hxx"
#include "glew.detail/gl_type.hxx"
#include "glew.detail/error.hxx"

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      
      class program_t
      {
          friend class renderer_t;
          
          program_t()
            : _program  ( glew::c::glCreateProgram() )
            , _finalizer( [this] { glew::c::glDeleteProgram( this->_program ); } )
          { }
          
          inline auto attach() const -> void { }
          
          inline auto _bind_vs( size_t ) const -> void { }
          
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
          
          glew::gl_type::GLuint _program;
          std::function<void()> _finalizer;
        
        public:
        
          using this_type = program_t;

          program_t( const this_type& ) = delete;
          program_t( this_type && o )
            : _program  ( std::move( o._program   ) )
            , _finalizer( std::move( o._finalizer ) )
          { }

          this_type& operator=( const this_type& ) = delete;
          this_type& operator=( this_type && )     = delete;

          auto operator==( const program_t& p ) const -> bool
          { return _program == p._program; }
          
          auto operator!=( const program_t& p ) const -> bool
          { return not operator==( p ); }
          
          inline auto cancel() -> void
          { _finalizer = []{}; }

          inline auto program_id() -> glew::gl_type::GLuint
          { return _program; }
          
          template<class T>
          inline void attach( const T& shader_ ) const
          {
            glew::c::glAttachShader( _program, shader_._shader );
            glew::test_error();
          }

          template<class T, class ... TS>
          inline void attach( const T& shader_, const TS& ... ts ) const
          {
            attach( shader_ );
            attach( ts ... );
          }

          inline void bind_vs( const glew::gl_type::GLuint index, const std::string& name )
          const
          {
            glew::c::glBindAttribLocation( _program, index, name.data() );
          }

          template<class ... TS>
          inline void bind_vs( const std::string& name, const TS& ... ts )
          const
          {
            _bind_vs( 0, name, ts ... );
          }

          inline void bind_fs( const glew::gl_type::GLuint index, const std::string& name )
          const
          {
            glew::c::glBindFragDataLocation( _program, index, name.data() );
          }

          template<class ... TS>
          inline void bind_fs( const std::string& name, const TS& ... ts )
          const
          {
            _bind_fs( 0, name, ts ... );
          }

          inline void link() const
          {
            glew::c::glLinkProgram( _program );
            {
              glew::c::GLint result = false;
              glew::c::glGetProgramiv( _program, GL_LINK_STATUS, &result );

              if ( ! result )
              {
                std::string error_log;
                glew::c::GLint size = 0;
                glew::c::glGetProgramiv( _program, GL_INFO_LOG_LENGTH, &size );
                error_log.resize( size );
                glew::c::GLint size_written = 0;
                glew::c::glGetProgramInfoLog(
                  _program, size, &size_written,
                  const_cast<char*>( error_log.data() )
                );
                throw std::runtime_error( error_log );
              }
            }
          }

          inline glew::gl_type::GLint active_attributes() const
          {
            glew::gl_type::GLint r;
            glew::c::glGetProgramiv( _program, GL_ACTIVE_ATTRIBUTES, &r );
            return r;
          }

          inline glew::gl_type::GLuint bind_vs( const std::string& s ) const
          {
            auto r = glew::c::glGetAttribLocation( _program, s.data() );
            glew::test_error();
            return r;
          }

          inline glew::gl_type::GLuint bind_fs( const std::string& s ) const
          {
            auto r = glew::c::glGetFragDataLocation( _program, s.data() );
            glew::test_error();
            return r;
          }

          inline glew::gl_type::GLuint uniform( const std::string& name ) const
          {
            const auto location = glew::c::glGetUniformLocation( _program, name.data() );
            glew::test_error();
            return location;
          }
          
          // tips: template specialization version is exitst on bottom
          template < class T >
          inline auto uniform( glew::gl_type::GLint localtion )
          -> T
          { throw std::logic_error("unsupported data type on uniform(getter)"); }
      };
      
      template < >
      inline auto program_t::uniform < float > ( glew::gl_type::GLint location )
        -> float
      {
        float v;
        glew::c::glGetUniformfv( _program, location, &v );
        return v;
      }
      
#define WRP_TMP( WRP_TMP_X ) \
      template < > \
      inline auto program_t::uniform < WRP_TMP_X > (glew::gl_type::GLint location ) \
        -> WRP_TMP_X \
      { \
        WRP_TMP_X v; \
        glew::c::glGetUniformfv( _program, location, &v.x ); \
        return v; \
      }
      
      WRP_TMP( glm::vec4 )
      WRP_TMP( glm::vec3 )
      WRP_TMP( glm::vec2 )
      WRP_TMP( glm::vec1 )
      
#undef WRP_TMP
      
#define WRP_TMP( WRP_TMP_X ) \
      template < > \
      inline auto program_t::uniform < WRP_TMP_X > (glew::gl_type::GLint location ) \
        -> WRP_TMP_X \
      { \
        WRP_TMP_X v; \
        glew::c::glGetUniformfv( _program, location, &v[0][0] ); \
        return v; \
      }
      
      WRP_TMP( glm::mat4 )
      WRP_TMP( glm::mat3 )
      WRP_TMP( glm::mat2 )
      
#undef WRP_TMP
      
      
#define WRP_TMP( WRP_TMP_X ) \
      template < > \
      inline auto program_t::uniform < WRP_TMP_X > (glew::gl_type::GLint location ) \
        -> WRP_TMP_X \
      { \
        WRP_TMP_X v; \
        glew::c::glGetUniformiv( _program, location, &v.x ); \
        return v; \
      }
      
      WRP_TMP( glm::i32vec4 )
      WRP_TMP( glm::i32vec3 )
      WRP_TMP( glm::i32vec2 )
      WRP_TMP( glm::i32vec1 )
      
#undef WRP_TMP
      
#define WRP_TMP( WRP_TMP_X ) \
      template < > \
      inline auto program_t::uniform < WRP_TMP_X > (glew::gl_type::GLint location ) \
        -> WRP_TMP_X \
      { \
        WRP_TMP_X v; \
        glew::c::glGetUniformuiv( _program, location, &v.x ); \
        return v; \
      }
      
      WRP_TMP( glm::u32vec4 )
      WRP_TMP( glm::u32vec3 )
      WRP_TMP( glm::u32vec2 )
      WRP_TMP( glm::u32vec1 )
      
#undef WRP_TMP
      
#define WRP_TMP( WRP_TMP_X ) \
      template < > \
      inline auto program_t::uniform < WRP_TMP_X > (glew::gl_type::GLint location ) \
        -> WRP_TMP_X \
      { \
        WRP_TMP_X v; \
        glew::c::glGetUniformdv( _program, location, &v.x ); \
        return v; \
      }
      
      WRP_TMP( glm::f64vec4 )
      WRP_TMP( glm::f64vec3 )
      WRP_TMP( glm::f64vec2 )
      WRP_TMP( glm::f64vec1 )
      
#undef WRP_TMP
      
#define WRP_TMP( WRP_TMP_X ) \
      template < > \
      inline auto program_t::uniform < WRP_TMP_X > (glew::gl_type::GLint location ) \
        -> WRP_TMP_X \
      { \
        WRP_TMP_X v; \
        glew::c::glGetUniformdv( _program, location, &v[0][0] ); \
        return v; \
      }
      
      WRP_TMP( glm::f64mat4 )
      WRP_TMP( glm::f64mat3 )
      WRP_TMP( glm::f64mat2 )
      
#undef WRP_TMP
    
    }
  }
}