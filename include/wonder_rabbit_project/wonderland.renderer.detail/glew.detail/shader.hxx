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

        struct glew;
        struct program;

        template<gl_type::GLuint SHADER_TYPE>
        struct shader
        {
            friend glew;
            friend program;
            static constexpr SHADER shader_type = SHADER( SHADER_TYPE );
            using this_type = shader<SHADER_TYPE>;

            shader( this_type && o )
              : shader_( std::move( o.shader_ ) )
            {
              o.cancel();
            }
            shader( const this_type& ) = delete;

            this_type& operator=( this_type && )     = delete;
            this_type& operator=( const this_type& ) = delete;

            inline void cancel()
            {
              finalizer_ = [] {};
            }

            inline void source( const std::string& v )
            {
              source_ = std::move( v );
            }

            inline void source( std::istream && s )
            {
              s.seekg( 0, std::ios::end );
              auto size = s.tellg();
              s.seekg( 0, std::ios::beg );
              source_.resize( size );
              s.read( const_cast<char*>( source_.data() ), size );
            }

            inline const std::string& source() const
            {
              return source_;
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
                const     c::GLchar*  str   = source_.data();
                const     c::GLint    size  = source_.size();
                c::glShaderSource( shader_, count, &str, &size );
              }
              
              c::glCompileShader( shader_ );
              
              {
                c::GLint result = false;
                c::glGetShaderiv( shader_, GL_COMPILE_STATUS, &result );

                if ( ! result )
                {
                  std::string error_log;
                  c::GLint size = 0;
                  c::glGetShaderiv( shader_, GL_INFO_LOG_LENGTH, &size );
                  error_log.resize( size );
                  c::GLint size_written = 0;
                  c::glGetShaderInfoLog(
                    shader_, size, &size_written,
                    const_cast<char*>( error_log.data() )
                  );
                  throw std::runtime_error( error_log );
                }
              }
              
            }

            ~shader()
            {
              finalizer_();
            }
            
          private:
            shader()
              : shader_( c::glCreateShader( gl_type::GLuint( shader_type ) ) )
              , finalizer_
                ( [this]
                  {
                    c::glDeleteShader( this->shader_ );
                  }
                )
            { }

            gl_type::GLuint shader_;
            std::function<void()> finalizer_;
            std::string source_;
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