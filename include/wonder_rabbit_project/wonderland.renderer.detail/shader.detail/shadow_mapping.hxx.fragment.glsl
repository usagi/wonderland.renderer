u8R"(#version 100

#ifdef GL_FRAGMENT_PRECISION_HIGH
  precision highp float;
#else
  precision mediump float;
#endif

void main()
{
  gl_FragColor = vec4( 0.0 );
}

)"