#pragma once

#include <functional>
#include <memory>
#include <iostream>

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      class destruct_invoker_t
      {
        std::function< auto () -> void > f;
        
        static std::exception_ptr _pe;
        
      public:
        
        static auto rethrow_if_deferred_exception()
          -> void
        {
          if ( _pe )
          {
            std::cerr << "destruct_invoker_t: detect a deffered thrown exception, rethrow immediate.\n";
            std::rethrow_exception( _pe );
          }
        }
        
        destruct_invoker_t( std::function<void()> && f_ )
          : f( std::move( f_ ) )
        { rethrow_if_deferred_exception(); }
        
        destruct_invoker_t( const destruct_invoker_t& ) = delete;
        
        destruct_invoker_t( destruct_invoker_t && t )
          : f( std::move( t.f ) )
        { t.cancel(); }
        
        ~destruct_invoker_t()
        {
          try
          { f(); }
          catch( ... )
          {
            std::cerr << "destruct_invoker_t: warning, catch an exception in ~destruct_invoker_t(), reserve rethrow in next ctor.\n";
            _pe = std::current_exception();
          }
        }
        
        inline void cancel()
        { f = [] {}; }
        
      };
      
      std::exception_ptr destruct_invoker_t::_pe = nullptr;
      
    }
  }
}