u8R"(#version 100

#ifdef GL_FRAGMENT_PRECISION_HIGH
  precision highp float;
#else
  precision mediump float;
#endif

varying vec4 var_color;
varying vec2 var_texcoords[ )" + std::to_string( count_of_textures ) + u8R"( ];
varying float var_texblends[ )" + std::to_string( count_of_textures ) + u8R"( ];

uniform vec3 diffuse;
uniform vec3 ambient;
uniform float transparent;

uniform sampler2D sampler;

bool is_nan(float);

void main(void)
{
  if ( ! is_nan( var_color.x ) )
    gl_FragColor = var_color;
  else
    gl_FragColor = vec4( ( diffuse + ambient ), transparent );

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
    gl_FragColor *= 1.0 - texblend;
    gl_FragColor += texture_color * texblend;
  }
}

bool is_nan(float val)
{
  return (val <= 0.0 || 0.0 <= val) ? false : true;
}
)"