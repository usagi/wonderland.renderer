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
#include "../glew.detail/error.hxx"

#include "helper.hxx"
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
          
          glew::gl_type::GLuint  _triangle_vao_id;
          
          const material_t& _material;
          
        public:
          
          struct vertex_buffer_t
          {
            using float_t = float;
            
            glm::vec4 position;
            glm::vec2 texcoord;
            glm::vec3 normal;
            glm::vec3 tangent;
            glm::vec4 bone_ids;
            glm::vec4 bone_weights;
            
            vertex_buffer_t
            ( glm::vec4&& pos
            , glm::vec2&& tex
            , glm::vec3&& nor
            , glm::vec3&& tan
            , glm::vec4&& bid
            , glm::vec4&& bwt
            )
              : position     ( std::move( pos ) )
              , texcoord     ( std::move( tex ) )
              , normal       ( std::move( nor ) )
              , tangent      ( std::move( tan ) )
              , bone_ids     ( std::move( bid ) )
              , bone_weights ( std::move( bwt ) )
            { }
            
            template < class T = float_t >
            auto to_ptr()
            -> const T*
            { return reinterpret_cast< T* >( &position.x ); }
            
            static constexpr auto size_of_element = sizeof( float_t );
            
            static constexpr auto size_of_memory
              = ( sizeof( decltype( position ) )
                + sizeof( decltype( texcoord ) )
                + sizeof( decltype( normal ) )
                + sizeof( decltype( tangent ) )
                + sizeof( decltype( bone_ids ) )
                + sizeof( decltype( bone_weights ) )
                );
            
            static constexpr auto count_of_elements = size_of_memory / size_of_element;
            
            static constexpr auto count_of_position_elements     = sizeof( decltype( position )     ) / size_of_element;
            static constexpr auto count_of_texcoord_elements     = sizeof( decltype( texcoord )     ) / size_of_element;
            static constexpr auto count_of_normal_elements       = sizeof( decltype( normal )       ) / size_of_element;
            static constexpr auto count_of_tangent_elements      = sizeof( decltype( tangent )      ) / size_of_element;
            static constexpr auto count_of_bone_ids_elements     = sizeof( decltype( bone_ids )     ) / size_of_element;
            static constexpr auto count_of_bone_weights_elements = sizeof( decltype( bone_weights ) ) / size_of_element;
            
            static constexpr auto memory_offset_of_position     = std::size_t( 0 );
            static constexpr auto memory_offset_of_texcoord     = memory_offset_of_position + sizeof( decltype( position ) );
            static constexpr auto memory_offset_of_normal       = memory_offset_of_texcoord + sizeof( decltype( texcoord ) );
            static constexpr auto memory_offset_of_tangent      = memory_offset_of_normal   + sizeof( decltype( normal )   );
            static constexpr auto memory_offset_of_bone_ids     = memory_offset_of_tangent  + sizeof( decltype( tangent )  );
            static constexpr auto memory_offset_of_bone_weights = memory_offset_of_bone_ids + sizeof( decltype( bone_ids ) );
          };
          
          ~mesh_t()
          {
            // http://www.opengl.org/sdk/docs/man/html/glDeleteBuffers.xhtml
            //  glew::gl_type::GLsizei n, const glew::gl_type::GLuint buffers
            glew::c::glDeleteBuffers( 1, &_triangle_buffer_id );
            
            // http://www.opengl.org/sdk/docs/man/html/glDeleteVertexArrays.xhtml
            //  glew::gl_type::GLsizei n, const glew::gl_type::GLuint* arrays
            glew::c::glDeleteVertexArrays( 1, &_triangle_vao_id );
          }
          
          mesh_t( const aiMesh* mesh, const std::vector<material_t>& materials_ )
            : _material( materials_.at( mesh -> mMaterialIndex ) )
          {
            
            // 頂点バッファー生成のための準備用バッファー型
            using prepare_vertex_buffer_t = std::vector< vertex_buffer_t >;
            
            // 三角群の準備用バッファー
            prepare_vertex_buffer_t triangles_buffer;
            
            // 面の数だけループ
            for ( auto n_face = 0; n_face < mesh -> mNumFaces; ++n_face )
            {
              // 面
              const auto face = mesh -> mFaces + n_face;
              
              // 頂点準備バッファーに面の頂点データを書き出すファンクター
              const auto push_pvb = [&]( prepare_vertex_buffer_t& pvb, unsigned n_vertex )
              {
                const auto offset = face -> mIndices[ n_vertex ];
                
                pvb.emplace_back
                ( std::move( helper::to_glm_vec4( mesh -> mVertices + offset           ) )
                , std::move( helper::to_glm_vec2( mesh -> mTextureCoords[ 0 ] + offset ) )
                , std::move( helper::to_glm_vec3( mesh -> mNormals  + offset           ) )
                , std::move( helper::to_glm_vec3( mesh -> mTangents + offset           ) )
                , std::move( glm::vec4( 0.0f ) )
                , std::move( glm::vec4( 0.0f ) )
                );
              };
              
              // 三角は頂点3つ
              constexpr auto indices_of_triangle = 3;
              
              // Assimpポストプロセスにより四角面は三角面に変換済みなので三角面だけを考慮すればよい
              for ( auto n_vertex = 0; n_vertex < indices_of_triangle; ++n_vertex )
                push_pvb( triangles_buffer, n_vertex );
            }
            
            // 三角群のバッファーを生成
            // http://www.opengl.org/sdk/docs/man/html/glGenVertexArrays.xhtml
            //  glew::gl_type::GLsizei n, glew::gl_type::GLuint* arrays
            glew::c::glGenVertexArrays( 1, &_triangle_vao_id );
            glew::test_error( __FILE__, __LINE__ );
            
            // http://www.opengl.org/sdk/docs/man/html/glBindVertexArray.xhtml
            //  glew::gl_type::GLuint array
            glew::c::glBindVertexArray( _triangle_vao_id );
            glew::test_error( __FILE__, __LINE__ );
            
            // http://www.opengl.org/sdk/docs/man/html/glGenBuffers.xhtml
            //  glew::gl_type::GLsizei n, glew::gl_type::GLuint* buffers
            glew::c::glGenBuffers( 1, &_triangle_buffer_id);
            glew::test_error( __FILE__, __LINE__ );
            
            // http://www.opengl.org/sdk/docs/man/html/glBindBuffer.xhtml
            //  GLenum target, glew::gl_type::GLuint buffer
            glew::c::glBindBuffer( GL_ARRAY_BUFFER, _triangle_buffer_id );
            glew::test_error( __FILE__, __LINE__ );
            
            // http://www.opengl.org/sdk/docs/man/html/glBufferData.xhtml
            //  GLenum target, glew::gl_type::GLsizeiptr size, const GLvoid* data, GLenum usage
            glew::c::glBufferData
            ( GL_ARRAY_BUFFER
            , triangles_buffer.size() * vertex_buffer_t::size_of_memory
            , triangles_buffer.data() -> to_ptr<void>()
            , glew::gl_type::GLenum( GL_STATIC_DRAW )
            );
            glew::test_error( __FILE__, __LINE__ );
            
            // 後始末
            glew::c::glBindBuffer( GL_ARRAY_BUFFER, 0 );
            glew::test_error( __FILE__, __LINE__ );
            
            glew::c::glBindVertexArray( 0 );
            glew::test_error( __FILE__, __LINE__ );
            
            _count_of_triangles = triangles_buffer.size();
            
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
                glew::c::glVertexAttribPointer( location_of_vs_texcoord, vertex_buffer_t::count_of_texcoord_elements, attribute, normalize_off, vertex_buffer_t::size_of_memory, reinterpret_cast<void*>( vertex_buffer_t::memory_offset_of_texcoord ) );
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
            
            // 後始末
            //glBindBuffer( GL_ARRAY_BUFFER, 0 );
            glew::c::glBindVertexArray( 0 );
          }
          
        };
      }
    }
  }
}