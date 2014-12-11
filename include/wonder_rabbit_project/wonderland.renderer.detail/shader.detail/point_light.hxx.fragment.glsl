u8R"(#version )" + std::to_string( glsl_version ) + u8R"(

#extension GL_EXT_frag_depth : enable

#ifdef GL_FRAGMENT_PRECISION_HIGH
  precision highp float;
#else
  precision mediump float;
#endif

)" + IN + u8R"( vec3 var_color;
)" + IN + u8R"( float var_log_z;

)" + OUT_COLOR_DECLARE + u8R"(

void main()
{
  vec2  t0 = ( gl_PointCoord.xy * 2.0 - 1.0 ) * 2.0;
  float t1 = sqrt( t0.x * t0.x + t0.y * t0.y );
  float t2 = 1.0 / pow( t1, 13.0 );
  
  )" + OUT_COLOR + u8R"( = vec4( var_color * t2, 1.0 );
  
#ifdef GL_EXT_frag_depth
  gl_FragDepthEXT = var_log_z;
#endif
}

)"