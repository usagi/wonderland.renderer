u8R"(#version )" + std::to_string( glsl_version ) + u8R"(

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
  vec2 t = gl_PointCoord.xy * 2.0 - 1.0;
  
  float distance = sqrt( t.x * t.x + t.y * t.y ) * 3.14159265358979;
  
  //if ( distance > 1.0 )
    //discard;
  
  float inversed_distance = 1.0 / distance;
  
  )" + OUT_COLOR + u8R"( = vec4( var_color * inversed_distance, inversed_distance );
  
  gl_FragDepth = var_log_z;
}

)"