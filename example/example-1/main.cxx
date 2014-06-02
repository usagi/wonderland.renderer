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
  std::cout
    << "[Home]/[End] key or mouse [Wheel]: camera distance forward/backward.\n"
       "[up]/[left]/[right]/[bottom]: control camera phi and theta.\n"
       "[shift] + [Home]/[End] key or mouse [Wheel]: light distance forward/backward.\n"
       "[shift] + [up]/[left]/[right]/[bottom]: control light phi and theta.\n"
       "[q]/[e]: camera eye auto orbitation on/off."
       "[1]: shader program change to Wonderland.Renderer embedded <constant> shader program.\n"
       "[2]: shader program change to Wonderland.Renderer embedded <phong> shader program.\n"
       "[3]: shader program change to Wonderland.Renderer embedded <cartoon> shader program.\n"
       "[9]/[0]: shadow on/off\n"
       "\n"
       "<compiled for>\n"
       "  OpenGL: " << wonder_rabbit_project::wonderland::renderer::shader::gl_version   << "\n"
       "  GLSL  : " << wonder_rabbit_project::wonderland::renderer::shader::glsl_version << "\n"
       "\n"
    ;
  
  using namespace wonder_rabbit_project::wonderland::subsystem;
  using namespace wonder_rabbit_project::wonderland::renderer;
  using wonder_rabbit_project::wonderland::renderer::model::animation_state_t;
  
  constexpr auto screen_width  = 1920 >> 1;
  constexpr auto screen_height = 1080 >> 1;
  
  auto subsystem = std::make_shared< subsystem_t >();
  
  auto ips = subsystem -> default_initialize_params();
  ips.put( "width"  , screen_width  );
  ips.put( "height" , screen_height );
  ips.put( "title"  , "Wonderland.Renderer example-1" );
  ips.put( "samples", 4 ); // FSAA
  subsystem -> initialize( std::move(ips) );
  
  auto renderer = std::make_shared< renderer_t >();
  
  // wonderland.renderer has the standard shader program
  //   effects:
  //     1. create shader program from embedded sources as < shader::constant >.
  //     2. set default program automatically ( you can set `renderer -> default_program( p )` after manually )
  // or you can manually create your shader from source string and link program if you need.
  //   auto my_shader0 = renderer -> create_shader< vertex_shader >( "your source string" );
  //   auto my_program = renderer -> create_program( my_shader0, my_shader1, my_shader2, ... );
  auto program_constant = renderer -> create_program_from_embedded< shader::constant >();
  auto program_phong    = renderer -> create_program_from_embedded< shader::phong    >();
  auto program_cartoon  = renderer -> create_program_from_embedded< shader::cartoon  >();
  
  // create light with effect:
  //  set default lights automatically
  //  ( you can add/remove default light `{add|remove}_lights( light0, light1, light2, ... )` )
  //  ( and `lights( light0, light1, light2, ... )` to set lights or remove all. )
  auto light0 = renderer -> create_light<point_light_t>();

  // load models with assimp currently
  auto model0 = renderer -> create_model( "assets/like_a_cornell_box.x" );
  auto model1 = renderer -> create_model( "assets/vertex-colored-cube.x" );
  auto model2 = renderer -> create_model( "assets/n175Anim.x" );
  
  struct model_instance_state_t
  {
    decltype(model0)  model;
    animation_state_t animation_state;
    glm::mat4         world_transformation;
  };
  
  std::array< model_instance_state_t, 10 > model_instance_states;
  
  {
    model_instance_states[0].model = model0;
    model_instance_states[0].world_transformation
      = glm::scale( glm::mat4(), glm::vec3( 4.0f ) )
      ;
    
    model_instance_states[1].model = model1;
    model_instance_states[1].world_transformation
      = glm::translate( glm::mat4(), glm::vec3( 0.0f, 1.0f, 0.0f ) )
      * glm::scale( glm::mat4(), glm::vec3( 0.5f ) )
      ;
      
    auto animation_names = model2 -> animation_names();
    
    for ( const auto& animation_name : animation_names )
      std::cout << "has animation: " << animation_name << std::endl;
    
    for ( auto n = 2ull; n < model_instance_states.size(); ++n )
    {
      auto& s = model_instance_states[ n ];
      
      s.model = model2;
      
      if ( ! animation_names.empty() )
      {
        s.animation_state.name = animation_names[ n % animation_names.size() ];
        s.animation_state.time = animation_state_t::fseconds( float(n) * 19937 );
      }
      
      s.world_transformation = glm::scale( glm::mat4(), glm::vec3( 1.f ) );
      switch ( n )
      { case 2: s.world_transformation *= glm::translate( glm::mat4(), glm::vec3(  5.0f, 1.0f,  0.0f ) ); break;
        case 3: s.world_transformation *= glm::translate( glm::mat4(), glm::vec3( -5.0f, 0.0f,  0.0f ) ); break;
        case 4: s.world_transformation *= glm::translate( glm::mat4(), glm::vec3(  5.0f, 1.0f,  5.0f ) ); break;
        case 5: s.world_transformation *= glm::translate( glm::mat4(), glm::vec3(  0.0f, 1.0f,  5.0f ) ); break;
        case 6: s.world_transformation *= glm::translate( glm::mat4(), glm::vec3( -5.0f, 1.0f,  5.0f ) ); break;
        case 7: s.world_transformation *= glm::translate( glm::mat4(), glm::vec3(  5.0f, 0.0f, -5.0f ) ); break;
        case 8: s.world_transformation *= glm::translate( glm::mat4(), glm::vec3(  0.0f, 1.0f, -5.0f ) ); break;
        case 9: s.world_transformation *= glm::translate( glm::mat4(), glm::vec3( -5.0f, 1.0f, -5.0f ) );
      }
    }
  }
  
  // clear color
  renderer
    -> clear_color( glm::vec4( 0.8f, 0.8f, 1.0f, 1.0f ) )
    ;
  
  // projection transformation
  renderer
    -> projection()
      //-> fov_y( glm::pi< float >() / 4.0f )
      -> fov_y( glm::pi< float >() / 3.0f )
      -> aspect_ratio( float( screen_width ) / float( screen_height ) )
