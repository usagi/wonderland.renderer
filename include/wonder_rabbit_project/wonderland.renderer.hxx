#pragma once

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtx/vec1.hpp>

#include "wonderland.renderer.detail/glew.hxx"

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      using namespace glew;
      using renderer_t = glew_t;
    }
  }
}