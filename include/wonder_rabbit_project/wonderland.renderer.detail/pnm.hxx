#pragma once

#include <string>
#include <fstream>

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      struct pnm_t
      {
        // default save is .pgm grayscale
        // TODO: add support .pbm and .ppm using template specialize
        template < class T = void >
        static inline auto save( const std::vector<float>& data, unsigned width, const std::string& filename = "out.pgm" )
          -> void
        {
          std::ofstream f( filename );
          
          constexpr auto saturation = 65535u;
          const auto height = unsigned( data.size() / width );
          
          f << "P2\n"
            << width << " " << height << "\n"
            << saturation << "\n"
            ;
          
          for ( auto y = height; y ; --y )
          {
            auto y_ = y - 1;
            for ( auto x = 0u; x < width ; ++x )
              f << unsigned( data[ x + y_ * width ] * saturation ) << " ";
            f << "\n";
          }
        }
      };
    }
  }
}