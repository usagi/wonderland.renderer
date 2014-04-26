#pragma once

#include <type_traits>

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
        template<class T>
        class arrayed_informationer final
        {
            template<class T_>
            static constexpr std::integral_constant<size_t, T_().size()> test_count( T_ );
            static constexpr std::integral_constant<size_t, 1          > test_count( ... );
            template<class T_>
            static constexpr typename T_::value_type test_type( T_ );
            static constexpr T                       test_type( ... );
          public:
            static constexpr size_t count = decltype( test_count( std::declval<T>() ) ) :: value;
            using                   type  = decltype( test_type( std::declval<T>() ) );
        };

        template<size_t N, bool TINVOKE, class TVERTEX>
        struct model_invoke_vertex_attribute_pointer_ final
        {
          using vertex_type = TVERTEX;

          static constexpr decltype( N ) index = N;
          static constexpr decltype( N ) next  = index + 1;
          static constexpr bool        invoke_next = next < vertex_type::elements;

          using element_wrapper = typename vertex_type::template offset_of<index>;
          using                   element_type   = typename element_wrapper::type;
          static constexpr size_t element_offset = element_wrapper::value;

          using arrayed_information = arrayed_informationer<element_type>;

          static constexpr size_t     elements   = arrayed_information::count;
          static constexpr gl_type::GLenum type  = gl_type::GLenum( vertex_attribute_from_type<typename arrayed_information::type>::vertex_attribute );
          static constexpr bool       normalized = false;
          static constexpr size_t     size       = vertex_type::size;
          static constexpr void*      offset     = reinterpret_cast<void*>( element_offset );

          static inline void invoke()
          {
            c::glVertexAttribPointer( index, elements, type, normalized, size, offset );
            c::glEnableVertexAttribArray( index );
            model_invoke_vertex_attribute_pointer_<next, invoke_next, vertex_type>::invoke();
          }
        };

        template<size_t N, class TVERTEX>
        struct model_invoke_vertex_attribute_pointer_<N, false, TVERTEX> final
        {
          static inline void invoke() { }
        };

        template<class TVERTEX>
        struct model_invoke_vertex_attribute_pointer final
        {
          static inline void invoke()
          {
            model_invoke_vertex_attribute_pointer_<0, true, TVERTEX>::invoke();
          }
        };

      }
    }
  }
}