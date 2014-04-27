#include <chrono>
#include <thread>
#include <iostream>
#include <iomanip>
#include <vector>
#include <array>

#ifdef EMSCRIPTEN
  #include <emscripten/emscripten.h>
#endif

#include <wonder_rabbit_project/wonderland.renderer.hxx>
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
  
  /*
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
  */
}

auto main()
  -> int
  try
{
  using namespace wonder_rabbit_project::wonderland::subsystem;
  using namespace wonder_rabbit_project::wonderland::renderer::glew;
  
  auto subsystem = std::make_shared<subsystem_t>();
  
  auto ips = subsystem -> default_initialize_params();
  ips.put( "title", "wonderland.subsystem example-2" );
  subsystem -> initialize( std::move(ips) );
  
  auto renderer = glew::instance();
  renderer -> initialize();
  
  auto vs = renderer -> create_shader<vertex_shader  >( vs_source() );
  auto fs = renderer -> create_shader<fragment_shader>( fs_source() );
  auto program = renderer -> create_program( vs, fs );
  
  using va_pos2f_tex2f = vertex
  < std::array<float, 2> // position
  , std::array<float, 2> // texcoord
  >;
  
  auto model = renderer -> create_model< va_pos2f_tex2f, usage_static_draw, mode_line_loop >
  ( { { {{ -.5f, -.5f }}, {{0, 0}} }
    , { {{  .5f, -.5f }}, {{1, 0}} }
    , { {{  .5f,  .5f }}, {{1, 1}} }
    , { {{ -.5f,  .5f }}, {{0, 1}} }
    }
  );
  
  renderer -> clear_color( { 0.f, 0.f, 1.f, 0.f } );
  
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
  ( [ renderer, &program, &model /*&vertices, &vao*/ ]
    {
      renderer -> clear();
      auto flusher     = renderer -> flusher();
      auto use_program = renderer -> use_program( program );
      //glBindVertexArray( vao );
      //glDrawArrays( GL_LINE_LOOP, 0, vertices.size() );
      //glBindVertexArray( 0 );
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