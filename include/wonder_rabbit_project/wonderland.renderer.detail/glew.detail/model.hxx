#pragma once

#include <boost/gil/gil_all.hpp>

#include "c.hxx"
#include "gl_type.hxx"

#include "model_invoke_vertex_attribute_pointer.hxx"

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      namespace glew
      {

        struct model
        {
            friend glew;
            virtual ~model() { }

            // ToDo: template parametrize
            // ToDo: overload the other view
            void texture_2d( const gl_type::GLuint n, const boost::gil::rgb8_view_t& v ) const
            {
              c::glBindTexture( GL_TEXTURE_2D, n );

              // ToDo: template parametrize
              c::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
              c::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
              c::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
              c::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

              c::glTexImage2D(
                GL_TEXTURE_2D, 0, GL_RGB,
                v.width(), v.height(), 0,
                GL_RGB, gl_type::GLenum( VERTEX_ATTRIBUTE::UINT8 ),
                boost::gil::interleaved_view_get_raw_data( v )
              );
            }

          protected:
            virtual void invoke() const = 0;
        };

        template
        <
        class TVERTEX,
              class TUSAGE,
              class TDEFAULT_INVOKE_MODE
              >
        struct model_v final : model
        {
          friend glew;
          using vertex_type = TVERTEX;
          using data_type = std::vector<vertex_type>;
          static constexpr USAGE usage = USAGE( TUSAGE::value );
          static constexpr MODE default_invoke_mode = TDEFAULT_INVOKE_MODE::value;
        public:
          ~model_v() override
          {
            c::glDeleteBuffers( buffer_count, &vertex_buffer );
            c::glDeleteVertexArrays( buffer_count, &vertex_arrays );
          }
        private:
          static constexpr size_t buffer_count = 1;
          gl_type::GLuint vertex_buffer;
          gl_type::GLuint vertex_arrays;
          gl_type::GLuint vertices;

          model_v( data_type && data )
          {
            vertices = data.size();
            c::glGenVertexArrays( buffer_count, &vertex_arrays );
            c::glBindVertexArray( vertex_arrays );
            c::glGenBuffers( buffer_count, &vertex_buffer );
            c::glBindBuffer( GL_ARRAY_BUFFER, vertex_buffer );
            c::glBufferData(
              GL_ARRAY_BUFFER,
              vertex_type::size * vertices,
              data.data(),
              gl_type::GLenum( usage )
            );
            c::glBindBuffer( GL_ARRAY_BUFFER, 0 );
            c::glBindVertexArray( 0 );
          }

          template<class TMODE>
          void invoke() const //override
          {
            c::glBindVertexArray( vertex_arrays );
            c::glBindBuffer( GL_ARRAY_BUFFER, vertex_buffer );
            model_invoke_vertex_attribute_pointer<vertex_type>::invoke();
            c::glDrawArrays( gl_type::GLenum( TMODE::value ), 0, vertices );
            c::glBindBuffer( GL_ARRAY_BUFFER, 0 );
            //c::glBindVertexArray(0);
          }

          void invoke() const override
          {
            invoke<TDEFAULT_INVOKE_MODE>();
          }
        };

        template
        <
        class TVERTEX,
              class TUSAGE,
              class TDEFAULT_INVOKE_MODE
              >
        struct model_vi final : model
        {
          friend glew;
          using vertex_type = TVERTEX;
          using data_vertices_type = std::vector<vertex_type>;
          static constexpr USAGE usage = USAGE( TUSAGE::value );
          static constexpr MODE default_invoke_mode = TDEFAULT_INVOKE_MODE::value;
          using data_indices_element_type = gl_type::GLuint;
          using data_indices_type = std::vector<data_indices_element_type>;
        public:
          ~model_vi() override
          {
            c::glDeleteBuffers( buffer_count, buffer.data() );
          }

        private:
          static constexpr size_t buffer_count = 2;
          static constexpr size_t vertex_buffer_id = 0;
          static constexpr size_t index_buffer_id  = vertex_buffer_id + 1;
          std::array<gl_type::GLuint, buffer_count> buffer;
          gl_type::GLuint
          num_of_vertices,
          num_of_indices;

          const gl_type::GLuint& vertex_buffer() const
          {
            return buffer[vertex_buffer_id];
          }
          const gl_type::GLuint& index_buffer() const
          {
            return buffer[index_buffer_id];
          }

          model_vi( data_vertices_type && data_vertices, data_indices_type && data_indices )
          {
            num_of_vertices = data_vertices.size();
            num_of_indices  = data_indices.size();
            c::glGenBuffers( buffer_count, buffer.data() );
            c::glBindBuffer( GL_ARRAY_BUFFER, vertex_buffer() );
            c::glBufferData(
              GL_ARRAY_BUFFER,
              vertex_type::size * num_of_vertices,
              data_vertices.data(),
              gl_type::GLenum( usage )
            );
            c::glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, index_buffer() );
            c::glBufferData(
              GL_ELEMENT_ARRAY_BUFFER,
              sizeof( data_indices_element_type ) * num_of_indices,
              data_indices.data(),
              gl_type::GLenum( usage )
            );
            c::glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
            c::glBindBuffer( GL_ARRAY_BUFFER, 0 );
          }

          template<class TMODE>
          void invoke() const //override
          {
            c::glBindBuffer( GL_ARRAY_BUFFER, vertex_buffer() );
            c::glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, index_buffer() );
            model_invoke_vertex_attribute_pointer<vertex_type>::invoke();
            c::glDrawElements(
              gl_type::GLenum( TMODE::value ),
              num_of_indices,
              gl_type::GLenum( VERTEX_ATTRIBUTE::UINT32 ),
              0
            );
            c::glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
            c::glBindBuffer( GL_ARRAY_BUFFER, 0 );
          }

          void invoke() const override
          {
            invoke<TDEFAULT_INVOKE_MODE>();
          }
        };

      }
    }
  }
}