//#ifdef EMSCRIPTEN
      // the environment(WebGL1, GLES2) is not support gl_FragDepth writable.
      // it cannot use a log depth buffer trick then use small linear clipping range.
      -> near_clip( 1.0e+0f )
      -> far_clip ( 3.0e+1f )
//#else
      // for default, it can use a log depth buffer trick with gl_FragDepth writable.
      // then it can use wide clipping range.
      -> near_clip( 1.0e-1f )
      -> far_clip ( 1.0e+2f )
//#endif
      -> update()
    ;
  
  subsystem -> update_functors.emplace_front
  ( [ subsystem, renderer
    , &model_instance_states
    , light0
    , program_constant
    , program_phong
    , program_cartoon
    ]
  {
    static auto camera_theta           = glm::half_pi< float >();
    static auto camera_phi             = glm::half_pi< float >() * -0.5f;
    static auto camera_distance        = 10.0f;
    static auto camera_auto_orbitation = false;
    
    static auto light_theta           = glm::half_pi< float >();
    static auto light_phi             = glm::half_pi< float >() * -0.5f;
    static auto light_distance        = 10.0f;
    static auto light_auto_orbitation = false;
    
    for ( auto& model_instance_state : model_instance_states )
      model_instance_state.animation_state += animation_state_t::fseconds( 1.0f / 30.0f );
    
    const auto wheel    = subsystem -> pointing_states_wheel();
    const auto key_home = subsystem -> keyboard_state< key::home >();
    const auto key_end  = subsystem -> keyboard_state< key::end  >();
    const auto shift    = subsystem -> keyboard_state< key::left_shift >()
                       || subsystem -> keyboard_state< key::right_shift >()
                       ;
    
    // TODO: for debug
    if ( subsystem -> keyboard_state< key::z >() )
      renderer -> _shadow_save = true;
    
    // camera change
    if      ( wheel.y > 0 or key_home )
      ( shift ? light_distance : camera_distance ) -= 0.2f;
    else if ( wheel.y < 0 or key_end  )
      ( shift ? light_distance : camera_distance ) += 0.2f;
    
    if ( subsystem -> keyboard_state< key::left_arrow >() )
      ( shift ? light_theta : camera_theta ) -= 0.02f;
    if ( subsystem -> keyboard_state< key::right_arrow >() )
      ( shift ? light_theta : camera_theta ) += 0.02f;
    if ( subsystem -> keyboard_state< key::up_arrow >() )
      ( shift ? light_phi : camera_phi ) -= 0.02f;
    if ( subsystem -> keyboard_state< key::down_arrow >() )
      ( shift ? light_phi : camera_phi ) += 0.02f;
    
    renderer -> camera()
      -> target( { 0.0f, 0.0f, 0.0f } )
      -> eye_from_orbit_of_target( camera_theta, camera_phi, camera_distance )
      -> up    ( { 0.0f, 1.0f, 0.0f } )
      ;
    
    // light change
    light0 -> position = ( glm::mat4_cast( glm::quat( glm::vec3( light_phi, light_theta, 0.0f ) ) ) * glm::vec4( 0.0f, 0.0f, light_distance, 1.0f ) ).xyz();
    
    // shadow change
    if ( subsystem -> keyboard_state< key::_9 >() )
      renderer -> shadow( true  );
    if ( subsystem -> keyboard_state< key::_0 >() )
      renderer -> shadow( false );
    
    // program change
    const auto program_change =
    [ renderer, subsystem ]
    ( program_t::const_shared_t program, const key_code_t key, const std::string& name )
      -> bool
    {
      if ( ( renderer -> default_program() != program ) and subsystem -> keyboard_state( key ) )
      {
        renderer -> default_program( program );
        std::cout << "change to Wonderland.Renderer embedded < " << name << " > shader program.\n";
        return true;
      }
      return false;
    };
    
    program_change( program_constant, key::_1, "constant" ) ? 0 :
    program_change( program_phong   , key::_2, "phong"    ) ? 0 :
    program_change( program_cartoon , key::_3, "cartoon"  )
    ;
    
    if ( ( not camera_auto_orbitation ) and subsystem -> keyboard_state< key::q >() )
    {
      camera_auto_orbitation = true;
      std::cout << "camera auto orbitation on.\n";
    }
    else if ( ( camera_auto_orbitation ) and subsystem -> keyboard_state< key::e >() )
    {
      camera_auto_orbitation = false;
      std::cout << "camera auto orbitation off.\n";
    }
    
    if ( camera_auto_orbitation )
      camera_theta += 0.01f;
    if ( light_auto_orbitation )
      light_theta += 0.01f;
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
    , &model_instance_states
    ]
  {
    // invoker effects:
    //  1. clear
    //  2. return { flusher(), use_program( p ) }
    //    use default program if call with no parameter.
    auto rendering_invoker = renderer -> invoker();
    
    // draw model with world transformation
    //  automatic set
    //   1. uniform(world_view_projection_transformation) if shader program has uniform world_view_projection_transformation
    //   2. uniform(world_transformation) if shader program has uniform world_transformation
    for ( const auto& model_instance_state : model_instance_states )
      renderer -> draw( model_instance_state.model , model_instance_state.world_transformation, { model_instance_state.animation_state } );
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