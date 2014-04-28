#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <stdexcept>

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      namespace stblib
      {
        
        namespace c
        {
          #include "stblib.detail/stb_image.h"
        }
        
        class image_loader_t
        {
          int _width, _height, _count_of_pixel_elements;
          std::vector<std::uint8_t> _data;
        public:
          explicit image_loader_t( const std::string& file_path )
          {
            auto data = c::stbi_load( file_path.data(), &_width, &_height, &_count_of_pixel_elements, 0 );
            
            if ( not data )
              throw std::runtime_error( c::stbi_failure_reason() );
            
            _data = { data, data + _width * _height * _count_of_pixel_elements };
            
            c::stbi_image_free( data );
          }
        };
        
      }
    }
  }
}