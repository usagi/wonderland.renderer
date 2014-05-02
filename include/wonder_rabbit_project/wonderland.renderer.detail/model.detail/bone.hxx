#pragma once

#include <stdexcept>
#include <chrono>
#include <list>

#include <glm/vec4.hpp>

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      namespace model
      {
        struct vertex_bone_t
        {
          glm::vec4 bone_ids;
          glm::vec4 bone_weights;
          
          vertex_bone_t()
            : bone_ids    ( 0.0f )
            , bone_weights( 0.0f )
          { }
          
          auto push( float id, float weight ) -> void
          {
            constexpr auto max_count = 4;
            
            for ( auto n = 0; n < max_count; ++n )
              if ( bone_weights[ n ] == 0.0f )
              {
                bone_ids    [ n ] = id;
                bone_weights[ n ] = weight;
                return;
              }
            
            throw std::runtime_error( "count of bones is not enought. need count <= 4 or fix up the wonderland.renderer." );
          }
        };
        
        using vertex_bones_t = std::vector<vertex_bone_t>;
      }
    }
  }
}