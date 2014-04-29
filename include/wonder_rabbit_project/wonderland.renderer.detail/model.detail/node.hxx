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

#include "helper.hxx"
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
            
            _transformation = helper::to_glm_mat4( node -> mTransformation );
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