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
        class constant final
        { constant() = delete;
        public:
          
          static auto vs_source() -> std::string
          { return 
            #include "constant.hxx.vertex.glsl"
            ;
          }
          
          static auto fs_source() -> std::string
          { return
            #include "constant.hxx.fragment.glsl"
            ;
          }
          
        };
      }
    }
  }
}