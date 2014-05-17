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
varying float var_texblends[ )" + std::to_string( count_of_textures ) + u8R"( ];

uniform vec3 diffuse;
uniform vec3 ambient;
uniform vec3 specular;
uniform float transparent;
uniform float reflective;
uniform vec3 point_light_position0;
uniform vec3 point_light_color0;
uniform float point_light_constant_attenuation0;
uniform float point_light_linear_attenuation0;
uniform float point_light_quadratic_attenuation0;

uniform sampler2D sampler;

vec4 calc_final_diffuse();
bool is_nan(float);

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
  gl_FragColor.rgb = ambient;
  
  if ( diffuse_factor > 0.0 )
  {
    vec3 view_direction = normalize( -var_position );
    
    // reflrect
    vec3 reflection_direction = reflect( -light0_direction, normal );
    float specular_factor = pow( max( dot( view_direction, reflection_direction ), 0.0000001 ), reflective );
    
    // half
    //vec3 lv_half = normalize( light0_direction + view_direction );
    //float specular_factor = pow( max( dot( normal, lv_half), 0.0000001 ), reflective );
    
    gl_FragColor.rgb += ( calc_final_diffuse().rgb * diffuse_factor + specular * specular_factor ) * attenuation;
  }
  gl_FragColor.a = transparent;
}

vec4 calc_final_diffuse()
{
  vec4 final_diffuse;
  
  if ( ! is_nan( var_color.x ) )
    final_diffuse = var_color;
  else
    final_diffuse = vec4( diffuse, transparent );

  float texblend = 0.0;
  
  vec4 texture_color = vec4( 0.0 );
  
  for ( int n = 0; n < )" + std::to_string( count_of_textures ) + u8R"(; ++n )
    if ( var_texblends[ n ] > 0.0 )
    {
      texblend += var_texblends[ n ];
      texture_color += texture2D( sampler, var_texcoords[ n ] );
    }
  
  if ( texblend > 0.0 )
  {
    final_diffuse *= 1.0 - texblend;
    final_diffuse += texture_color * texblend;
  }
  
  return final_diffuse;
}

bool is_nan(float val)
{
  return (val <= 0.0 || 0.0 <= val) ? false : true;
}
)"