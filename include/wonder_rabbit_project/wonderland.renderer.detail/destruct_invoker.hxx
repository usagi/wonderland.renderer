#pragma once

#include <functional>
#include <memory>

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      class destruct_invoker_t
      {
        std::function<void()> f;
        
      public:
        
        destruct_invoker_t( std::function<void()> && f_ )
          : f( std::move( f_ ) )
        { }
        
        destruct_invoker_t( const destruct_invoker_t& ) = delete;
        
        destruct_invoker_t( destruct_invoker_t && t )
          : f( std::move( t.f ) )
        { t.cancel(); }
        
        ~destruct_invoker_t()
        { f(); }
        
        inline void cancel()
        { f = [] {}; }
        
      };
    }
  }
}