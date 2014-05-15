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

auto main()
-> int
try
{
  std::cout << "Controll camera: Home/End key or Wheel.\n";
  
  using namespace wonder_rabbit_project::wonderland::subsystem;
  using namespace wonder_rabbit_project::wonderland::renderer;
  using wonder_rabbit_project::wonderland::renderer::model::animation_state_t;
  
  constexpr auto screen_width  = 1920 >> 1;
  constexpr auto screen_height = 1080 >> 1;
  
  auto subsystem = std::make_shared<subsystem_t>();
  
  auto ips = subsystem -> default_initialize_params();
  ips.put( "width"  , screen_width  );
  ips.put( "height" , screen_height );
  ips.put( "title"  , "Wonderland.Renderer example-1" );
  ips.put( "samples", 4 ); // FSAA
  subsystem -> initialize( std::move(ips) );
  
  auto renderer = std::make_shared<renderer_t>();
  
  // wonderland.renderer has the standard shader program
  //   effects:
  //     1. create default shader program
  //     2. set default program automatically ( you can set `renderer -> default_program( p )` after manually )
  // or you can manually create your shader from source string and link program if you need.
  //   auto my_shader0 = renderer -> create_shader< vertex_shader >( "your source string" );
  //   auto my_program = renderer -> create_program( my_shader0, my_shader1, my_shader2, ... );
  auto program = renderer -> create_program_standard();
  
  // create light with effect:
  //  set default lights automatically
  //  ( you can add/remove default light `{add|remove}_lights( light0, light1, light2, ... )` )
  //  ( and `lights( light0, light1, light2, ... )` to set lights or remove all. )
  auto light0 = renderer -> create_light<point_light_t>();
  
  // load models with assimp currently
  auto model0 = renderer -> create_model( "assets/vertex-colored-cube.x" );
  auto model1 = renderer -> create_model( "assets/n175Anim.x" );
  
  struct model_instance_state_t
  {
    decltype(model0)* pmodel = nullptr;
    animation_state_t animation_state;
    glm::mat4         world_transformation;
  };
  
  std::array< model_instance_state_t, 9 > model_instance_states;
  
  {
    model_instance_states[0].pmodel = &model0;
    model_instance_states[0].world_transformation
      = glm::scale( glm::mat4(), glm::vec3( 0.1f ) );
      
    auto animation_names = model1.animation_names();
    
    for ( const auto& animation_name : animation_names )
      std::cout << "has animation: " << animation_name << std::endl;
    
    for ( auto n = 1; n < model_instance_states.size(); ++n )
    {
      auto& s = model_instance_states[ n ];
      
      s.pmodel = &model1;
      
      if ( ! animation_names.empty() )
      {
        s.animation_state.name = animation_names[ n % animation_names.size() ];
        s.animation_state.time = animation_state_t::fseconds( float(n) * 19937 );
      }
      
      s.world_transformation = glm::scale( glm::mat4(), glm::vec3( 25.f ) );
      switch ( n )
      { case 1: s.world_transformation *= glm::translate( glm::mat4(), glm::vec3(  5.0f, 0.0f,  0.0f ) ); break;
        case 2: s.world_transformation *= glm::translate( glm::mat4(), glm::vec3( -5.0f, 0.0f,  0.0f ) ); break;
        case 3: s.world_transformation *= glm::translate( glm::mat4(), glm::vec3(  5.0f, 0.0f,  5.0f ) ); break;
        case 4: s.world_transformation *= glm::translate( glm::mat4(), glm::vec3(  0.0f, 0.0f,  5.0f ) ); break;
        case 5: s.world_transformation *= glm::translate( glm::mat4(), glm::vec3( -5.0f, 0.0f,  5.0f ) ); break;
        case 6: s.world_transformation *= glm::translate( glm::mat4(), glm::vec3(  5.0f, 0.0f, -5.0f ) ); break;
        case 7: s.world_transformation *= glm::translate( glm::mat4(), glm::vec3(  0.0f, 0.0f, -5.0f ) ); break;
        case 8: s.world_transformation *= glm::translate( glm::mat4(), glm::vec3( -5.0f, 0.0f, -5.0f ) );
      }
    }
  }
  
  // clear color
  {
    const auto color = glm::vec4( 0.8f, 0.8f, 1.0f, 1.0f );
    renderer -> clear_color( color );
  }
  
  // projection transformation
  {
    const auto fov_y = glm::pi<float>() / 4.0f;
    constexpr auto screen_aspect_ratio = float( screen_width ) / float( screen_height );
    constexpr auto near_clip =    0.1f;
    constexpr auto far_clip  = 1000.0f;
    renderer -> projection_transformation( glm::perspective( fov_y, screen_aspect_ratio, near_clip, far_clip ) );
  }
  
  subsystem -> update_functors.emplace_front
  ( [ subsystem, renderer, &model_instance_states, &light0 ]
  {
    static auto counter = 0.0f;
    
    static auto distance = 300.0f;
    
    for ( auto& model_instance_state : model_instance_states )
      model_instance_state.animation_state += animation_state_t::fseconds( 1.0f / 30.0f );
    
    const auto wheel = subsystem -> pointing_states_wheel();
    const auto key_home = subsystem -> keyboard_state< key::home >();
    const auto key_end  = subsystem -> keyboard_state< key::end  >();
    
    if ( wheel.y > 0 or key_home )
      distance -= 10.0f;
    else if ( wheel.y < 0 or key_end )
      distance += 10.0f;
    
    // camera as view transformation generator
    auto& camera = renderer -> camera();
    camera.eye   ( { distance * std::sin( counter ), 100.0f, distance * std::cos( counter ) } );
    camera.target( { 0.0f, 25.0f, 0.0f } );
    camera.up    ( { 0.0f,  1.0f, 0.0f } );
    
    light0.position = glm::vec3( distance * std::sin( counter ), distance * std::cos( counter ), distance );
    
    counter += 0.01f;
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
  ( [ renderer
  , &program
  , &model_instance_states
  ]
  {
    // invoker effects:
    //  1. clear
    //  2. return { flusher(), use_program( p ) }
    //    use default program if call with no parameter.
    auto rendering_invoker = renderer -> invoke();
    
    // draw model with world transformation
    //  automatic set
    //   1. uniform(world_view_projection_transformation) if shader program has uniform world_view_projection_transformation
    //   2. uniform(world_transformation) if shader program has uniform world_transformation
    
    for ( const auto& model_instance_state : model_instance_states )
      renderer -> draw( *model_instance_state.pmodel , model_instance_state.world_transformation, { model_instance_state.animation_state } );
  }
  );
  
  subsystem -> render_functors.emplace_front
  ( [ ]
  {
    static auto before = std::chrono::high_resolution_clock::now();
    const auto elapsed = std::chrono::high_resolution_clock::now() - before;
    const auto wait = std::chrono::duration<float>( 1.0f / 30.0f ) - elapsed;
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