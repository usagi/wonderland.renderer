#pragma once

#include "c.hxx"
#include "gl_type.hxx"

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      namespace glew
      {

        struct glew_t;
        struct program;

        template<gl_type::GLuint SHADER_TYPE>
        struct shader
        {
            friend glew_t;
            friend program;
            static constexpr SHADER shader_type = SHADER( SHADER_TYPE );
            using this_type = shader<SHADER_TYPE>;

            shader( this_type && o )
              : _shader( std::move( o._shader ) )
            {
              o.cancel();
            }
            
            shader( const this_type& ) = delete;

            this_type& operator=( this_type && )     = delete;
            this_type& operator=( const this_type& ) = delete;

            inline void cancel()
            {
              _finalizer = [] {};
            }

            inline void source( const std::string& s )
            {
              _source = s;
            }

            inline void source( std::istream && s )
            {
              s.seekg( 0, std::ios::end );
              auto size = s.tellg();
              s.seekg( 0, std::ios::beg );
              _source.resize( size );
              s.read( const_cast<char*>( _source.data() ), size );
            }

            inline const std::string& source() const
            {
              return _source;
            }

            inline void compile( const std::string& v )
            {
              source( v );
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
                constexpr c::GLsizei  count = 1;
                const     c::GLchar*  str   = _source.data();
                const     c::GLint    size  = _source.size();
                c::glShaderSource( _shader, count, &str, &size );
              }
              
              c::glCompileShader( _shader );
              
              {
                c::GLint result = false;
                c::glGetShaderiv( _shader, GL_COMPILE_STATUS, &result );

                if ( ! result )
                {
                  std::string error_log;
                  c::GLint size = 0;
                  c::glGetShaderiv( _shader, GL_INFO_LOG_LENGTH, &size );
                  error_log.resize( size );
                  c::GLint size_written = 0;
                  c::glGetShaderInfoLog(
                    _shader, size, &size_written,
                    const_cast<char*>( error_log.data() )
                  );
                  throw std::runtime_error( error_log );
                }
              }
              
            }

            ~shader()
            {
              _finalizer();
            }
            
          private:
            shader()
              : _shader( c::glCreateShader( gl_type::GLuint( shader_type ) ) )
              , _finalizer
                ( [this]
                  {
                    c::glDeleteShader( this->_shader );
                  }
                )
            { }
            
            gl_type::GLuint _shader;
            std::function<void()> _finalizer;
            std::string _source;
        };

        using vertex_shader          = shader<gl_type::GLuint( SHADER::VERTEX )>;
        using fragment_shader        = shader<gl_type::GLuint( SHADER::FRAGMENT )>;
        using geometry_shader        = shader<gl_type::GLuint( SHADER::GEOMETRY )>;
        using tess_control_shader    = shader<gl_type::GLuint( SHADER::TESS_CONTROL )>;
        using tess_evaluation_shader = shader<gl_type::GLuint( SHADER::TESS_EVALUATION )>;
      }
    }
  }
}