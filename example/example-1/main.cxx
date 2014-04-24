#include <chrono>
#include <thread>
#include <iostream>
#include <iomanip>
#include <vector>
#include <array>

#ifdef EMSCRIPTEN
  #include <emscripten/emscripten.h>
#endif

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>

#include <wonder_rabbit_project/wonderland.subsystem.hxx>

namespace
{
  auto vs_source() -> std::string
  { return 
      "#version 100\n"
      "attribute vec4 position;\n"
      "void main(void)\n"
      "{\n"
      "  gl_Position = position;\n"
      "}"
      ;
  }
  
  auto fs_source() -> std::string
  { return
      "#version 100\n"
      "void main(void)\n"
      "{\n"
      "  gl_FragColor = vec4( 1., 0., 0., 1. );\n"
      "}"
      ;
  }

  auto build_program(const std::string& vs_source, const std::string& fs_source)
    -> GLuint
  {
    const auto assert_shader = [](GLuint shader, const std::string& message)
    {
      GLint status;
      glGetShaderiv( shader, GL_COMPILE_STATUS, &status );
      if ( not status )
      {
        GLsizei buffer_size;
        glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &buffer_size );
        std::string log;
        if ( buffer_size )
        {
          log.resize( buffer_size );
          glGetShaderInfoLog( shader, buffer_size, nullptr, const_cast<char*>(log.data()) );
        }
        throw std::runtime_error( message + "\n" + log);
      }
    };
    
    const auto assert_program = [](GLuint program, const std::string& message)
    {
      GLint status;
      glGetProgramiv( program, GL_LINK_STATUS, &status );
      if ( not status )
      {
        GLsizei buffer_size;
        glGetProgramiv( program, GL_INFO_LOG_LENGTH, &buffer_size );
        std::string log;
        if ( buffer_size )
        {
          log.resize( buffer_size );
          glGetProgramInfoLog( program, buffer_size, nullptr, const_cast<char*>(log.data()) );
        }
        throw std::runtime_error( message + "\n" + log);
      }
    };
    
    auto vss = vs_source.data();
    const auto vs = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vs, 1, &vss, nullptr );
    glCompileShader( vs );
    assert_shader( vs , "failed to compile vertex shader" );

    auto fss = fs_source.data();
    const auto fs = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fs, 1, &fss, nullptr );
    glCompileShader( fs );
    assert_shader( fs , "failed to compile fragment shader" );
    
    const auto program = glCreateProgram();
    glAttachShader( program, vs );
    glDeleteShader( vs );
    glAttachShader( program, fs );
    glDeleteShader( fs );
    
    glBindAttribLocation( program, 0, "position" );
    glLinkProgram( program );
    assert_program( program, "failed to link program" );
    
    return program;
  }
  
  auto build_vao(const std::vector<std::array<float, 2>>& vertices)
    -> GLuint
  {
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 2 * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    return vao;
  }
}

auto main()
  -> int
  try
{
  using wonder_rabbit_project::wonderland::subsystem::subsystem_t;
  using wonder_rabbit_project::wonderland::subsystem::key;
  
  auto subsystem = std::make_shared<subsystem_t>();
  
  auto ips = subsystem -> default_initialize_params();
  ips.put( "title", "wonderland.subsystem example-2" );
  subsystem -> initialize( std::move(ips) );
  
  std::vector<std::array<float, 2>>
    vertices = 
      { {{ -.5f, -.5f }}
      , {{  .5f, -.5f }}
      , {{  .5f,  .5f }}
      , {{ -.5f,  .5f }}
      };
  
  auto program = build_program( vs_source(), fs_source() );
  auto vao     = build_vao( vertices );
  
  subsystem -> update_functors.emplace_front
  ( []
    {
      
    }
  );
  
  subsystem -> update_functors.emplace_front
    ( [ subsystem ]
      {
        if ( subsystem -> keyboard_state< key::escape >() )
          subsystem -> to_continue( false );
      }
    );
  
  subsystem -> render_functors.emplace_front
  ( [ &vertices, &program, &vao ]
    {
      glClearColor( 0.f, 0.f, 0.f, 0.f );
      glClear( GL_COLOR_BUFFER_BIT );
      glUseProgram( program );
      glBindVertexArray( vao );
      glDrawArrays( GL_LINE_LOOP, 0, vertices.size() );
      glBindVertexArray( 0 );
      glUseProgram( 0 );
    }
  );
  
  subsystem -> render_functors.emplace_front
  ( [ ]
    {
      static auto before = std::chrono::high_resolution_clock::now();
      const auto elapsed = std::chrono::high_resolution_clock::now() - before;
      const auto wait = std::chrono::milliseconds(33) - elapsed;
      std::this_thread::sleep_for(wait);
      before = std::chrono::high_resolution_clock::now();
    }
  );
  
  subsystem -> invoke();
}
catch( const std::exception& e )
{ std::cerr << e.what(); return 1; }
catch( ... )
{ std::cerr << "unknown exception"; return 2; }