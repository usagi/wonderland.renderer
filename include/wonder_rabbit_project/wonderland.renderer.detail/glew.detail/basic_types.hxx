#pragma once

#include <array>

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      namespace glew
      {
        
        template<class T, size_t N> using vec = std::array<T, N>;

        template<class T>
        union u_vec2
        {
          T                 a[2];
          vec<T, 2>         v_2;
          vec<vec<T, 2>, 1> v_2_1;
          vec<vec<T, 1>, 2> v_1_2;
          const T* data() const
          {
            return a;
          }
        };

        template<class T>
        union u_vec3
        {
          T                 a[3];
          vec<T, 3>         v_3;
          vec<vec<T, 3>, 1> v_3_1;
          vec<vec<T, 1>, 3> v_1_3;
          const T* data() const
          {
            return a;
          }
        };

        template<class T>
        union u_vec4
        {
          T                 a[4];
          vec<T, 4>         v_4;
          vec<vec<T, 4>, 1> v_4_1;
          vec<vec<T, 2>, 2> v_2_2;
          vec<vec<T, 1>, 4> v_1_4;
          const T* data() const
          {
            return a;
          }
        };

        template<class T>
        using u_matrix22 = u_vec4<T>;

        template<class T>
        union u_matrix33
        {
          T                 a[9];
          vec<T, 9>         v_9;
          vec<vec<T, 9>, 1> v_9_1;
          vec<vec<T, 3>, 3> v_3_3;
          vec<vec<T, 1>, 9> v_1_9;
          vec<u_vec3<T>, 3> uv3_3;
          const T* data() const
          {
            return a;
          }
        };

        template<class T>
        union u_matrix44
        {
          T                   a[16];
          vec<T, 16>          v_16;
          vec<vec<T, 16>,  1> v_16_1;
          vec<vec<T,  8>,  2> v_8_2;
          vec<vec<T,  4>,  4> v_4_4;
          vec<vec<T,  2>,  8> v_2_8;
          vec<vec<T,  1>, 16> v_1_16;
          vec<u_vec2<T>, 8>   uv2_8;
          vec<u_vec4<T>, 4>   uv4_4;
          const T* data() const
          {
            return a;
          }
        };
        
      }
    }
  }
}