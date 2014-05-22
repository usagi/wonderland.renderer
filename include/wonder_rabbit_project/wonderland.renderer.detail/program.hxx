#pragma once

#include <memory>

#include "glew.detail/c.hxx"
#include "glew.detail/gl_type.hxx"
#include "glew.detail/error.hxx"

#include "glew.detail/wrapper.detail/program.hxx"
#include "glew.detail/wrapper.detail/uniform.hxx"

#include "destruct_invoker.hxx"

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      
      class program_t
        : public std::enable_shared_from_this< program_t >
        , protected glew::uniform_t
        , protected glew::program_t
      {
      public:
        using shared_t       = std::shared_ptr<       program_t >;
        using const_shared_t = std::shared_ptr< const program_t >;
        
      private:
        
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
      
        program_t()
          : _program  ( glew::c::glCreateProgram() )
          , _finalizer( [this] { glew::c::glDeleteProgram( this->_program ); } )
        { }
        
        program_t( const program_t& ) = delete;
        program_t( program_t && o )
          : _program  ( std::move( o._program   ) )
          , _finalizer( std::move( o._finalizer ) )
        { }

        program_t& operator=( const program_t& t ) = delete;
        program_t& operator=( program_t && t )
        {
          _program = std::move( t._program );
          _finalizer = std::move( t._finalizer );
          t.cancel();
          return *this;
        }

        auto operator==( const program_t& p ) const
          -> bool
        { return _program == p._program; }
        
        auto operator!=( const program_t& p ) const
          -> bool
        { return not operator==( p ); }
        
        inline auto cancel()
          -> std::shared_ptr< program_t >
        {
          _finalizer = []{};
          return shared_from_this();
        }

        inline auto program_id() const
          -> glew::gl_type::GLuint
        { return _program; }
        
        template<class T>
        auto attach( std::shared_ptr<T> shader_ ) const
          -> const_shared_t
        {
          glew::c::glAttachShader( _program, shader_ -> shader_id() );
          WRP_GLEW_TEST_ERROR
          return shared_from_this();
        }

        template<class T, class ... TS>
        auto attach( std::shared_ptr<T> shader_, TS ... ts ) const
          -> const_shared_t
        {
          attach( shader_ );
          attach( ts ... );
          return shared_from_this();
        }

        auto bind_vs( const glew::gl_type::GLuint index, const std::string& name ) const
          -> const_shared_t
        {
          glew::c::glBindAttribLocation( _program, index, name.data() );
          return shared_from_this();
        }

        template<class ... TS>
        auto bind_vs( const std::string& name, const TS& ... ts ) const
          -> const_shared_t
        {
          _bind_vs( 0, name, ts ... );
          return shared_from_this();
        }

        auto bind_fs( const glew::gl_type::GLuint index, const std::string& name ) const
          -> const_shared_t
        {
          glew::c::glBindFragDataLocation( _program, index, name.data() );
          return shared_from_this();
        }

        template<class ... TS>
        auto bind_fs( const std::string& name, const TS& ... ts ) const
          -> const_shared_t
        {
          _bind_fs( 0, name, ts ... );
          return shared_from_this();
        }

        auto link() const
          -> const_shared_t
        {
          glew::c::glLinkProgram( _program );
          WRP_GLEW_TEST_ERROR
          
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
          
          return shared_from_this();
        }

        auto active_attributes() const
          -> glew::gl_type::GLint
        {
          glew::gl_type::GLint r;
          glew::c::glGetProgramiv( _program, GL_ACTIVE_ATTRIBUTES, &r );
          return r;
        }

        auto bind_vs( const std::string& s ) const
          -> glew::gl_type::GLuint
        {
          auto r = glew::c::glGetAttribLocation( _program, s.data() );
          WRP_GLEW_TEST_ERROR
          return r;
        }

        auto bind_fs( const std::string& s ) const
          -> glew::gl_type::GLuint
        {
          auto r = glew::c::glGetFragDataLocation( _program, s.data() );
          WRP_GLEW_TEST_ERROR
          return r;
        }

        auto uniform( const std::string& name ) const
          -> glew::gl_type::GLuint
        { return glew::uniform_t::uniform( name ); }
        
        template < class T >
        auto uniform( glew::gl_type::GLint location )
          -> T
        { glew::uniform_t::uniform< T >( location ); }
        
        auto use() const
          -> const_shared_t
        {
          glew::program_t::use_program( _program );
          WRP_GLEW_TEST_ERROR
          return shared_from_this();
        }
        
        auto scoped_use() const
          -> destruct_invoker_t
        {
          use();
          return destruct_invoker_t( []{ glew::program_t::use_program(); } );
        }
        
      };
      
    }
  }
}