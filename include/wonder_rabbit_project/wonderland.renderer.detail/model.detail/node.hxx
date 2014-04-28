#pragma once

#include <glm/mat4x4.hpp>

// assimp::Importer
#include <assimp/Importer.hpp>
// aiPostProcessSteps for assimp::Importer::ReadFile
#include <assimp/postprocess.h>
// aiScene, aiNode
#include <assimp/scene.h>

#include "../glew.detail/c.hxx"
#include "../glew.detail/gl_type.hxx"

#include "mesh.hxx"

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      namespace model
      {
        class node_t
        {
          std::vector< node_t > _nodes;
          std::vector< int    > _indices;
          glm::mat4             _transformation;
        public:
          node_t( const aiNode* node )
          {
            _nodes.reserve( node -> mNumMeshes );
            for ( auto n = 0; n < node -> mNumChildren; ++n)
              _nodes.emplace_back( node -> mChildren[ n ] );
            
            _indices.reserve( node -> mNumMeshes );
            for ( auto n = 0; n < node -> mNumMeshes; ++n )
              _indices.emplace_back( node -> mMeshes[ n ] );
            
            // Assimp::aiMatrix4x4t
            //  http://assimp.sourceforge.net/lib_html/classai_matrix4x4t.html
            // glm::mat4
            //  https://github.com/g-truc/glm/blob/0.9.5/glm/detail/type_mat4x4.hpp
            const auto& nt = node -> mTransformation;
            _transformation = glm::mat4
            ( nt[0][0], nt[0][1], nt[0][2], nt[0][3]
            , nt[1][0], nt[1][1], nt[1][2], nt[1][3]
            , nt[2][0], nt[2][1], nt[2][2], nt[2][3]
            , nt[3][0], nt[3][1], nt[3][2], nt[3][3]
            );
          }
          
          auto draw( const std::vector< mesh_t >& meshes ) const
          -> void
          {
            //glew::c::glPushMatrix();
            //glew::c::glMultMatrixf( &_transformation[0][0] );
            
            for ( const auto& node : _nodes )
              node.draw( meshes );
            
            for ( const auto& mesh : meshes )
              mesh.draw();
            
            //glew::c::glPopMatrix();
          }
        };
      }
    }
  }
}