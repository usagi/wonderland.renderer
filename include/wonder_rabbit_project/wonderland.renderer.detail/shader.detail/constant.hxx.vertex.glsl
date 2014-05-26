u8R"(#version )" + std::to_string( glsl_version ) + u8R"(

//#extension GL_EXT_gpu_shader4 : enable

in vec4 position;
in vec4 color;
in vec2 texcoord0;
//in vec2 texcoord1;
//in vec2 texcoord2;
//in vec2 texcoord3;
//in vec2 texcoord4;
//in vec2 texcoord5;
//in vec2 texcoord6;
//in vec2 texcoord7;
in vec3 tangent;
in vec3 bitangent;
//in ivec4 bone_ids;
in vec4 bone_ids;
in vec4 bone_weights;

out vec4 var_color;
out vec2 var_texcoords[ )" + std::to_string( count_of_textures ) + u8R"( ];
out vec4 var_shadow_position;
out float var_log_z;

uniform mat4 world_view_projection_transformation;
uniform mat4 world_transformation;
uniform mat4 bones[ )" + std::to_string( max_bones ) + u8R"( ];
uniform mat4 shadow_transformation;
uniform float z_log_trick_far;

const float C = 1.0e-3; 

float z_log_trick_calc_log_z ( float, vec4 );
vec4  z_log_trick_apply_log_z( float, vec4 );

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
  gl_Position = world_view_projection_transformation * local_position;
  
  // log-z trick
  var_log_z   = z_log_trick_calc_log_z ( z_log_trick_far, gl_Position );
  gl_Position = z_log_trick_apply_log_z( var_log_z      , gl_Position );
  
  var_color = color;
  
  var_texcoords[ 0 ] = texcoord0;
  //  var_texcoords[ 1 ] = texcoord1;
  //  var_texcoords[ 2 ] = texcoord2;
  //  var_texcoords[ 3 ] = texcoord3;
  //  var_texcoords[ 4 ] = texcoord4;
  //  var_texcoords[ 5 ] = texcoord5;
  //  var_texcoords[ 6 ] = texcoord6;
  //  var_texcoords[ 7 ] = texcoord7;
  
  var_shadow_position = shadow_transformation * world_transformation * local_position;
  //var_shadow_position = z_log_trick( var_shadow_position );
}

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

)"