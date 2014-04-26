#pragma once

#include <memory>
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

        template<class T, size_t N>
        class program_uniform_delegate
        {
            friend program;
            decltype( nullptr ) operator()( gl_type::GLuint program_, gl_type::GLint location_ );
        };

        template<>
        class program_uniform_delegate<gl_type::GLfloat, 1>
        {
            friend program;
            using t = gl_type::GLfloat;
            t operator()( gl_type::GLuint program_, gl_type::GLint location_ )
            {
              t r;
              c::glGetUniformfv( program_, location_, &r );
              return std::move( r );
            }
        };

        template<>
        class program_uniform_delegate<gl_type::GLint, 1>
        {
            friend program;
            using t = gl_type::GLint;
            t operator()( gl_type::GLuint program_, gl_type::GLint location_ )
            {
              t r;
              c::glGetUniformiv( program_, location_, &r );
              return std::move( r );
            }
        };

        template<>
        class program_uniform_delegate<gl_type::GLuint, 1>
        {
            friend program;
            using t = gl_type::GLuint;
            t operator()( gl_type::GLuint program_, gl_type::GLint location_ )
            {
              t r;
              c::glGetUniformuiv( program_, location_, &r );
              return std::move( r );
            }
        };

        template<>
        class program_uniform_delegate<gl_type::GLdouble, 1>
        {
            friend program;
            using t = gl_type::GLdouble;
            t operator()( gl_type::GLuint program_, gl_type::GLint location_ )
            {
              t r;
              c::glGetUniformdv( program_, location_, &r );
              return std::move( r );
            }
        };

        template<size_t N>
        class program_uniform_delegate<gl_type::GLfloat, N>
        {
            friend program;
            using t = gl_type::GLfloat;
            using rt = vec<t, N>;
            rt operator()( gl_type::GLuint program_, gl_type::GLint location_ )
            {
              rt r;
              c::glGetUniformfv( program_, location_, r.data() );
              return std::move( r );
            }
        };

        template<size_t N>
        class program_uniform_delegate<gl_type::GLint, N>
        {
            friend program;
            using t = gl_type::GLint;
            using rt = vec<t, N>;
            rt operator()( gl_type::GLuint program_, gl_type::GLint location_ )
            {
              rt r;
              c::glGetUniformiv( program_, location_, r.data() );
              return std::move( r );
            }
        };

        template<size_t N>
        class program_uniform_delegate<gl_type::GLuint, N>
        {
            friend program;
            using t = gl_type::GLuint;
            using rt = vec<t, N>;
            rt operator()( gl_type::GLuint program_, gl_type::GLint location_ )
            {
              rt r;
              c::glGetUniformuiv( program_, location_, r.data() );
              return std::move( r );
            }
        };

        template<size_t N>
        class program_uniform_delegate<gl_type::GLdouble, N>
        {
            friend program;
            using t = gl_type::GLdouble;
            using rt = vec<t, N>;
            rt operator()( gl_type::GLuint program_, gl_type::GLint location_ )
            {
              rt r;
              c::glGetUniformdv( program_, location_, r.data() );
              return std::move( r );
            }
        };

      }
    }
  }
}