#pragma once

#include "wrapper.detail/uniform.hxx"
#include "wrapper.detail/program.hxx"
#include "wrapper.detail/misc.hxx"

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      namespace glew
      {
        
        struct wrapper_t
          : public uniform_t
          , public program_t
          , public misc_t
        {
        };
      }
    }
  }
}