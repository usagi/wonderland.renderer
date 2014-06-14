#pragma once

#include <functional>
#include <memory>
#include <iostream>

#include <boost/optional.hpp>

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      class destruct_invoker_t
      {
        std::function< auto () -> void > f;
        
#ifdef EMSCRIPTEN
        static boost::optional< std::exception > _pe;
#else
        static std::exception_ptr _pe;
#endif
        
      public:
        
        static auto rethrow_if_deferred_exception()
          -> void
        {
          if ( _pe )
          {
            std::cerr << "destruct_invoker_t: detect a deffered thrown exception, rethrow immediate.\n";
#ifdef EMSCRIPTEN
            throw _pe;
#else
            std::rethrow_exception( _pe );
#endif
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
#ifdef EMSCRIPTEN
          catch( const std::exception& e )
          {
            std::cerr
              << "destruct_invoker_t: warning, catch an exception in ~destruct_invoker_t(), but emscripten is not support exception_ptr then reserve pseudo-rethrow in next ctor.\n"
              << "exception type: " << typeid( e ).name() << "\n"
              << "exception what: " << e.what() << "\n"
              ;
            _pe = std::exception( e );
          }
          catch( ... )
          {
            std::cerr << "destruct_invoker_t: warning, catch a non std::exception in ~destruct_invoker_t(), but emscripten is not support exception_ptr then reserve pseudo-rethrow in next ctor.\n";
            _pe = std::exception();
          }
#else
          catch( ... )
          {
            std::cerr << "destruct_invoker_t: warning, catch an exception in ~destruct_invoker_t(), reserve rethrow in next ctor.\n";
            _pe = std::current_exception();
          }
#endif
        }
        
        inline void cancel()
        { f = [] {}; }
        
      };
      
#ifdef EMSCRIPTEN
      boost::optional< std::exception > destruct_invoker_t::_pe = boost::none;
#else
      std::exception_ptr destruct_invoker_t::_pe = nullptr;
#endif
      
    }
  }
}