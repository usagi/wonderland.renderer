#pragma once

#include <boost/gil/gil_all.hpp>
#include <boost/optional.hpp>

// assimp::Importer
#include <assimp/Importer.hpp>
// aiPostProcessSteps for assimp::Importer::ReadFile
#include <assimp/postprocess.h>
// aiScene, aiNode
#include <assimp/scene.h>

#include "../glew.detail/c.hxx"
#include "../glew.detail/gl_type.hxx"

#include "material.hxx"

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      namespace model
      {
        class mesh_t
        {
          glew::gl_type::GLuint  _triangle_buffer_id;
          glew::gl_type::GLsizei _count_of_triangles;
          glew::gl_type::GLuint  _quad_buffer_id;
          glew::gl_type::GLsizei _count_of_quads;
          
          glew::gl_type::GLuint  _triangle_vao_id;
          glew::gl_type::GLuint  _quad_vao_id;
          
          const material_t& _material;
          
        public:
          
          struct vertex_buffer_t
          {
            using float_t = float;
            
            struct position_t { float_t x, y, z, w; } position;
            struct texcoord_t { float_t x, y, z; } texcoord;
            struct normal_t   { float_t x, y, z; } normal;
            struct tangent_t  { float_t x, y, z; } tangent;
            
            vertex_buffer_t
            ( float_t pos_x, float_t pos_y, float_t pos_z, float_t pos_w
            , float_t tex_x, float_t tex_y, float_t tex_z
            , float_t nor_x, float_t nor_y, float_t nor_z
            , float_t tan_x, float_t tan_y, float_t tan_z
            )
            : position( { pos_x, pos_y, pos_z, pos_w } )
            , texcoord( { tex_x, tex_y, tex_z } )
            , normal  ( { nor_x, nor_y, nor_z } )
            , tangent ( { tan_x, tan_y, tan_z } )
            { }
            
            template < class T = float_t >
            auto to_ptr()
            -> const T*
            { return reinterpret_cast< T* >( &position.x ); }
            
            static constexpr auto size_of_element = sizeof( float_t );
            
            static constexpr auto size_of_memory
            = ( sizeof( position_t )
            + sizeof( texcoord_t )
            + sizeof( normal_t )
            + sizeof( tangent_t )
            );
            
            static constexpr auto count_of_elements = size_of_memory / size_of_element;
            
            static constexpr auto count_of_position_elements = sizeof( position_t ) / size_of_element;
            static constexpr auto count_of_texcoord_elements = sizeof( texcoord_t ) / size_of_element;
            static constexpr auto count_of_normal_elements   = sizeof( normal_t   ) / size_of_element;
            static constexpr auto count_of_tangent_elements  = sizeof( tangent_t  ) / size_of_element;
            
            static constexpr auto memory_offset_of_position = std::size_t( 0 );
            static constexpr auto memory_offset_of_texcorrd = memory_offset_of_position + sizeof( position_t );
            static constexpr auto memory_offset_of_normal   = memory_offset_of_texcorrd + sizeof( texcoord_t );
            static constexpr auto memory_offset_of_tangent  = memory_offset_of_normal   + sizeof( normal_t   );
          };
          
          ~mesh_t()
          {
            // http://www.opengl.org/sdk/docs/man/html/glDeleteBuffers.xhtml
            //  glew::gl_type::GLsizei n, const glew::gl_type::GLuint buffers
            glew::c::glDeleteBuffers( 1, &_triangle_buffer_id );
            glew::c::glDeleteBuffers( 1, &_quad_buffer_id );
            
            // http://www.opengl.org/sdk/docs/man/html/glDeleteVertexArrays.xhtml
            //  glew::gl_type::GLsizei n, const glew::gl_type::GLuint* arrays
            glew::c::glDeleteVertexArrays( 1, &_triangle_vao_id );
            glew::c::glDeleteVertexArrays( 1, &_quad_vao_id );
          }
          
          mesh_t( const aiMesh* mesh, const std::vector<material_t>& materials_ )
            : _material( materials_.at( mesh -> mMaterialIndex ) )
          {
            
            // 頂点バッファー生成のための準備用バッファー型
            using prepare_vertex_buffer_t = std::vector< vertex_buffer_t >;
            
            // 三角群と四角群の準備用バッファー
            prepare_vertex_buffer_t triangles_buffer, quads_buffer;
            
            // 面の数だけループ
            for ( auto n_face = 0; n_face < mesh -> mNumFaces; ++n_face )
            {
              // 面
              const auto face = mesh -> mFaces + n_face;
              
              // 頂点準備バッファーに面の頂点データを書き出すファンクター
              const auto push_pvb = [&]( prepare_vertex_buffer_t& pvb, unsigned n_vertex )
              {
                const auto offset = face -> mIndices[ n_vertex ];
                
                const auto position = mesh -> mVertices           + offset;
                const auto texcoord = mesh -> mTextureCoords[ 0 ] + offset;
                const auto normal   = mesh -> mNormals            + offset;
                const auto tangent  = mesh -> mTangents           + offset;
                
                pvb.emplace_back
                ( position -> x, position -> y, position -> z, 1.f
                , texcoord -> x, texcoord -> y, texcoord -> z
                , normal   -> x, normal   -> y, normal   -> z
                , tangent  -> x, tangent  -> y, tangent  -> z
                );
              };
              
              // 三角は頂点3つ、四角は頂点4つ
              constexpr auto indices_of_triangle = 3;
              constexpr auto indices_of_quad     = 4;
              
              // 面が三角か四角かで準備バッファーを切り替えて書き出し
              switch( face -> mNumIndices )
              {
                case indices_of_triangle:
                  for ( auto n_vertex = 0; n_vertex < indices_of_triangle; ++n_vertex )
                    push_pvb( triangles_buffer, n_vertex );
                  break;
                case indices_of_quad:
                  for ( auto n_vertex = 0; n_vertex < indices_of_quad; ++n_vertex )
                    push_pvb( quads_buffer, n_vertex );
              }
            }
            
            // 三角群のバッファーを生成
            // http://www.opengl.org/sdk/docs/man/html/glGenVertexArrays.xhtml
            //  glew::gl_type::GLsizei n, glew::gl_type::GLuint* arrays
            glew::c::glGenVertexArrays( 1, &_triangle_vao_id );
            // http://www.opengl.org/sdk/docs/man/html/glBindVertexArray.xhtml
            //  glew::gl_type::GLuint array
            glew::c::glBindVertexArray( _triangle_vao_id );
            // http://www.opengl.org/sdk/docs/man/html/glGenBuffers.xhtml
            //  glew::gl_type::GLsizei n, glew::gl_type::GLuint* buffers
            glew::c::glGenBuffers( 1, &_triangle_buffer_id);
            // http://www.opengl.org/sdk/docs/man/html/glBindBuffer.xhtml
            //  GLenum target, glew::gl_type::GLuint buffer
            glew::c::glBindBuffer( GL_ARRAY_BUFFER, _triangle_buffer_id );
            // http://www.opengl.org/sdk/docs/man/html/glBufferData.xhtml
            //  GLenum target, glew::gl_type::GLsizeiptr size, const GLvoid* data, GLenum usage
            glew::c::glBufferData
            ( GL_ARRAY_BUFFER
            , triangles_buffer.size() * vertex_buffer_t::size_of_memory
            , triangles_buffer.data() -> to_ptr<void>()
            , glew::gl_type::GLenum( GL_STATIC_DRAW )
            );
            // 後始末
            glew::c::glBindBuffer( GL_ARRAY_BUFFER, 0 );
            glew::c::glBindVertexArray( 0 );
            _count_of_triangles = triangles_buffer.size();
            
            // 四角群のバッファーを生成
            glew::c::glGenVertexArrays( 1, &_quad_vao_id );
            glew::c::glBindVertexArray( _quad_vao_id );
            glew::c::glGenBuffers( 1, &_quad_buffer_id);
            glew::c::glBindBuffer( GL_ARRAY_BUFFER, _quad_buffer_id );
            glew::c::glBufferData
            ( GL_ARRAY_BUFFER
            , quads_buffer.size() * vertex_buffer_t::size_of_memory
            , quads_buffer.data() -> to_ptr<void>()
            , glew::gl_type::GLenum( GL_STATIC_DRAW )
            );
            glew::c::glBindBuffer( GL_ARRAY_BUFFER, 0 );
            glew::c::glBindVertexArray( 0 );
            _count_of_quads = quads_buffer.size();
          }
          
          auto draw() const
          -> void
          {
            constexpr glew::gl_type::GLenum    attribute = GL_FLOAT;
            constexpr glew::gl_type::GLboolean normalize_on  = true;
            constexpr glew::gl_type::GLboolean normalize_off = false;
            
            const auto set_vertex_attribute = [ this ]
            {
              glew::gl_type::GLint program_id;
              glew::c::glGetIntegerv( GL_CURRENT_PROGRAM, &program_id );
              
              const auto location_of_vs_position = glew::c::glGetAttribLocation( program_id, "position" );
              const auto location_of_vs_texcoord = glew::c::glGetAttribLocation( program_id, "texcoord" );
              const auto location_of_vs_normal   = glew::c::glGetAttribLocation( program_id, "normal"   );
              const auto location_of_vs_tangent  = glew::c::glGetAttribLocation( program_id, "tangent"  );
              
              if ( location_of_vs_position not_eq -1 )
              {
                // http://www.opengl.org/sdk/docs/man/html/glVertexAttribPointer.xhtml
                //  glew::gl_type::GLuint index, GLint size, GLenum type, GLboolean normalized, glew::gl_type::GLsizei stride, const GLvoid* pointer
                glew::c::glVertexAttribPointer( location_of_vs_position, vertex_buffer_t::count_of_position_elements, attribute, normalize_off, vertex_buffer_t::size_of_memory, reinterpret_cast<void*>( vertex_buffer_t::memory_offset_of_position ) );
                // http://www.opengl.org/sdk/docs/man/html/glEnableVertexAttribArray.xhtml
                //  glew::gl_type::GLuint index
                glew::c::glEnableVertexAttribArray( location_of_vs_position );
              }
              
              if ( location_of_vs_texcoord not_eq -1 )
              { 
                glew::c::glVertexAttribPointer( location_of_vs_texcoord, vertex_buffer_t::count_of_texcoord_elements, attribute, normalize_off, vertex_buffer_t::size_of_memory, reinterpret_cast<void*>( vertex_buffer_t::memory_offset_of_texcorrd ) );
                glew::c::glEnableVertexAttribArray( location_of_vs_texcoord );
              }
              
              if ( location_of_vs_normal not_eq -1 )
              {
                glew::c::glVertexAttribPointer( location_of_vs_normal  , vertex_buffer_t::count_of_normal_elements  , attribute, normalize_on , vertex_buffer_t::size_of_memory, reinterpret_cast<void*>( vertex_buffer_t::memory_offset_of_normal   ) );
                glew::c::glEnableVertexAttribArray( location_of_vs_normal   );
              }
              
              if ( location_of_vs_tangent not_eq -1 )
              {
                glew::c::glVertexAttribPointer( location_of_vs_tangent , vertex_buffer_t::count_of_tangent_elements , attribute, normalize_on , vertex_buffer_t::size_of_memory, reinterpret_cast<void*>( vertex_buffer_t::memory_offset_of_tangent  ) );
                glew::c::glEnableVertexAttribArray( location_of_vs_tangent  );
              }
              
              // マテリアルの有効化
              _material.draw();
            };
            
            // 三角群描画
            {
              glew::c::glBindVertexArray( _triangle_vao_id );
              // http://www.opengl.org/sdk/docs/man/html/glBindBuffer.xhtml
              //  GLenum target, glew::gl_type::GLuint buffer
              glew::c::glBindBuffer( GL_ARRAY_BUFFER, _triangle_buffer_id );
              set_vertex_attribute();
              // http://www.opengl.org/sdk/docs/man/html/glDrawArrays.xhtml
              //  GLenum mode, GLint first, glew::gl_type::GLsizei count
              glew::c::glDrawArrays( GL_TRIANGLES, 0, _count_of_triangles );
            }
            
            // 四角群描画
            {
              glew::c::glBindVertexArray( _quad_vao_id );
              glew::c::glBindBuffer( GL_ARRAY_BUFFER, _quad_buffer_id );
              set_vertex_attribute();
              glew::c::glDrawArrays( GL_QUADS, 0, _count_of_quads );
            }
            
            // 後始末
            //glBindBuffer( GL_ARRAY_BUFFER, 0 );
            glew::c::glBindVertexArray( 0 );
          }
          
        };
      }
    }
  }
}