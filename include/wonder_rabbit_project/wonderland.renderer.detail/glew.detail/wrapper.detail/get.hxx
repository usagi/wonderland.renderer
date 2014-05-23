#pragma once

#include <cstdint>
#include <stdexcept>
#include <sstream>
#include <typeinfo>

#define GLM_SWIZZLE
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/vec1.hpp>

#include "../c.hxx"
#include "../gl_type.hxx"

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      namespace glew
      {
        template < typename gl_type::GLenum T_pname, class T >
        struct get_impl_t
        { static inline auto invoke() -> T; };
        
        // bool
        
        template < typename gl_type::GLenum T_pname >
        struct get_impl_t < T_pname, bool >
        {
          using t = bool;
          
          static inline auto invoke() -> t
          { t r; c::glGetBooleanv( T_pname, reinterpret_cast< gl_type::GLboolean* >( &r ) ); return r; }
        };
        
        template < typename gl_type::GLenum T_pname >
        struct get_impl_t < T_pname, glm::bvec1 >
        {
          using t = glm::bvec1;
          
          static inline auto invoke() -> t
          { t r; c::glGetBooleanv( T_pname, reinterpret_cast< gl_type::GLboolean* >( &r[0] ) ); return r; }
        };
        
        template < typename gl_type::GLenum T_pname >
        struct get_impl_t < T_pname, glm::bvec2 >
        {
          using t = glm::bvec2;
          
          static inline auto invoke() -> t
          { t r; c::glGetBooleanv( T_pname, reinterpret_cast< gl_type::GLboolean* >( &r[0] ) ); return r; }
        };
        
        template < typename gl_type::GLenum T_pname >
        struct get_impl_t < T_pname, glm::bvec3 >
        {
          using t = glm::bvec3;
          
          static inline auto invoke() -> t
          { t r; c::glGetBooleanv( T_pname, reinterpret_cast< gl_type::GLboolean* >( &r[0] ) ); return r; }
        };
        
        template < typename gl_type::GLenum T_pname >
        struct get_impl_t < T_pname, glm::bvec4 >
        {
          using t = glm::bvec4;
          
          static inline auto invoke() -> t
          { t r; c::glGetBooleanv( T_pname, reinterpret_cast< gl_type::GLboolean* >( &r[0] ) ); return r; }
        };
        
        // double
        
        template < typename gl_type::GLenum T_pname >
        struct get_impl_t < T_pname, gl_type::GLdouble >
        {
          using t = gl_type::GLdouble;
          
          static inline auto invoke() -> t
          { t r; c::glGetDoublev( T_pname, &r ); return r; }
        };
        
        template < typename gl_type::GLenum T_pname >
        struct get_impl_t < T_pname, glm::f64vec1 >
        {
          using t = glm::f64vec1;
          
          static inline auto invoke() -> t
          { t r; c::glGetDoublev( T_pname, &r[0] ); return r; }
        };
        
        template < typename gl_type::GLenum T_pname >
        struct get_impl_t < T_pname, glm::f64vec2 >
        {
          using t = glm::f64vec2;
          
          static inline auto invoke() -> t
          { t r; c::glGetDoublev( T_pname, &r[0] ); return r; }
        };
        
        template < typename gl_type::GLenum T_pname >
        struct get_impl_t < T_pname, glm::f64vec3 >
        {
          using t = glm::f64vec3;
          
          static inline auto invoke() -> t
          { t r; c::glGetDoublev( T_pname, &r[0] ); return r; }
        };
        
        template < typename gl_type::GLenum T_pname >
        struct get_impl_t < T_pname, glm::f64vec4 >
        {
          using t = glm::f64vec4;
          
          static inline auto invoke() -> t
          { t r; c::glGetDoublev( T_pname, &r[0] ); return r; }
        };
        
        // float
        
        template < typename gl_type::GLenum T_pname >
        struct get_impl_t < T_pname, gl_type::GLfloat >
        {
          using t = gl_type::GLfloat;
          
          static inline auto invoke() -> t
          { t r; c::glGetFloatv( T_pname, &r ); return r; }
        };
        
        template < typename gl_type::GLenum T_pname >
        struct get_impl_t < T_pname, glm::vec1 >
        {
          using t = glm::vec1;
          
          static inline auto invoke() -> t
          { t r; c::glGetFloatv( T_pname, &r[0] ); return r; }
        };
        
        template < typename gl_type::GLenum T_pname >
        struct get_impl_t < T_pname, glm::vec2 >
        {
          using t = glm::vec2;
          
          static inline auto invoke() -> t
          { t r; c::glGetFloatv( T_pname, &r[0] ); return r; }
        };
        
        template < typename gl_type::GLenum T_pname >
        struct get_impl_t < T_pname, glm::vec3 >
        {
          using t = glm::vec3;
          
          static inline auto invoke() -> t
          { t r; c::glGetFloatv( T_pname, &r[0] ); return r; }
        };
        
        template < typename gl_type::GLenum T_pname >
        struct get_impl_t < T_pname, glm::vec4 >
        {
          using t = glm::vec4;
          
          static inline auto invoke() -> t
          { t r; c::glGetFloatv( T_pname, &r[0] ); return r; }
        };
        
        // int
        
        template < typename gl_type::GLenum T_pname >
        struct get_impl_t < T_pname, gl_type::GLint >
        {
          using t = gl_type::GLint;
          
          static inline auto invoke() -> t
          { t r; c::glGetIntegerv( T_pname, &r ); return r; }
        };
        
        template < typename gl_type::GLenum T_pname >
        struct get_impl_t < T_pname, glm::i32vec1 >
        {
          using t = glm::i32vec1;
          
          static inline auto invoke() -> t
          { t r; c::glGetIntegerv( T_pname, &r[0] ); return r; }
        };
        
        template < typename gl_type::GLenum T_pname >
        struct get_impl_t < T_pname, glm::i32vec2 >
        {
          using t = glm::i32vec2;
          
          static inline auto invoke() -> t
          { t r; c::glGetIntegerv( T_pname, &r[0] ); return r; }
        };
        
        template < typename gl_type::GLenum T_pname >
        struct get_impl_t < T_pname, glm::i32vec3 >
        {
          using t = glm::i32vec3;
          
          static inline auto invoke() -> t
          { t r; c::glGetIntegerv( T_pname, &r[0] ); return r; }
        };
        
        template < typename gl_type::GLenum T_pname >
        struct get_impl_t < T_pname, glm::i32vec4 >
        {
          using t = glm::i32vec4;
          
          static inline auto invoke() -> t
          { t r; c::glGetIntegerv( T_pname, &r[0] ); return r; }
        };
        
        // int64
        
        template < typename gl_type::GLenum T_pname >
        struct get_impl_t < T_pname, gl_type::GLint64 >
        {
          using t = gl_type::GLint64;
          
          static inline auto invoke() -> t
          { t r; c::glGetInteger64v( T_pname, &r ); return r; }
        };
        
        template < typename gl_type::GLenum T_pname >
        struct get_impl_t < T_pname, glm::i64vec1 >
        {
          using t = glm::i64vec1;
          
          static inline auto invoke() -> t
          { t r; c::glGetInteger64v( T_pname, &r[0] ); return r; }
        };
        
        template < typename gl_type::GLenum T_pname >
        struct get_impl_t < T_pname, glm::i64vec2 >
        {
          using t = glm::i64vec2;
          
          static inline auto invoke() -> t
          { t r; c::glGetInteger64v( T_pname, &r[0] ); return r; }
        };
        
        template < typename gl_type::GLenum T_pname >
        struct get_impl_t < T_pname, glm::i64vec3 >
        {
          using t = glm::i64vec3;
          
          static inline auto invoke() -> t
          { t r; c::glGetInteger64v( T_pname, &r[0] ); return r; }
        };
        
        template < typename gl_type::GLenum T_pname >
        struct get_impl_t < T_pname, glm::i64vec4 >
        {
          using t = glm::i64vec4;
          
          static inline auto invoke() -> t
          { t r; c::glGetInteger64v( T_pname, &r[0] ); return r; }
        };
        
        struct get_t
        {
          
          template < typename gl_type::GLenum T_pname, class T = gl_type::GLint >
          static inline auto get()
            -> T
          { return get_impl_t< T_pname, T >::invoke(); }
          
          template < class T = gl_type::GLint >
          static inline auto get( typename gl_type::GLenum pname )
            -> T
          {
            std::stringstream message;
            message
              << "uniform<T>: unsupported type T."
                 " T=" << typeid( T ).name() << " "
                 " pname=" << pname << " "
              ;
            throw std::logic_error( message.str() );
          }
          
        };
        
        // bool
        
        template < >
        inline auto get_t::get< bool >( typename gl_type::GLenum pname )
          -> bool
        { bool r; c::glGetBooleanv( pname, reinterpret_cast< gl_type::GLboolean* >( &r ) ); return r; }
        
        template < >
        inline auto get_t::get< glm::bvec1 >( typename gl_type::GLenum pname )
          -> glm::bvec1
        { glm::bvec1 r; c::glGetBooleanv( pname, reinterpret_cast< gl_type::GLboolean* >( reinterpret_cast< gl_type::GLboolean* >( &r[0] ) ) ); return r; }
        
        template < >
        inline auto get_t::get< glm::bvec2 >( typename gl_type::GLenum pname )
          -> glm::bvec2
        { glm::bvec2 r; c::glGetBooleanv( pname, reinterpret_cast< gl_type::GLboolean* >( reinterpret_cast< gl_type::GLboolean* >( &r[0] ) ) ); return r; }
        
        template < >
        inline auto get_t::get< glm::bvec3 >( typename gl_type::GLenum pname )
          -> glm::bvec3
        { glm::bvec3 r; c::glGetBooleanv( pname, reinterpret_cast< gl_type::GLboolean* >( reinterpret_cast< gl_type::GLboolean* >( &r[0] ) ) ); return r; }
        
        template < >
        inline auto get_t::get< glm::bvec4 >( typename gl_type::GLenum pname )
          -> glm::bvec4
        { glm::bvec4 r; c::glGetBooleanv( pname, reinterpret_cast< gl_type::GLboolean* >( reinterpret_cast< gl_type::GLboolean* >( &r[0] ) ) ); return r; }
        
        // double
        
        template < >
        inline auto get_t::get< double >( typename gl_type::GLenum pname )
          -> double
        { double r; c::glGetDoublev( pname, &r ); return r; }
        
        template < >
        inline auto get_t::get< glm::f64vec1 >( typename gl_type::GLenum pname )
          -> glm::f64vec1
        { glm::f64vec1 r; c::glGetDoublev( pname, &r[0] ); return r; }
        
        template < >
        inline auto get_t::get< glm::f64vec2 >( typename gl_type::GLenum pname )
          -> glm::f64vec2
        { glm::f64vec2 r; c::glGetDoublev( pname, &r[0] ); return r; }
        
        template < >
        inline auto get_t::get< glm::f64vec3 >( typename gl_type::GLenum pname )
          -> glm::f64vec3
        { glm::f64vec3 r; c::glGetDoublev( pname, &r[0] ); return r; }
        
        template < >
        inline auto get_t::get< glm::f64vec4 >( typename gl_type::GLenum pname )
          -> glm::f64vec4
        { glm::f64vec4 r; c::glGetDoublev( pname, &r[0] ); return r; }
        
        // float
        
        template < >
        inline auto get_t::get< float >( typename gl_type::GLenum pname )
          -> float
        { float r; c::glGetFloatv( pname, &r ); return r; }
        
        template < >
        inline auto get_t::get< glm::vec1 >( typename gl_type::GLenum pname )
          -> glm::vec1
        { glm::vec1 r; c::glGetFloatv( pname, &r[0] ); return r; }
        
        template < >
        inline auto get_t::get< glm::vec2 >( typename gl_type::GLenum pname )
          -> glm::vec2
        { glm::vec2 r; c::glGetFloatv( pname, &r[0] ); return r; }
        
        template < >
        inline auto get_t::get< glm::vec3 >( typename gl_type::GLenum pname )
          -> glm::vec3
        { glm::vec3 r; c::glGetFloatv( pname, &r[0] ); return r; }
        
        template < >
        inline auto get_t::get< glm::vec4 >( typename gl_type::GLenum pname )
          -> glm::vec4
        { glm::vec4 r; c::glGetFloatv( pname, &r[0] ); return r; }
        
        // int
        
        template < >
        inline auto get_t::get< std::int32_t >( typename gl_type::GLenum pname )
          -> std::int32_t
        { std::int32_t r; c::glGetIntegerv( pname, &r ); return r; }
        
        template < >
        inline auto get_t::get< glm::i32vec1 >( typename gl_type::GLenum pname )
          -> glm::i32vec1
        { glm::i32vec1 r; c::glGetIntegerv( pname, &r[0] ); return r; }
        
        template < >
        inline auto get_t::get< glm::i32vec2 >( typename gl_type::GLenum pname )
          -> glm::i32vec2
        { glm::i32vec2 r; c::glGetIntegerv( pname, &r[0] ); return r; }
        
        template < >
        inline auto get_t::get< glm::i32vec3 >( typename gl_type::GLenum pname )
          -> glm::i32vec3
        { glm::i32vec3 r; c::glGetIntegerv( pname, &r[0] ); return r; }
        
        template < >
        inline auto get_t::get< glm::i32vec4 >( typename gl_type::GLenum pname )
          -> glm::i32vec4
        { glm::i32vec4 r; c::glGetIntegerv( pname, &r[0] ); return r; }
        
        // int64
        
        template < >
        inline auto get_t::get< std::int64_t >( typename gl_type::GLenum pname )
          -> std::int64_t
        { std::int64_t r; c::glGetInteger64v( pname, &r ); return r; }
        
        template < >
        inline auto get_t::get< glm::i64vec1 >( typename gl_type::GLenum pname )
          -> glm::i64vec1
        { glm::i64vec1 r; c::glGetInteger64v( pname, &r[0] ); return r; }
        
        template < >
        inline auto get_t::get< glm::i64vec2 >( typename gl_type::GLenum pname )
          -> glm::i64vec2
        { glm::i64vec2 r; c::glGetInteger64v( pname, &r[0] ); return r; }
        
        template < >
        inline auto get_t::get< glm::i64vec3 >( typename gl_type::GLenum pname )
          -> glm::i64vec3
        { glm::i64vec3 r; c::glGetInteger64v( pname, &r[0] ); return r; }
        
        template < >
        inline auto get_t::get< glm::i64vec4 >( typename gl_type::GLenum pname )
          -> glm::i64vec4
        { glm::i64vec4 r; c::glGetInteger64v( pname, &r[0] ); return r; }
        
      }
    }
  }
}