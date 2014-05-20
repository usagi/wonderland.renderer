#pragma once

#include <memory>

#include "glew.detail/c.hxx"
#include "glew.detail/gl_type.hxx"

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      class program_t;

      template < glew::gl_type::GLuint T_shader_type >
      struct shader_t
        : public std::enable_shared_from_this< shader_t< T_shader_type > >
      {
        friend program_t;
        
        shader_t()
          : _shader( glew::c::glCreateShader( shader_type ) )
          , _finalizer( [this]{ glew::c::glDeleteShader( this->_shader ); } )
        { }
        
        glew::gl_type::GLuint _shader;
        std::function<void()> _finalizer;
        std::string           _source;
    
      public:
        
        static constexpr auto shader_type = T_shader_type;
        
        using this_type = shader_t< T_shader_type >;
        using shared_t  = std::shared_ptr< this_type >;

        shader_t( this_type && o )
          : _shader( std::move( o._shader ) )
        { o.cancel(); }
        
        shader_t( const this_type& ) = delete;

        this_type& operator=( this_type && )     = delete;
        this_type& operator=( const this_type& ) = delete;
        
        ~shader_t()
        { _finalizer(); }
        
        auto cancel()
          -> std::shared_ptr< this_type >
        {
          _finalizer = [] {};
          return this_type::shared_from_this();
        }

        auto source( const std::string& s )
          -> std::shared_ptr< this_type >
        {
          _source = s;
          return this -> shared_from_this();
        }
        
        auto source( std::string&& s )
          -> std::shared_ptr< this_type >
        {
          _source = std::move( s );
          return this -> shared_from_this();
        }
        
        auto source( std::istream && s )
          -> std::shared_ptr< this_type >
        {
          s.seekg( 0, std::ios::end );
          auto size = s.tellg();
          s.seekg( 0, std::ios::beg );
          _source.resize( size );
          s.read( const_cast<char*>( _source.data() ), size );
          return this -> shared_from_this();
        }

        auto source() const
          -> const std::string&
        { return _source; }

        auto compile( const std::string& v )
          -> std::shared_ptr< const this_type >
        {
          return source( v )
            -> compile()
            ;
        }
        
        auto compile( std::string&& v )
          -> std::shared_ptr< const this_type >
        {
          return source( std::move( v ) )
            -> compile()
            ;
        }
        
        auto compile( std::istream && s )
          -> std::shared_ptr< const this_type >
        {
          return source( std::move( s ) )
            -> compile()
            ;
        }

        auto compile() const
          -> std::shared_ptr< const this_type >
        {
          {
            constexpr glew::c::GLsizei  count = 1;
            const     glew::c::GLchar*  str   = _source.data();
            const     glew::c::GLint    size  = _source.size();
            glew::c::glShaderSource( _shader, count, &str, &size );
          }
          
          glew::c::glCompileShader( _shader );
          
          {
            glew::c::GLint result = false;
            glew::c::glGetShaderiv( _shader, GL_COMPILE_STATUS, &result );

            if ( ! result )
            {
              std::string error_log;
              glew::c::GLint size = 0;
              glew::c::glGetShaderiv( _shader, GL_INFO_LOG_LENGTH, &size );
              error_log.resize( size );
              glew::c::GLint size_written = 0;
              glew::c::glGetShaderInfoLog(
                _shader, size, &size_written,
                const_cast<char*>( error_log.data() )
              );
              throw std::runtime_error( error_log );
            }
          }
          
          return this_type::shared_from_this();
        }
      };

#ifdef GL_VERSION_2_0
      using vertex_shader_t          = shader_t< GL_VERTEX_SHADER          >;
      using fragment_shader_t        = shader_t< GL_FRAGMENT_SHADER        >;
#endif
#ifdef GL_VERSION_3_2
      using geometry_shader_t        = shader_t< GL_GEOMETRY_SHADER        >;
#endif
#ifdef GL_VERSION_4_3
      using tess_control_shader_t    = shader_t< GL_TESS_CONTROL_SHADER    >;
      using tess_evaluation_shader_t = shader_t< GL_TESS_EVALUATION_SHADER >;
      using compute_shader_t         = shader_t< GL_COMPUTE_SHADER         >;
#endif
      
    }
  }
}