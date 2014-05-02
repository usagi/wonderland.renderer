#pragma once

#include <string>
#include <sstream>

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/gtc/quaternion.hpp>

#include <assimp/matrix4x4.h>
#include <assimp/vector3.h>
#include <assimp/quaternion.h>

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      namespace model
      {
        namespace helper
        {

          // Assimp::aiMatrix4x4t
          //  http://assimp.sourceforge.net/lib_html/classai_matrix4x4t.html
          // glm::mat4
          //  https://github.com/g-truc/glm/blob/0.9.5/glm/detail/type_mat4x4.hpp
          template < class T = void >
          inline auto to_glm_mat4( const aiMatrix4x4& in )
            -> glm::mat4
          {
            return
              { in[0][0], in[0][1], in[0][2], in[0][3]
              , in[1][0], in[1][1], in[1][2], in[1][3]
              , in[2][0], in[2][1], in[2][2], in[2][3]
              , in[3][0], in[3][1], in[3][2], in[3][3]
              };
          }
          
          template < class T = void >
          inline auto to_glm_vec4( const aiVector3D* in ) -> glm::vec4
          { return { in -> x, in -> y, in -> z, 1.0f }; }

          template < class T = void >
          inline auto to_glm_vec3( const aiVector3D* in ) -> glm::vec3
          { return { in -> x, in -> y, in -> z }; }
          
          template < class T = void >
          inline auto to_glm_vec2( const aiVector3D* in ) -> glm::vec2
          { return { in -> x, in -> y }; }
          
          template < class T = void >
          inline auto to_glm_quat( const aiQuaternion* in ) -> glm::quat
          { return { in -> w, in -> x, in -> y, in -> z }; }
          
          template < class T = void >
          inline auto to_string( const glm::mat4& m )
            -> std::string
          {
            std::stringstream r;
            
            for ( auto a = 0; a < 4; ++a )
            {
              for ( auto b = 0; b < 4; ++b )
                r << m[a][b] << " ";
              r << "\n";
            }
            
            return r.str();
          }
          
          template < class T >
          inline auto to_string( const T& v )
          -> std::string
          {
            std::stringstream r;
            
            for ( auto a = 0; a < v.length(); ++a )
              r << v[a] << " ";
            r << "\n";
            
            return r.str();
          }
          
        }
      }
    }
  }
}