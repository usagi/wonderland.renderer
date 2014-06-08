u8R"(#version )" + std::to_string( glsl_version ) + u8R"(

//#extension GL_EXT_shadow_samplers : enable

#ifdef GL_FRAGMENT_PRECISION_HIGH
  precision highp float;
#else
  precision mediump float;
#endif

)" + IN + u8R"( vec3 var_position;
)" + IN + u8R"( vec4 var_color;
)" + IN + u8R"( vec3 var_normal;
)" + IN + u8R"( vec2 var_texcoords[ )" + std::to_string( count_of_textures ) + u8R"( ];
)" + IN + u8R"( vec4 var_shadow_direction;
//)" + IN + u8R"( vec4 var_shadow_position;
#ifdef GL_EXT_frag_depth
)" + IN + u8R"( float var_log_z;
)" + IN + u8R"( float var_log_z_shadow;
#endif
)" + OUT_COLOR_DECLARE + u8R"(

uniform vec3 diffuse;
uniform vec3 ambient;
uniform vec3 specular;
uniform vec3 emissive;
uniform float transparent;
uniform float reflective;
uniform vec3 point_light_position0;
uniform vec3 point_light_color0;
uniform float point_light_constant_attenuation0;
uniform float point_light_linear_attenuation0;
uniform float point_light_quadratic_attenuation0;
uniform vec3 view_direction;
uniform float texblends[ )" + std::to_string( count_of_textures ) + u8R"( ];
uniform float z_log_trick_far;
const float C = 1.0e-3;

uniform samplerCube shadow_sampler;
uniform sampler2D diffuse_sampler0;

float calc_shadow_ratio_omnidirectional();
//float calc_shadow_ratio_directional();
vec3 hsv_add( vec3, vec3 );
vec4 hsva_add( vec4, vec4 );
vec3 from_rgb_to_hsv( vec3 );
vec3 from_hsv_to_rgb( vec3 );
vec4 from_rgba_to_hsva( vec4 );
vec4 from_hsva_to_rgba( vec4 );
vec4 hsva_calc_diffuse();
bool is_nan( float );

void main(void)
{
  vec3 normal = normalize( var_normal );
  
  vec3 light0_direction = point_light_position0 - var_position;
  float light0_distance = length( light0_direction );
  light0_direction = normalize( light0_direction );
  
  float attenuation = 1.0 /
    ( point_light_constant_attenuation0
    + point_light_linear_attenuation0 * light0_distance
    + point_light_quadratic_attenuation0 * light0_distance * light0_distance
    );
  
  float diffuse_factor = max( dot( light0_direction, normal ), 0.0 );

  vec4 hsva = vec4( 0.0,  0.0,  0.0,  1.0 );
  
  if ( diffuse_factor > 0.0 )
  {
    // reflrect
    vec3 reflection_direction = reflect( -light0_direction, normal );
    float specular_factor = pow( max( dot( view_direction, reflection_direction ), 1.0e-10 ), reflective );
    
    // half
    //vec3 lv_half = normalize( light0_direction + view_direction );
    //float specular_factor = pow( max( dot( normal, lv_half), 1.0e-10 ), reflective );
    
    vec4 hsva_diffuse = hsva_calc_diffuse();
    hsva_diffuse.z *= diffuse_factor;
    
    hsva = hsva_diffuse;
    
    vec3 hsv_specular = from_rgb_to_hsv( specular );
    hsv_specular.z *= specular_factor;
    
    hsva.xyz = hsv_add( hsva.xyz , hsv_specular );
    
    hsva.z *= attenuation;
  }
  
  hsva.xyz = hsv_add( hsva.xyz, from_rgb_to_hsv( ambient  ) );
  hsva.xyz = hsv_add( hsva.xyz, from_rgb_to_hsv( emissive ) );
  
  hsva.z *= calc_shadow_ratio_omnidirectional();
  //hsva.z *= calc_shadow_ratio_directional();
  
  )" + OUT_COLOR + u8R"( = from_hsva_to_rgba( hsva );
  //)" + OUT_COLOR + u8R"( = vec4( vec3( texture( shadow_sampler, normalize(var_shadow_direction.xyz) ).r ), 1.0);
#ifdef GL_EXT_frag_depth
  gl_FragDepth = var_log_z;
#endif
}

float calc_shadow_ratio_omnidirectional()
{
  vec4 shadow_direction = var_shadow_direction;
#ifdef GL_EXT_frag_depth
  //shadow_direction.z = var_log_z_shadow;
#endif

  shadow_direction.y *= -1.0;
  shadow_direction.x *= -1.0;

  vec3 normalized_shadow_direction = normalize( shadow_direction.xyz );
  vec3 abs_normalized_shadow_direction = abs( normalized_shadow_direction );
  
  float fragment_depth = 0.0;
  
  if ( abs_normalized_shadow_direction.x > max( abs_normalized_shadow_direction.y, abs_normalized_shadow_direction.z ) )
    fragment_depth = abs( shadow_direction.x );
  else if ( abs_normalized_shadow_direction.z > max( abs_normalized_shadow_direction.x, abs_normalized_shadow_direction.y ) )
    fragment_depth = abs( shadow_direction.z );
  else
    fragment_depth = abs( shadow_direction.y );
    
  float shadow_depth = texture( shadow_sampler, normalized_shadow_direction ).r;
  
  float FC = 1.0 / log( z_log_trick_far * C + 1.0 );
  fragment_depth = log( fragment_depth * C + 1.0 ) * FC;
  
  return ( fragment_depth > shadow_depth ) ? 0.2 : 1.0;
}

/*
float calc_shadow_ratio_directional()
{
  vec4 shadow_position = var_shadow_position;
#ifdef GL_EXT_frag_depth
  shadow_position.z = var_log_z_shadow;
#endif
  
#if )" + std::to_string( glsl_version_lt( 300 ) ) + u8R"(
  // for legacy environment
  #define textureProj texture2DProj
  float anti_artifact_bias = 1.0e-6;
  float z = shadow_position.z / shadow_position.w;
#else
  // for modern environment
  float anti_artifact_bias = 1.0e-3;
  float z = shadow_position.z;
#endif
  float shadow_multi
    = 0.20 * textureProj( shadow_sampler, shadow_position ).r
    + 0.20 * textureProj( shadow_sampler, vec4( shadow_position.xyz + vec3( -1.0e-2,  1.0e-2, -1.0e-2 ), shadow_position.w ) ).r
    + 0.20 * textureProj( shadow_sampler, vec4( shadow_position.xyz + vec3(  1.0e-2,  1.0e-2,  1.0e-2 ), shadow_position.w ) ).r
    + 0.20 * textureProj( shadow_sampler, vec4( shadow_position.xyz + vec3( -1.0e-2, -1.0e-2,  1.0e-2 ), shadow_position.w ) ).r
    + 0.20 * textureProj( shadow_sampler, vec4( shadow_position.xyz + vec3(  1.0e-2, -1.0e-2, -1.0e-2 ), shadow_position.w ) ).r
    ;
#ifdef textureProj
  #undef textureProj
#endif
  
  float delta = max( z - ( shadow_multi - anti_artifact_bias ), 0.0 );
#ifndef GL_EXT_frag_depth
  // anti too dilute shadow in linear depth space
  delta = sqrt( delta );
#endif
  delta *= 1.0e+3;
  
  return 1.0 - min( delta , 0.80 );
}
*/

vec3 hsv_add( vec3 a, vec3 b )
{
  if ( b.z == 0.0 )
    return a;
  
  float v_ratio = a.z / (a.z + b.z);
  
  return vec3( mix( a.x, b.x, v_ratio), mix( a.y, b.y, 1.0 - v_ratio ), a.z + b.z );
}

vec4 hsva_add( vec4 a, vec4 b )
{
  return vec4( hsv_add( a.xyz, b.xyz ), a.a + b.a );
}

vec3 from_rgb_to_hsv( vec3 rgb )
{
  vec4 K = vec4( 0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0 );
  vec4 p = mix( vec4( rgb.bg, K.wz ), vec4( rgb.gb, K.xy ), step( rgb.b, rgb.g ) );
  vec4 q = mix( vec4( p.xyw, rgb.r ), vec4( rgb.r, p.yzx ), step( p.x, rgb.r ) );

  float d = q.x - min( q.w, q.y );
  float e = 1.0e-10;
  return vec3( abs( q.z + ( q.w - q.y ) / ( 6.0 * d + e ) ), d / ( q.x + e ), q.x );
}

vec3 from_hsv_to_rgb( vec3 hsv )
{
  vec4 K = vec4( 1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0 );
  vec3 p = abs( fract( hsv.xxx + K.xyz ) * 6.0 - K.www );
  return hsv.z * mix( K.xxx, clamp( p - K.xxx, 0.0, 1.0 ), hsv.y );
}

vec4 from_rgba_to_hsva( vec4 rgba )
{
  return vec4( from_rgb_to_hsv( rgba.rgb ), rgba.a );
}

vec4 from_hsva_to_rgba( vec4 hsva )
{
  return vec4( from_hsv_to_rgb( hsva.xyz ), hsva.a );
}

vec4 hsva_calc_diffuse()
{
  vec4 result;
  
  if ( ! is_nan( var_color.x ) )
    result = from_rgba_to_hsva( var_color );
  else
    result = vec4( from_rgb_to_hsv( diffuse ), transparent );
  
  float texblend = 0.0;
  
  vec4 texture_color = vec4( 0.0 );
  
  for ( int n = 0; n < )" + std::to_string( count_of_textures ) + u8R"(; ++n )
    if ( texblends[ n ] > 0.0 )
    {
      texblend += texblends[ n ];
      vec4 sampled_rgba_color = texture2D( diffuse_sampler0, var_texcoords[ n ] );
      vec4 current_blend_color = from_rgba_to_hsva( sampled_rgba_color * texblends[ n ] );
      texture_color = hsva_add( texture_color, current_blend_color );
    }

  if ( texblend > 0.0 )
  {
    result *= 1.0 - texblend;
    result += texture_color * texblend;
  }

  return result;
}

bool is_nan(float val)
{
  return (val <= 0.0 || 0.0 <= val) ? false : true;
}
)"