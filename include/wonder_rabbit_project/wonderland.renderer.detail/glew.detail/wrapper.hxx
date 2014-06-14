#pragma once

#include "wrapper.detail/uniform.hxx"
#include "wrapper.detail/program.hxx"
#include "wrapper.detail/texture.hxx"
#include "wrapper.detail/frame_buffer.hxx"
#include "wrapper.detail/sampler.hxx"
#include "wrapper.detail/enable.hxx"
#include "wrapper.detail/get.hxx"
#include "wrapper.detail/misc.hxx"
#include "wrapper.detail/viewport.hxx"

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
          , public texture_t
          , public frame_buffer_t
          , public sampler_t
          , public enable_t
          , public get_t
          , public misc_t
          , public viewport_t
        {
        };
      }
    }
  }
}