#pragma once

#include <array>
#include <typeinfo>
#include <sstream>

#define GLM_SWIZZLE
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/vec1.hpp>
#include <glm/gtc/constants.hpp>

#include "../c.hxx"
#include "../gl_type.hxx"
#include "../../destruct_invoker.hxx"

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      namespace glew
      {
        struct sampler_t
        {
          
          template < typename gl_type::GLsizei T_count_of_samplers = 1 >
          static inline auto generate_samplers()
            -> std::array< gl_type::GLuint, T_count_of_samplers >
          {
            std::array< gl_type::GLuint, T_count_of_samplers > samplers;
            c::glGenSamplers( T_count_of_samplers, &samplers[0] );
            return samplers;
          }
          
          template < class T = void >
          static inline auto generate_sampler()
            -> gl_type::GLuint
          {
            gl_type::GLuint sampler;
            c::glGenSamplers( 1, &sampler );
            return sampler;
          }
          
          template < class T = void >
          static inline auto delete_sampler( gl_type::GLuint sampler_id )
            -> void
          { c::glDeleteSamplers( 1, &sampler_id ); }
          
          template < class T = void >
          static inline auto bind_sampler( gl_type::GLuint unit, gl_type::GLuint sampler )
            -> void
          { c::glBindSampler( unit, sampler ); }
          
          // note: parameter reversed for default use case easy.
          template < class T_samplers >
          static inline auto bind_samplers( T_samplers samplers, gl_type::GLuint first = 0 )
            -> void
#ifdef GL_VERSION_4_4
          { c::glBindSamplers( first, samplers.size(), &samplers[0] ); }
#else
          {
            for ( auto n = 0; n < samplers.size(); ++n )
              bind_sampler( n, samplers[n] );
          }
#endif
          
          // has template specializing
          template < class T >
          static inline auto sampler_parameter( gl_type::GLuint sampler, gl_type::GLenum pname, T param )
            -> void
          {
            std::stringstream message;
            message
              << "sampler_parameter: unsupported T."
                 " T_internal_format=" << typeid( T ).name() << " "
                 " sampler=" << sampler << " "
                 " pname=" << pname << " "
                 " param=" << param << " "
              ;
            throw std::logic_error( message.str() );
          }
          
          // has template specializing
          template < class T >
          static inline auto texture_parameter( gl_type::GLenum target, gl_type::GLenum pname, T param )
            -> void
          {
            std::stringstream message;
            message
              << "texture_parameter: unsupported T."
                 " T_internal_format=" << typeid( T ).name() << " "
                 " target=" << target << " "
                 " pname=" << pname << " "
                 " param=" << param << " "
              ;
            throw std::logic_error( message.str() );
          }
          
        };
        
        template < >
        inline auto sampler_t::sampler_parameter< gl_type::GLfloat >
        ( gl_type::GLuint sampler, gl_type::GLenum pname, gl_type::GLfloat param )
          -> void
        { c::glSamplerParameterf( sampler, pname, param ); }
        
        template < >
        inline auto sampler_t::sampler_parameter< glm::vec1 >
        ( gl_type::GLuint sampler, gl_type::GLenum pname, glm::vec1 param )
          -> void
        { c::glSamplerParameterfv( sampler, pname, &param[0] ); }
        
        template < >
        inline auto sampler_t::sampler_parameter< glm::vec2 >
        ( gl_type::GLuint sampler, gl_type::GLenum pname, glm::vec2 param )
          -> void
        { c::glSamplerParameterfv( sampler, pname, &param[0] ); }
        
        template < >
        inline auto sampler_t::sampler_parameter< glm::vec3 >
        ( gl_type::GLuint sampler, gl_type::GLenum pname, glm::vec3 param )
          -> void
        { c::glSamplerParameterfv( sampler, pname, &param[0] ); }
        
        template < >
        inline auto sampler_t::sampler_parameter< glm::vec4 >
        ( gl_type::GLuint sampler, gl_type::GLenum pname, glm::vec4 param )
          -> void
        { c::glSamplerParameterfv( sampler, pname, &param[0] ); }
        
        template < >
        inline auto sampler_t::sampler_parameter< gl_type::GLint >
        ( gl_type::GLuint sampler, gl_type::GLenum pname, gl_type::GLint param )
          -> void
        { c::glSamplerParameteri( sampler, pname, param ); }
        
        template < >
        inline auto sampler_t::sampler_parameter< glm::i32vec1 >
        ( gl_type::GLuint sampler, gl_type::GLenum pname, glm::i32vec1 param )
          -> void
        { c::glSamplerParameteriv( sampler, pname, &param[0] ); }
        
        template < >
        inline auto sampler_t::sampler_parameter< glm::i32vec2 >
        ( gl_type::GLuint sampler, gl_type::GLenum pname, glm::i32vec2 param )
          -> void
        { c::glSamplerParameteriv( sampler, pname, &param[0] ); }
        
        template < >
        inline auto sampler_t::sampler_parameter< glm::i32vec3 >
        ( gl_type::GLuint sampler, gl_type::GLenum pname, glm::i32vec3 param )
          -> void
        { c::glSamplerParameteriv( sampler, pname, &param[0] ); }
        
        template < >
        inline auto sampler_t::sampler_parameter< glm::i32vec4 >
        ( gl_type::GLuint sampler, gl_type::GLenum pname, glm::i32vec4 param )
          -> void
        { c::glSamplerParameteriv( sampler, pname, &param[0] ); }
        
        template < >
        inline auto sampler_t::texture_parameter< gl_type::GLfloat >
        ( gl_type::GLenum target, gl_type::GLenum pname, gl_type::GLfloat param )
          -> void
        { c::glTexParameterf( target, pname, param ); }
        
        template < >
        inline auto sampler_t::texture_parameter< glm::vec1 >
        ( gl_type::GLenum target, gl_type::GLenum pname, glm::vec1 param )
          -> void
        { c::glTexParameterfv( target, pname, &param[0] ); }
        
        template < >
        inline auto sampler_t::texture_parameter< glm::vec2 >
        ( gl_type::GLenum target, gl_type::GLenum pname, glm::vec2 param )
          -> void
        { c::glTexParameterfv( target, pname, &param[0] ); }
        
        template < >
        inline auto sampler_t::texture_parameter< glm::vec3 >
        ( gl_type::GLenum target, gl_type::GLenum pname, glm::vec3 param )
          -> void
        { c::glTexParameterfv( target, pname, &param[0] ); }
        
        template < >
        inline auto sampler_t::texture_parameter< glm::vec4 >
        ( gl_type::GLenum target, gl_type::GLenum pname, glm::vec4 param )
          -> void
        { c::glTexParameterfv( target, pname, &param[0] ); }
        
        template < >
        inline auto sampler_t::texture_parameter< gl_type::GLint >
        ( gl_type::GLenum target, gl_type::GLenum pname, gl_type::GLint param )
          -> void
        { c::glTexParameterf( target, pname, param ); }
        
        template < >
        inline auto sampler_t::texture_parameter< glm::i32vec1 >
        ( gl_type::GLenum target, gl_type::GLenum pname, glm::i32vec1 param )
          -> void
        { c::glTexParameteriv( target, pname, &param[0] ); }
        
        template < >
        inline auto sampler_t::texture_parameter< glm::i32vec2 >
        ( gl_type::GLenum target, gl_type::GLenum pname, glm::i32vec2 param )
          -> void
        { c::glTexParameteriv( target, pname, &param[0] ); }
        
        template < >
        inline auto sampler_t::texture_parameter< glm::i32vec3 >
        ( gl_type::GLenum target, gl_type::GLenum pname, glm::i32vec3 param )
          -> void
        { c::glTexParameteriv( target, pname, &param[0] ); }
        
        template < >
        inline auto sampler_t::texture_parameter< glm::i32vec4 >
        ( gl_type::GLenum target, gl_type::GLenum pname, glm::i32vec4 param )
          -> void
        { c::glTexParameteriv( target, pname, &param[0] ); }
        
      }
    }
  }
}