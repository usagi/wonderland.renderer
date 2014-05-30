u8R"(#version )" + std::to_string( glsl_version ) + u8R"(

#ifdef GL_FRAGMENT_PRECISION_HIGH
  precision highp float;
#else
  precision mediump float;
#endif

)" + IN + u8R"( vec4 var_color;
)" + IN + u8R"( float var_log_z;

)" + OUT_COLOR_DECLARE + u8R"(

void main()
{
  vec3 n;
  n.xy = gl_PointCoord * 2.0 - 1.0;                        // 座標値を [0, 1] → [-1, 1] に変換する
  n.z = 1.0 - dot(n.xy, n.xy);                             // 1 から x と y のそれぞれの二乗和を引く
  if (n.z < 0.0) discard;                                  // 結果が負ならフラグメントを捨てる
  )" + OUT_COLOR + u8R"( = var_color;
  gl_FragDepth = var_log_z;
}

)"