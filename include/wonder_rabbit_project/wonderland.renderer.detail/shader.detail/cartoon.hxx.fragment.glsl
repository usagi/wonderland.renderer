u8R"(#version 100

#ifdef GL_FRAGMENT_PRECISION_HIGH
  precision highp float;
#else
  precision mediump float;
#endif

varying vec3 var_position;
varying vec4 var_color;
varying vec3 var_normal;
varying vec2 var_texcoords[ )" + std::to_string( count_of_textures ) + u8R"( ];

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

uniform sampler2D sampler;

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
  
  float edge_factor = max( dot( view_direction, var_normal ), 0.0 );
  
  float attenuation = 1.0 /
    ( point_light_constant_attenuation0
    + point_light_linear_attenuation0 * light0_distance
    + point_light_quadratic_attenuation0 * light0_distance * light0_distance
    );
  
  float diffuse_factor = dot( light0_direction, normal );
  
  vec4 hsva = vec4( 0.0,  0.0,  0.0,  1.0 );
  
  if ( diffuse_factor > 0.0 )
  {
    // reflrect
    vec3 reflection_direction = reflect( -light0_direction, normal );
    float specular_factor = pow( max( dot( view_direction, reflection_direction ), 0.0000001 ), reflective );
    
    // half
    //vec3 lv_half = normalize( light0_direction + view_direction );
    //float specular_factor = pow( max( dot( normal, lv_half), 0.0000001 ), reflective );
    
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

  vec3 hsv = from_rgb_to_hsv( gl_FragColor.rgb );
  
  hsva.xyz = vec3
  ( float(int( hsva.x * 16.0 )) / 16.0
  , float(int( hsva.y * 16.0 )) / 16.0
  , float(int( hsva.z * 3.0 )) / 3.0
  );

  if ( pow( edge_factor, 4.0 ) < 0.10 )
    hsva.z *= 0.20;
  
  gl_FragColor = from_hsva_to_rgba( hsva );
}


vec3 hsv_add( vec3 a, vec3 b )
{
  if ( b.z == 0.0 )
    return a;
  
  float v_ratio = a.z / b.z;
  
  return vec3( mix( a.x, b.x, v_ratio), mix( a.y, b.y, v_ratio ), a.z + b.z );
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
    result = vec4( from_rgb_to_hsv( var_color.rgb ), var_color.a );
  else
    result = vec4( from_rgb_to_hsv( diffuse ), transparent );
  
  float texblend = 0.0;
  
  vec4 texture_color = vec4( 0.0 );
  
  for ( int n = 0; n < )" + std::to_string( count_of_textures ) + u8R"(; ++n )
    if ( texblends[ n ] > 0.0 )
    {
      texblend += texblends[ n ];
      vec4 sampled_rgba_color = texture2D( sampler, var_texcoords[ n ] );
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