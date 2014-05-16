#pragma once

#include <string>

#include "misc.hxx"

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      namespace shader
      {
        
        class phong final
        { phong() = delete;
        public:
          
          static constexpr unsigned count_of_textures = 1;
          
          static auto vs_source() -> std::string
          { return 
            #include "phong.hxx.vertex.glsl"
            ;
          }
          
          static auto fs_source() -> std::string
          { return
            #include "phong.hxx.fragment.glsl"
            ;
          }
          
        };
      }
    }
  }
}