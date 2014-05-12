#pragma once

#include <algorithm>

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
#include "bone.hxx"
#include "animation_data.hxx"

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
          std::string           _name;
          
          std::vector< node_t > _nodes;
          std::vector< int    > _indices;
          glm::mat4             _transformation;
          
          glew::gl_type::GLint _program_id;
          glew::gl_type::GLint _location_of_local_transformation;
          
          glm::mat4 _before_gpu_local_transformation;
          
          const std::unordered_map< std::string, animation_t >& _animations;
          
          inline auto update_current_shader
          ( glew::gl_type::GLint program_id
          , glew::gl_type::GLint location_of_local_transformation
          )
            -> void
          {
            _program_id = program_id;
            _location_of_local_transformation = location_of_local_transformation;
          }
          
          inline auto push_local_transformation()
            -> void
          {
            if ( _location_of_local_transformation not_eq -1 )
            {
              glew::c::glGetUniformfv( _program_id, _location_of_local_transformation, &_before_gpu_local_transformation[0][0] );
              
              const auto current_gpu_local_transformation = _before_gpu_local_transformation * _transformation;
              
              glew::c::glUniformMatrix4fv( _location_of_local_transformation , 1, false, &current_gpu_local_transformation[0][0] );
            }
          }
          
          inline auto pop_local_transformation()
            -> void
          {
            if ( _location_of_local_transformation not_eq -1 )
              glew::c::glUniformMatrix4fv( _location_of_local_transformation , 1, false, &_before_gpu_local_transformation[0][0] );
          }
          
        public:
          
          node_t( const aiNode* node, const std::unordered_map< std::string, animation_t >& animations, const bool transpose_node )
            : _name( node -> mName.C_Str() )
            , _animations( animations )
          {
            _nodes.reserve( node -> mNumMeshes );
            for ( auto n = 0; n < node -> mNumChildren; ++n)
              _nodes.emplace_back( node -> mChildren[ n ], animations, transpose_node );
            
            _indices.reserve( node -> mNumMeshes );
            for ( auto n = 0; n < node -> mNumMeshes; ++n )
              _indices.emplace_back( node -> mMeshes[ n ] );
            
            _transformation = transpose_node
              ? glm::transpose( helper::to_glm_mat4( node -> mTransformation ) )
              : helper::to_glm_mat4( node -> mTransformation )
              ;
          }
          
          auto draw
          ( std::vector< mesh_t >& meshes
          , const animation_states_t& animation_states
          , const glew::gl_type::GLint program_id
          , const glew::gl_type::GLint location_of_local_transformation
          )
            -> void
          {
            update_current_shader( program_id, location_of_local_transformation );
            
            push_local_transformation();
            
            for ( auto& node : _nodes )
              node.draw( meshes, animation_states, program_id, location_of_local_transformation );
            
            for ( auto index : _indices )
              meshes[ index ].draw
              ( animation_states
              , program_id
              );
            
            pop_local_transformation();
          }
          
          auto transformation() const
            -> const glm::mat4&
          { return _transformation; }
          
          auto name() const
            -> const std::string&
          { return _name; }
          
          auto nodes() const
            -> const decltype( _nodes )&
          { return _nodes; }
        };
      }
    }
  }
}