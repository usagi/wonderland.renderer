u8R"(#version )" + std::to_string( glsl_version ) + u8R"(

//#extension GL_EXT_shadow_samplers : enable

#ifdef GL_FRAGMENT_PRECISION_HIGH
  precision highp float;
#else
  precision mediump float;
#endif

)" + IN + u8R"( vec4 var_color;
)" + IN + u8R"( vec2 var_texcoords[ )" + std::to_string( count_of_textures ) + u8R"( ];
)" + IN + u8R"( vec4 var_shadow_position;
#ifdef GL_EXT_frag_depth
)" + IN + u8R"( float var_log_z;
)" + IN + u8R"( float var_log_z_shadow;
#endif
)" + OUT_COLOR_DECLARE + u8R"(

uniform vec3 diffuse;
uniform vec3 ambient;
uniform vec3 emissive;
uniform float transparent;
uniform float texblends[ )" + std::to_string( count_of_textures ) + u8R"( ];

uniform sampler2D shadow_sampler;
uniform sampler2D diffuse_sampler0;

float calc_shadow_ratio();
vec3 hsv_add( vec3, vec3 );
vec4 hsva_add( vec4, vec4 );
vec3 from_rgb_to_hsv( vec3 );
vec3 from_hsv_to_rgb( vec3 );
vec4 from_rgba_to_hsva( vec4 );
vec4 from_hsva_to_rgba( vec4 );
vec4 hsva_calc_diffuse();
bool is_nan( float );

void main()
{
  vec4 hsva = hsva_calc_diffuse();
  hsva.xyz = hsv_add( hsva.xyz, from_rgb_to_hsv( ambient  ) );
  hsva.xyz = hsv_add( hsva.xyz, from_rgb_to_hsv( emissive ) );
  
  hsva.z *= calc_shadow_ratio();
  
  )" + OUT_COLOR + u8R"( = from_hsva_to_rgba( hsva );
#ifdef GL_EXT_frag_depth
  gl_FragDepth = var_log_z;
#endif
}

float calc_shadow_ratio()
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
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = rgb.g < rgb.b ? vec4(rgb.bg, K.wz) : vec4(rgb.gb, K.xy);
    vec4 q = rgb.r < p.x ? vec4(p.xyw, rgb.r) : vec4(rgb.r, p.yzx);

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
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