#pragma once

#include <functional>
#include <memory>

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      namespace glew
      {
        struct destruct_invoker
        {
          destruct_invoker( std::function<void()> && f_ )
            : f( std::move( f_ ) )
          { }
          destruct_invoker( const destruct_invoker& ) = delete;
          destruct_invoker( destruct_invoker && t )
            : destruct_invoker( std::move( t.f ) )
          {
            t.cancel();
          }
          ~destruct_invoker()
          {
            f();
          }
          inline void cancel()
          {
            f = [] {};
          }
          std::function<void()> f;
        };
      }
    }
  }
}