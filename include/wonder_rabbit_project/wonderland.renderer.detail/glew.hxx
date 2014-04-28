#pragma once

#include <memory>
#include <mutex>

#include "glew.detail/c.hxx"
#include "glew.detail/gl_type.hxx"
#include "glew.detail/error.hxx"

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      namespace glew
      {
        struct glew_t final
        {
            using this_type = glew_t;

            glew_t( const this_type& ) = delete;
            glew_t( this_type && )     = delete;
            this_type& operator=( const this_type& ) = delete;
            this_type& operator=( this_type && )     = delete;

            static auto instance() -> this_type*
            {
              static this_type i;
              return &i;
            }

            void initialize()
            {
              auto r = c::glewInit();
              
              if ( r != GLEW_OK )
              {
                auto message = std::string( "glewInit fail. code = " ) + std::to_string( r ) + " : " +  std::string( reinterpret_cast<const char*>( c::glewGetErrorString( r ) ) );
                throw std::runtime_error( message );
              }
            }
            
          private:
            
            glew_t() { }
        };
        
        static auto glew_init() -> void
        {
          static std::mutex m;
          
          if ( not m.try_lock() )
            return;
          
          auto r = c::glewInit();
          if ( r != GLEW_OK )
          {
            auto message = std::string( "glewInit fail. code = " ) + std::to_string( r ) + " : " +  std::string( reinterpret_cast<const char*>( c::glewGetErrorString( r ) ) );
            throw std::runtime_error( message );
          }
        }
        
      }
    }
  }
}