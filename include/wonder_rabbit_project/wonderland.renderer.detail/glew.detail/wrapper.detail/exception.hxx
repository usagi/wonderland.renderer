#pragma once

#include <stdexcept>

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      namespace glew
      {
        namespace exception
        {
          struct program_is_not_set: public std::runtime_error
          { explicit program_is_not_set( const std::string& what = "" ): std::runtime_error( what ){} };
        }
      }
    }
  }
}