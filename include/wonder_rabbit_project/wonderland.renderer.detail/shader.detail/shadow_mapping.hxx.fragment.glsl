u8R"(#version )" + std::to_string( glsl_version ) + u8R"(

#ifdef GL_FRAGMENT_PRECISION_HIGH
  precision highp float;
#else
  precision mediump float;
#endif

in vec4 var_color;
in vec2 var_texcoords[ )" + std::to_string( count_of_textures ) + u8R"( ];
in float var_log_z;

out vec4 fragment_color;

uniform vec3 diffuse;
uniform vec3 ambient;
uniform vec3 emissive;
uniform float transparent;
uniform float texblends[ )" + std::to_string( count_of_textures ) + u8R"( ];

uniform sampler2D diffuse_sampler;

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
  // in now, color output is not nessesary.
  // but in future, apply alpha to the shadow mapping that is nessesary, maybe.
  vec4 hsva = hsva_calc_diffuse();
  hsva.xyz = hsv_add( hsva.xyz, from_rgb_to_hsv( ambient  ) );
  hsva.xyz = hsv_add( hsva.xyz, from_rgb_to_hsv( emissive ) );
  fragment_color = from_hsva_to_rgba( hsva );
  gl_FragDepth = var_log_z;
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
      vec4 sampled_rgba_color = texture2D( diffuse_sampler, var_texcoords[ n ] );
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