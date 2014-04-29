#pragma once

#include <string>
#include <sstream>
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
          
          auto width() -> int { return _width; }
          auto height() -> int { return _height; }
          auto count_of_pixel_elements() -> int { return _count_of_pixel_elements; }
          
          auto begin() -> decltype( _data.begin() ) { return _data.begin(); }
          auto end() -> decltype( _data.end() ) { return _data.end(); }
          
          auto cbegin() -> decltype( _data.cbegin() ) { return _data.cbegin(); }
          auto cend() -> decltype( _data.cend() ) { return _data.cend(); }
          
          auto data() -> decltype( _data.data() ) { return _data.data(); }
          
          auto to_string() -> std::string
          {
            std::stringstream r;
            
            r << "[stblib::image_loader_t ( " << this << " ) ]\n"
              << "  width() : " << width() << "\n"
              << "  height(): " << height() << "\n"
              << "  count_of_pixel_elements(): " << count_of_pixel_elements() << "\n"
              << "  data(): " << data() << "\n"
              << "  _data.size(): " << _data.size() << "\n"
              ;
            
            return r.str();
          }
        };
        
      }
    }
  }
}