u8R"(#version )" + std::to_string( glsl_version ) + u8R"(

)" + IN + u8R"( vec3 position;
)" + IN + u8R"( vec3 color;

)" + OUT + u8R"( vec3 var_color;
)" + OUT + u8R"( float var_log_z;

uniform mat4 view_projection_transformation;

#ifdef GL_EXT_frag_depth
uniform float z_log_trick_far;
#endif

uniform float point_size;

#ifdef GL_EXT_frag_depth
const float C = 1.0e-3; 
float z_log_trick_calc_log_z ( float, vec4 );
vec4  z_log_trick_apply_log_z( float, vec4 );
#endif

void main(void)
{
  vec4 out_position = view_projection_transformation * vec4( position, 1.0 );
  
#ifdef GL_EXT_frag_depth
  // log-z trick
  var_log_z   = z_log_trick_calc_log_z ( z_log_trick_far, out_position );
  gl_Position = z_log_trick_apply_log_z( var_log_z      , out_position );
#else
  gl_Position = out_position;
#endif
  
  var_color = color;
  
  // TODO: appply light attenuation params
  gl_PointSize = point_size / gl_Position.w;
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