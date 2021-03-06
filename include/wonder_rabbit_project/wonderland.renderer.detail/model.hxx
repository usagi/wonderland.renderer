#pragma once

#include <boost/gil/gil_all.hpp>

// assimp::Importer
#include <assimp/Importer.hpp>
// aiPostProcessSteps for assimp::Importer::ReadFile
#include <assimp/postprocess.h>
// aiScene, aiNode
#include <assimp/scene.h>

#include "glew.detail/c.hxx"
#include "glew.detail/gl_type.hxx"

#include "model.detail/material.hxx"
#include "model.detail/mesh.hxx"
#include "model.detail/node.hxx"
#include "model.detail/model.hxx"

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      using model_t = model::model_t;
    }
  }
}