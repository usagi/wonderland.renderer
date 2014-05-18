u8R"(#version 100

#ifdef GL_FRAGMENT_PRECISION_HIGH
  precision highp float;
#else
  precision mediump float;
#endif

varying vec4 var_color;
varying vec2 var_texcoords[ )" + std::to_string( count_of_textures ) + u8R"( ];

uniform vec3 diffuse;
uniform vec3 ambient;
uniform vec3 emissive;
uniform float transparent;
uniform float texblends[ )" + std::to_string( count_of_textures ) + u8R"( ];

uniform sampler2D sampler;

vec4 calc_final_diffuse();
bool is_nan( float );

void main( void )
{
  vec4 final_diffuse = calc_final_diffuse();
  gl_FragColor = final_diffuse;
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
    if ( texblends[ n ] > 0.0 )
    {
      texblend += texblends[ n ];
      texture_color += texture2D( sampler, var_texcoords[ n ] );
    }

  if ( texblend > 0.0 )
  {
    final_diffuse *= 1.0 - texblend;
    final_diffuse += texture_color * texblend;
  }

  final_diffuse.rgb += ambient + emissive;
  
  return final_diffuse;
}

bool is_nan(float val)
{
  return (val <= 0.0 || 0.0 <= val) ? false : true;
}
)"