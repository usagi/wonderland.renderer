#pragma once

#include <sstream>
#include <typeinfo>

#define GLM_SWIZZLE
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/vec1.hpp>

#include "../c.hxx"
#include "../gl_type.hxx"

#include "program.hxx"

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      namespace glew
      {
        struct uniform_t
        {
          // get location from GPU by name
          static inline auto uniform( const gl_type::GLuint program_id, const std::string& name )
            -> gl_type::GLuint
          {
            const auto location = c::glGetUniformLocation( program_id, name.data() );
            return location;
          }
          
          static inline auto uniform( const std::string& name )
          -> gl_type::GLuint
          { return uniform( program_t::current_program(), name ); }
          
          // get value from GPU by loation
          //   impl as template specialization
          template < class T >
          static inline auto uniform( const gl_type::GLuint program_id, const gl_type::GLint location )
            -> T
          {
            std::stringstream message;
            message
              << "uniform<T>: unsupported type T."
                 " T=" << typeid( T ).name() << " "
                 " program_id=" << program_id << " "
                 " location=" << location
              ;
            throw std::logic_error( message.str() );
          }
          
          template < class T >
          static inline auto uniform( const gl_type::GLint location )
            -> T
          { return uniform<T>( program_t::current_program(), location ); }
          
          // get value from GPU by name
          template < class T >
          static inline auto uniform( const gl_type::GLuint program_id, const std::string& name )
            -> T
          { return uniform<T>( program_id, uniform( program_id, name) ); }
          
          template < class T >
          static inline auto uniform( const std::string& name )
            -> T
          {
            const auto program_id = program_t::current_program();
            return uniform<T>( program_id, uniform( program_id, name) );
          }
          
          // set value to GPU by name
          template < class T >
          static inline auto uniform( const gl_type::GLuint program_id, const std::string& name, const T& value )
            -> void
          { uniform( uniform( program_id, name ), value ); }
          
          template < class T >
          static inline auto uniform( const std::string& name, const T& value )
            -> void
          { uniform( uniform( program_t::current_program(), name ), value ); }
          
          // set float series
          static inline auto uniform( gl_type::GLint location, const gl_type::GLfloat value ) -> void
          { c::glUniform1f( location, value ); }
          
          static inline auto uniform( gl_type::GLint location, const glm::vec1& value) -> void
          { c::glUniform1f( location, value.x ); }
          
          template < std::size_t array_size >
          static inline auto uniform( gl_type::GLint location, const std::array< float, array_size >& values) -> void
          { c::glUniform1fv( location, array_size, values.data() ); }
          
          static inline auto uniform( gl_type::GLint location, const std::vector<float>& values) -> void
          { c::glUniform1fv( location, values.size(), values.data() ); }
          
          static inline auto uniform( gl_type::GLint location, const std::vector<glm::vec1>& values) -> void
          { c::glUniform1fv( location, values.size(), & values.data() -> x ); }
          
          static inline auto uniform( gl_type::GLint location, const glm::vec2& value ) -> void
          { c::glUniform2fv( location, 1, & value.x ); }
          
          static inline auto uniform( gl_type::GLint location, const std::vector<glm::vec2>& values ) -> void
          { c::glUniform2fv( location, values.size(), & values.data() -> x ); }
          
          static inline auto uniform( gl_type::GLint location, const glm::vec3& value ) -> void
          { c::glUniform3fv( location, 1, & value.x ); }
          
          static inline auto uniform( gl_type::GLint location, const std::vector<glm::vec3>& values ) -> void
          { c::glUniform3fv( location, values.size(), & values.data() -> x ); }
          
          static inline auto uniform( gl_type::GLint location, const glm::vec4& value ) -> void
          { c::glUniform4fv( location, 1, & value.x ); }
          
          static inline auto uniform( gl_type::GLint location, const std::vector<glm::vec4>& values ) -> void
          { c::glUniform4fv( location, values.size(), & values.data() -> x ); }
          
          static inline auto uniform( gl_type::GLint location, const glm::mat4& value ) -> void
          { c::glUniformMatrix4fv( location, 1, false, & value[0][0] ); }
          
          static inline auto uniform( gl_type::GLint location, const std::vector<glm::mat4>& values ) -> void
          { c::glUniformMatrix4fv( location, values.size(), false, & ( *values.data() )[0][0] ); }
          
          // set int series
          static inline auto uniform( gl_type::GLint location, const gl_type::GLint value ) -> void
          { c::glUniform1i( location, value ); }
          
          static inline auto uniform( gl_type::GLint location, const glm::i32vec1 value ) -> void
          { c::glUniform1i( location, value.x ); }
          
          static inline auto uniform( gl_type::GLint location, const std::vector<gl_type::GLint>& values ) -> void
          { c::glUniform1iv( location, values.size(), values.data() ); }
          
          static inline auto uniform( gl_type::GLint location, const std::vector<glm::i32vec1>& values ) -> void
          { c::glUniform1iv( location, values.size(), & values.data() -> x ); }
          
          static inline auto uniform( gl_type::GLint location, const glm::i32vec2& value ) -> void
          { c::glUniform2iv( location, 1, & value.x ); }
          
          static inline auto uniform( gl_type::GLint location, const std::vector<glm::i32vec2>& values ) -> void
          { c::glUniform2iv( location, values.size(), & values.data() -> x ); }
          
          static inline auto uniform( gl_type::GLint location, const glm::i32vec3& value ) -> void
          { c::glUniform3iv( location, 1, & value.x ); }
          
          static inline auto uniform( gl_type::GLint location, const std::vector<glm::i32vec3>& values ) -> void
          { c::glUniform3iv( location, values.size(), & values.data() -> x ); }
          
          static inline auto uniform( gl_type::GLint location, const glm::i32vec4& value ) -> void
          { c::glUniform4iv( location, 1, & value.x ); }
          
          static inline auto uniform( gl_type::GLint location, const std::vector<glm::i32vec4>& values ) -> void
          { c::glUniform4iv( location, values.size(), & values.data() -> x ); }
          
          // set uint series
          static inline auto uniform( gl_type::GLint location, const gl_type::GLuint value ) -> void
          { c::glUniform1ui( location, value ); }
          
          static inline auto uniform( gl_type::GLint location, const glm::u32vec1 value ) -> void
          { c::glUniform1ui( location, value.x ); }
          
          static inline auto uniform( gl_type::GLint location, const std::vector<gl_type::GLuint>& values ) -> void
          { c::glUniform1uiv( location, values.size(), values.data() ); }
          
          static inline auto uniform( gl_type::GLint location, const std::vector<glm::u32vec1>& values ) -> void
          { c::glUniform1uiv( location, values.size(), & values.data() -> x ); }
          
          static inline auto uniform( gl_type::GLint location, const glm::u32vec2& value ) -> void
          { c::glUniform2uiv( location, 1, & value.x ); }
          
          static inline auto uniform( gl_type::GLint location, const std::vector<glm::u32vec2>& values ) -> void
          { c::glUniform2uiv( location, values.size(), & values.data() -> x ); }
          
          static inline auto uniform( gl_type::GLint location, const glm::u32vec3& value ) -> void
          { c::glUniform3uiv( location, 1, & value.x ); }
          
          static inline auto uniform( gl_type::GLint location, const std::vector<glm::u32vec3>& values ) -> void
          { c::glUniform3uiv( location, values.size(), & values.data() -> x ); }
          
          static inline auto uniform( gl_type::GLint location, const glm::u32vec4& value ) -> void
          { c::glUniform4uiv( location, 1, & value.x ); }
          
          static inline auto uniform( gl_type::GLint location, const std::vector<glm::u32vec4>& values ) -> void
          { c::glUniform4uiv( location, values.size(), & values.data() -> x ); }
        };
        
        // get float series
        template < >
        inline auto uniform_t::uniform< gl_type::GLfloat >( gl_type::GLuint program_id, gl_type::GLint location )
          -> gl_type::GLfloat
        { gl_type::GLfloat value; c::glGetUniformfv( program_id, location, &value ); return value; }
        
        template < >
        inline auto uniform_t::uniform< glm::vec1 >( gl_type::GLuint program_id, gl_type::GLint location )
          -> glm::vec1
        { glm::vec1 value; c::glGetUniformfv( program_id, location, &value[0] ); return value; }
        
        template < >
        inline auto uniform_t::uniform< glm::vec2 >( gl_type::GLuint program_id, gl_type::GLint location )
          -> glm::vec2
        { glm::vec2 value; c::glGetUniformfv( program_id, location, &value[0] ); return value; }
        
        template < >
        inline auto uniform_t::uniform< glm::vec3 >( gl_type::GLuint program_id, gl_type::GLint location )
          -> glm::vec3
        { glm::vec3 value; c::glGetUniformfv( program_id, location, &value[0] ); return value; }

        template < >
        inline auto uniform_t::uniform< glm::vec4 >( gl_type::GLuint program_id, gl_type::GLint location )
          -> glm::vec4
        { glm::vec4 value; c::glGetUniformfv( program_id, location, &value[0] ); return value; }

        template < >
        inline auto uniform_t::uniform< glm::mat4 >( gl_type::GLuint program_id, gl_type::GLint location )
        -> glm::mat4
        { glm::mat4 value; c::glGetUniformfv( program_id, location, &value[0][0] ); return value; }

        // get int series
        template < >
        inline auto uniform_t::uniform< gl_type::GLint >( gl_type::GLuint program_id, gl_type::GLint location )
          -> gl_type::GLint
        { gl_type::GLint value; c::glGetUniformiv( program_id, location, &value ); return value; }
        
        template < >
        inline auto uniform_t::uniform< glm::i32vec1 >( gl_type::GLuint program_id, gl_type::GLint location )
          -> glm::i32vec1
        { glm::i32vec1 value; c::glGetUniformiv( program_id, location, &value[0] ); return value; }
        
        template < >
        inline auto uniform_t::uniform< glm::i32vec2 >( gl_type::GLuint program_id, gl_type::GLint location )
          -> glm::i32vec2
        { glm::i32vec2 value; c::glGetUniformiv( program_id, location, &value[0] ); return value; }
        
        template < >
        inline auto uniform_t::uniform< glm::i32vec3 >( gl_type::GLuint program_id, gl_type::GLint location )
          -> glm::i32vec3
        { glm::i32vec3 value; c::glGetUniformiv( program_id, location, &value[0] ); return value; }
        
        template < >
        inline auto uniform_t::uniform< glm::i32vec4 >( gl_type::GLuint program_id, gl_type::GLint location )
          -> glm::i32vec4
        { glm::i32vec4 value; c::glGetUniformiv( program_id, location, &value[0] ); return value; }
        
        // get uint series
        template < >
        inline auto uniform_t::uniform< gl_type::GLuint >( gl_type::GLuint program_id, gl_type::GLint location )
          -> gl_type::GLuint
        { gl_type::GLuint value; c::glGetUniformuiv( program_id, location, &value ); return value; }
        
        template < >
        inline auto uniform_t::uniform< glm::u32vec1 >( gl_type::GLuint program_id, gl_type::GLint location )
          -> glm::u32vec1
        { glm::u32vec1 value; c::glGetUniformuiv( program_id, location, &value[0] ); return value; }
        
        template < >
        inline auto uniform_t::uniform< glm::u32vec2 >( gl_type::GLuint program_id, gl_type::GLint location )
          -> glm::u32vec2
        { glm::u32vec2 value; c::glGetUniformuiv( program_id, location, &value[0] ); return value; }
        
        template < >
        inline auto uniform_t::uniform< glm::u32vec3 >( gl_type::GLuint program_id, gl_type::GLint location )
          -> glm::u32vec3
        { glm::u32vec3 value; c::glGetUniformuiv( program_id, location, &value[0] ); return value; }
        
        template < >
        inline auto uniform_t::uniform< glm::u32vec4 >( gl_type::GLuint program_id, gl_type::GLint location )
          -> glm::u32vec4
        { glm::u32vec4 value; c::glGetUniformuiv( program_id, location, &value[0] ); return value; }
        
        // get double series
        template < >
        inline auto uniform_t::uniform< gl_type::GLdouble >( gl_type::GLuint program_id, gl_type::GLint location )
          -> gl_type::GLdouble
        { gl_type::GLdouble value; c::glGetUniformdv( program_id, location, &value ); return value; }
        
        template < >
        inline auto uniform_t::uniform< glm::f64vec1 >( gl_type::GLuint program_id, gl_type::GLint location )
          -> glm::f64vec1
        { glm::f64vec1 value; c::glGetUniformdv( program_id, location, &value[0] ); return value; }
        
        template < >
        inline auto uniform_t::uniform< glm::f64vec2 >( gl_type::GLuint program_id, gl_type::GLint location )
          -> glm::f64vec2
        { glm::f64vec2 value; c::glGetUniformdv( program_id, location, &value[0] ); return value; }
        
        template < >
        inline auto uniform_t::uniform< glm::f64vec3 >( gl_type::GLuint program_id, gl_type::GLint location )
          -> glm::f64vec3
        { glm::f64vec3 value; c::glGetUniformdv( program_id, location, &value[0] ); return value; }
        
        template < >
        inline auto uniform_t::uniform< glm::f64vec4 >( gl_type::GLuint program_id, gl_type::GLint location )
          -> glm::f64vec4
        { glm::f64vec4 value; c::glGetUniformdv( program_id, location, &value[0] ); return value; }
      }
    }
  }
}