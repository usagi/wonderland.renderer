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
        
        class cartoon final
        { cartoon() = delete;
        public:
          
          static auto vs_source() -> std::string
          { return 
            #include "cartoon.hxx.vertex.glsl"
            ;
          }
          
          static auto fs_source() -> std::string
          { return
            #include "cartoon.hxx.fragment.glsl"
            ;
          }
          
        };
      }
    }
  }
}