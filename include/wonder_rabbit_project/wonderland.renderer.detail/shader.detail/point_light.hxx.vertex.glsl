u8R"(#version )" + std::to_string( glsl_version ) + u8R"(

)" + IN + u8R"(  vec3 position;
)" + IN + u8R"(  vec3 color;

)" + OUT_POSITION_DECLARE + u8R"(
)" + OUT + u8R"( vec4 var_color;
)" + OUT + u8R"( float var_log_z;

uniform mat4 view_projection_transformation;
uniform float z_log_trick_far;

const float C = 1.0e-3; 

float z_log_trick_calc_log_z ( float, vec4 );
vec4  z_log_trick_apply_log_z( float, vec4 );

void main(void)
{
  vec4 out_position = view_projection_transformation * vec4( vec3( 1000.0, 1.0, 1.0 ), 1.0 );
  
  // log-z trick
  var_log_z                 = z_log_trick_calc_log_z ( z_log_trick_far, out_position );
  )" + OUT_POSITION + u8R"( = z_log_trick_apply_log_z( var_log_z      , out_position );
  
  var_color = vec4( color, 1.0 );
  
  // TODO: appply light attenetion params
  gl_PointSize = 10.0;
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