u8R"(#version )" + std::to_string( glsl_version ) + u8R"(

//#extension GL_EXT_gpu_shader4 : enable

)" + IN + u8R"( vec4 position;
)" + IN + u8R"( vec4 color;
)" + IN + u8R"( vec3 normal;
)" + IN + u8R"( vec2 texcoord0;
//)" + IN + u8R"( vec2 texcoord1;
//)" + IN + u8R"( vec2 texcoord2;
//)" + IN + u8R"( vec2 texcoord3;
//)" + IN + u8R"( vec2 texcoord4;
//)" + IN + u8R"( vec2 texcoord5;
//)" + IN + u8R"( vec2 texcoord6;
//)" + IN + u8R"( vec2 texcoord7;
)" + IN + u8R"( vec3 tangent;
)" + IN + u8R"( vec3 bitangent;
//)" + IN + u8R"( ivec4 bone_ids;
)" + IN + u8R"( vec4 bone_ids;
)" + IN + u8R"( vec4 bone_weights;

)" + OUT + u8R"( vec3 var_position;
)" + OUT + u8R"( vec4 var_color;
)" + OUT + u8R"( vec3 var_normal;
)" + OUT + u8R"( vec2 var_texcoords[ )" + std::to_string( count_of_textures ) + u8R"( ];

// for omnidirectional shadow mapping
)" + OUT + u8R"( vec3 var_shadow_direction;

// for directional shadow mapping
//)" + OUT + u8R"( vec4 var_shadow_position;
#ifdef GL_EXT_frag_depth
)" + OUT + u8R"( float var_log_z;
// for directional shadow mapping
//)" + OUT + u8R"( float var_log_z_shadow;
#endif

uniform mat4 world_view_projection_transformation;
uniform mat4 world_view_transformation;
uniform mat4 world_transformation;
uniform mat4 bones[ )" + std::to_string( max_bones ) + u8R"( ];
uniform mat4 shadow_transformation;
uniform vec3 point_light_position0;
#ifdef GL_EXT_frag_depth
uniform float z_log_trick_far;
const float C = 1.0e-3;

float z_log_trick_calc_log_z ( float, vec4 );
vec4  z_log_trick_apply_log_z( float, vec4 );
#endif

void main(void)
{
  mat4 animation_transformation
    = ( bone_weights[ 0 ] + bone_weights[ 1 ] + bone_weights[ 2 ] +bone_weights[ 3 ] == 0.0 )
    ? mat4( 1.0 )
    : bones[ int( bone_ids[ 0 ] ) ] * bone_weights[ 0 ]
    + bones[ int( bone_ids[ 1 ] ) ] * bone_weights[ 1 ]
    + bones[ int( bone_ids[ 2 ] ) ] * bone_weights[ 2 ]
    + bones[ int( bone_ids[ 3 ] ) ] * bone_weights[ 3 ]
    ;
    
  vec4 local_position = animation_transformation * position;
  vec4 out_position = world_view_projection_transformation * local_position;
  
#ifdef GL_EXT_frag_depth
  // log-z trick
  var_log_z   = z_log_trick_calc_log_z ( z_log_trick_far, out_position );
  gl_Position = z_log_trick_apply_log_z( var_log_z      , out_position );
#else
  gl_Position = out_position;
#endif
  
  var_position = ( world_transformation * local_position ).xyz;
  var_color    = color;
  var_normal   = ( animation_transformation * vec4( normal, 1.0 ) ).xyz;

  var_texcoords[ 0 ] = texcoord0;
  //  var_texcoords[ 1 ] = texcoord1;
  //  var_texcoords[ 2 ] = texcoord2;
  //  var_texcoords[ 3 ] = texcoord3;
  //  var_texcoords[ 4 ] = texcoord4;
  //  var_texcoords[ 5 ] = texcoord5;
  //  var_texcoords[ 6 ] = texcoord6;
  //  var_texcoords[ 7 ] = texcoord7;
  
  // for omnidirectional shadow mapping
  var_shadow_direction = ( world_transformation * local_position ).xyz;
  var_shadow_direction -= point_light_position0;
  var_shadow_direction.xy *= -1.0;
  
  // for directional shadow mapping
  /*
  var_shadow_position = shadow_transformation * world_transformation * local_position;
#ifdef GL_EXT_frag_depth
  var_log_z_shadow    = z_log_trick_calc_log_z ( z_log_trick_far , var_shadow_position );
  var_shadow_position = z_log_trick_apply_log_z( var_log_z_shadow, var_shadow_position );
#endif
  */
}

#ifdef GL_EXT_frag_depth
float z_log_trick_calc_log_z( float far, vec4 position )
{
  float FC = 1.0 / log( far * C + 1.0 );
  return log( position.w * C + 1.0 ) * FC;
}

vec4 z_log_trick_apply_log_z( float log_z, vec4 position )
{
  position.z = ( 2.0 * log_z - 1.0 ) * position.w;
  return position;
}
#endif
)"