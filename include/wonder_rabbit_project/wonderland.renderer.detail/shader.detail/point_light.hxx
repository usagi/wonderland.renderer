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
        class point_light final
        { point_light() = delete;
        public:
          
          static auto vs_source() -> std::string
          {
            using namespace pseudo_pre_processor::vertex_shader;
            return 
            #include "point_light.hxx.vertex.glsl"
            ;
          }
          
          static auto fs_source() -> std::string
          {
            using namespace pseudo_pre_processor::fragment_shader;
            return
            #include "point_light.hxx.fragment.glsl"
            ;
          }
          
        };
      }
    }
  }
}