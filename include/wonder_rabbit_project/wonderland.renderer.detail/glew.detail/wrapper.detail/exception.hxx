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
          
          struct unsupported_in_platform: public std::runtime_error
          { explicit unsupported_in_platform( const std::string& what = "" ): std::runtime_error( what ){} };
        }
      }
    }
  }
}