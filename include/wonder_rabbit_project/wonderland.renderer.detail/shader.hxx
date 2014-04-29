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
      struct program_t;

      template < glew::gl_type::GLuint T_shader_type >
      struct shader_t
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

          shader_t( this_type && o )
            : _shader( std::move( o._shader ) )
          { o.cancel(); }
          
          shader_t( const this_type& ) = delete;

          this_type& operator=( this_type && )     = delete;
          this_type& operator=( const this_type& ) = delete;
          
          ~shader_t()
          { _finalizer(); }
          
          inline void cancel()
          { _finalizer = [] {}; }

          inline void source( const std::string& s )
          { _source = s; }
          
          inline void source( std::string&& s )
          { _source = std::move( s ); }
          
          inline void source( std::istream && s )
          {
            s.seekg( 0, std::ios::end );
            auto size = s.tellg();
            s.seekg( 0, std::ios::beg );
            _source.resize( size );
            s.read( const_cast<char*>( _source.data() ), size );
          }

          inline const std::string& source() const
          { return _source; }

          inline void compile( const std::string& v )
          {
            source( v );
            compile();
          }
          
          inline void compile( std::string&& v )
          {
            source( std::move( v ) );
            compile();
          }
          
          inline void compile( std::istream && s )
          {
            source( std::move( s ) );
            compile();
          }

          inline void compile() const
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
            
          }
      };

      using vertex_shader_t          = shader_t< GL_VERTEX_SHADER          >;
      using fragment_shader_t        = shader_t< GL_FRAGMENT_SHADER        >;
      using geometry_shader_t        = shader_t< GL_GEOMETRY_SHADER        >;
      using tess_control_shader_t    = shader_t< GL_TESS_CONTROL_SHADER    >;
      using tess_evaluation_shader_t = shader_t< GL_TESS_EVALUATION_SHADER >;
      
    }
  }
}