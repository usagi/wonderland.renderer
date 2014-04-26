#pragma once

#include "c.hxx"
#include "gl_type.hxx"

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      namespace glew
      {
        template<class T, class ... TS>
        struct vertex final
        {
            static constexpr size_t elements = 1 + sizeof...( TS );
            //static constexpr size_t size = sizeof(T) + vertex<TS...>::size;
          private:
            template<size_t N, class T_, class ... TS_>
            struct offset_of_
            {
              using type = typename offset_of_ < N - 1, TS_ ... >::type;
              static constexpr size_t add_offset = sizeof( T_ ) + offset_of_ < N - 1, TS_ ... >::add_offset;
            };

            template<class T_, class ... TS_>
            struct offset_of_<0, T_, TS_ ...>
            {
              using type = T_;
              static constexpr size_t add_offset = 0;
            };

          public:
            template<size_t N>
            struct offset_of
            {
              using type = typename offset_of_<N, T, TS ...>::type;
              static constexpr size_t value = offset_of_<N, T, TS ...>::add_offset;
            };

            template<size_t N>
            typename offset_of<N>::type& element_of()
            {
              return *reinterpret_cast<typename offset_of<N>::type*>( &data_[offset_of<N>::value] );
            }

            template<size_t N>
            void element_of( typename offset_of<N>::type && a )
            {
              *reinterpret_cast<typename offset_of<N>::type*>( &data_[offset_of<N>::value] ) = std::move( a );
            }

            template<size_t N>
            void element_of( const typename offset_of<N>::type& a )
            {
              *reinterpret_cast<typename offset_of<N>::type*>( &data_[offset_of<N>::value] ) = a;
            }

            static constexpr size_t size
              = offset_of<sizeof...( TS )>::value
                + sizeof( typename offset_of<sizeof...( TS )>::type )
                ;

          private:
            std::array<uint8_t, size> data_;

          public:
            decltype( data_.data() )   data()   const
            {
              return data_.data();
            }
            decltype( data_.begin() )  begin()
            {
              return data_.begin();
            }
            decltype( data_.end() )    end()
            {
              return data_.end();
            }
            decltype( data_.cbegin() ) cbegin() const
            {
              return data_.cbegin();
            }
            decltype( data_.cend() )   cend()   const
            {
              return data_.cend();
            }

            vertex() { }
            vertex( const T& v0, const TS& ... vs )
            {
              initialize<0, T, TS...>( v0, vs... );
            }

          private:
            template<size_t N> void initialize() const { }

            template<size_t N, class T_, class ... TS_>
            void initialize( const T_& v, const TS_& ... vs )
            {
              element_of<N>( std::move( v ) );

              if ( sizeof...( TS_ ) )
                initialize < N + 1, TS_... > ( vs... );
            }
        };

        template<class T> struct vertex<T> final
        {
            static constexpr size_t elements = 1;
            static constexpr size_t size = sizeof( T );

            template<size_t N>
            struct offset_of
            {
              using type = T;
              static constexpr size_t value = 0;
            };

            vertex() { }
            vertex( const T& v0 ) : data_( v0 ) { }

            const uint8_t* const data()   const
            {
              return reinterpret_cast<uint8_t*>( &data_ );
            }
            const uint8_t*       begin()
            {
              return data_.data();
            }
            const uint8_t*       end()
            {
              return data_.data() + 1;
            }
            const uint8_t* const cbegin() const
            {
              return data_.begin();
            }
            const uint8_t*       cend()   const
            {
              return data_.end();
            }
          private:
            T data_;
        };
      }
    }
  }
}