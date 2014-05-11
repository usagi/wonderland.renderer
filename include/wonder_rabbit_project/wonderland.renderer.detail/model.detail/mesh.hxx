#pragma once

#include <unordered_map>

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
#include "bone.hxx"
#include "animation_data.hxx"

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      namespace model
      {
        
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
          auto to_ptr() const
          -> const T*
          { return reinterpret_cast< const T* >( &position.x ); }
          
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
        
        class mesh_t
        {
          friend class node_t;
          
          glew::gl_type::GLuint  _triangle_vb_id;
          glew::gl_type::GLuint  _triangle_ib_id;
          glew::gl_type::GLsizei _count_of_indices;
          
          glew::gl_type::GLuint  _triangle_vao_id;
          
          const material_t&      _material;
          
          // ボーンデータ
          std::vector< glm::mat4 >& _bone_offsets;
          
          std::unordered_map< std::string, unsigned >& _bone_name_to_bone_index_mapping;
          
          const std::unordered_map< std::string, animation_t >& _animations;
          
          using vertices_buffer_t = std::vector< vertex_buffer_t >;
          using indices_buffer_t  = std::vector< glew::gl_type::GLuint >;
          
          auto initialize_prepare_buffers( const aiMesh* mesh, vertices_buffer_t& vb, indices_buffer_t& ib )
            -> void
          {
            
            constexpr auto indices_of_triangle = 3;
            
            for ( auto n_vertex = 0; n_vertex < mesh -> mNumVertices; ++ n_vertex )
              vb.emplace_back
              ( std::move( helper::to_glm_vec4( mesh -> mVertices           + n_vertex ) )
              , std::move( helper::to_glm_vec2( mesh -> mTextureCoords[ 0 ] + n_vertex ) )
              , std::move( helper::to_glm_vec3( mesh -> mNormals            + n_vertex ) )
              , std::move( helper::to_glm_vec3( mesh -> mTangents           + n_vertex ) )
              , std::move( glm::vec4( 0.0f ) )
              , std::move( glm::vec4( 0.0f ) )
              );
            
            for ( auto n_face = 0; n_face < mesh -> mNumFaces; ++ n_face )
            {
              const auto face = mesh -> mFaces + n_face;
              
              if ( face -> mNumIndices not_eq indices_of_triangle )
                throw std::runtime_error( "required must be indices of face is 3. try create_model with aiProcess_Triangulate." );
              
              ib.emplace_back( std::move( face -> mIndices[0] ) );
              ib.emplace_back( std::move( face -> mIndices[1] ) );
              ib.emplace_back( std::move( face -> mIndices[2] ) );
              
            }
            
            _count_of_indices = ib.size();
            
          }
          
          auto initialize_animation_bone
          ( const aiMesh* mesh
          , vertices_buffer_t& vb
          )
            -> void
          {
            // アニメーション・ボーンまわり
            
            const auto bones = mesh -> mBones;
            
            for ( auto n_bone = 0; n_bone < mesh -> mNumBones; ++n_bone )
            {
              const auto bone = bones[ n_bone ];
              
              const std::string bone_name( bone -> mName.C_Str() );
              
              unsigned bone_index = 0;
              
              if ( _bone_name_to_bone_index_mapping.find( bone_name ) == _bone_name_to_bone_index_mapping.end() )
              {
                bone_index = _bone_name_to_bone_index_mapping.size();
                _bone_offsets.push_back( glm::mat4( 1.0f ) );
              }
              else
                bone_index = _bone_name_to_bone_index_mapping[ bone_name ];
              
              _bone_name_to_bone_index_mapping[ bone_name ] = bone_index;
              
              // TODO: もしかしたら .x 以外では bone_offset に transpose していると怪奇現象化するかも。要確認
              _bone_offsets[ bone_index ] = glm::transpose( helper::to_glm_mat4( bone -> mOffsetMatrix ) );
              
              for ( auto n_weight = 0; n_weight < bone -> mNumWeights; ++n_weight )
              {
                const auto& weight = bone -> mWeights[ n_weight ];
                
                auto& vertex = vb[ weight.mVertexId ];
                bool overflow_check = true;
                
                for ( auto n = 0; n < 4; ++n )
                  if ( vertex.bone_weights[ n ] == 0.0f )
                  {
                    vertex.bone_ids    [ n ] = bone_index;
                    vertex.bone_weights[ n ] = weight.mWeight;
                    overflow_check = false;
                    break;
                  }
                  
                  if ( overflow_check )
                    throw std::runtime_error( "bone buffer is not enought, need limit data bone/vertex <= 4, or fix engine." );
              }
            }
          }
          
          auto initialize_generate_buffers( const vertices_buffer_t& vb, const indices_buffer_t& ib )
            -> void
          {
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
            glew::c::glGenBuffers( 1, &_triangle_vb_id);
            glew::test_error( __FILE__, __LINE__ );
            
            glew::c::glGenBuffers( 1, &_triangle_ib_id);
            glew::test_error( __FILE__, __LINE__ );
            
            // http://www.opengl.org/sdk/docs/man/html/glBindBuffer.xhtml
            //  GLenum target, glew::gl_type::GLuint buffer
            glew::c::glBindBuffer( GL_ARRAY_BUFFER, _triangle_vb_id );
            glew::test_error( __FILE__, __LINE__ );
            
            glew::c::glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, _triangle_ib_id );
            glew::test_error( __FILE__, __LINE__ );
            
            // http://www.opengl.org/sdk/docs/man/html/glBufferData.xhtml
            //  GLenum target, glew::gl_type::GLsizeiptr size, const GLvoid* data, GLenum usage
            glew::c::glBufferData
            ( GL_ARRAY_BUFFER
            , vb.size() * vertex_buffer_t::size_of_memory
            , vb.data() -> to_ptr<void>()
            , glew::gl_type::GLenum( GL_STATIC_DRAW )
            );
            glew::test_error( __FILE__, __LINE__ );
            
            glew::c::glBufferData
            ( GL_ELEMENT_ARRAY_BUFFER
            , ib.size() * sizeof( indices_buffer_t::value_type )
            , reinterpret_cast< const void* >( ib.data() )
            , glew::gl_type::GLenum( GL_STATIC_DRAW )
            );
            glew::test_error( __FILE__, __LINE__ );
            
            // 後始末
            glew::c::glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
            glew::test_error( __FILE__, __LINE__ );
            
            glew::c::glBindBuffer( GL_ARRAY_BUFFER, 0 );
            glew::test_error( __FILE__, __LINE__ );
            
            glew::c::glBindVertexArray( 0 );
            glew::test_error( __FILE__, __LINE__ );
          }
          
        public:
          
          ~mesh_t()
          {
            // http://www.opengl.org/sdk/docs/man/html/glDeleteBuffers.xhtml
            //  glew::gl_type::GLsizei n, const glew::gl_type::GLuint buffers
            glew::c::glDeleteBuffers( 1, &_triangle_ib_id );
            glew::c::glDeleteBuffers( 1, &_triangle_vb_id );
            
            // http://www.opengl.org/sdk/docs/man/html/glDeleteVertexArrays.xhtml
            //  glew::gl_type::GLsizei n, const glew::gl_type::GLuint* arrays
            glew::c::glDeleteVertexArrays( 1, &_triangle_vao_id );
          }
          
          mesh_t
          ( const aiMesh* mesh
          , const std::vector<material_t>& materials_
          , std::vector< glm::mat4 >& bone_offsets
          , std::unordered_map< std::string, unsigned >& bone_name_to_bone_index_mapping
          , const std::unordered_map< std::string, animation_t >& animations
          )
            : _material( materials_.at( mesh -> mMaterialIndex ) )
            , _bone_offsets( bone_offsets )
            , _bone_name_to_bone_index_mapping( bone_name_to_bone_index_mapping )
            , _animations( animations )
          {
            vertices_buffer_t vb;
            indices_buffer_t ib;
            
            initialize_prepare_buffers ( mesh, vb, ib );
            initialize_animation_bone  ( mesh, vb     );
            initialize_generate_buffers(       vb, ib );
          }
          
          auto draw
          ( const animation_states_t& animation_states
          , glew::gl_type::GLint program_id
          )
            -> void
          {
            // vertex transfar
            
            constexpr glew::gl_type::GLenum    attribute = GL_FLOAT;
            constexpr glew::gl_type::GLboolean normalize_on  = true;
            constexpr glew::gl_type::GLboolean normalize_off = false;
            
            const auto set_vertex_attribute = [ this, program_id ]
            {
              const auto location_of_vs_position     = glew::c::glGetAttribLocation( program_id, "position"     );
              const auto location_of_vs_texcoord     = glew::c::glGetAttribLocation( program_id, "texcoord"     );
              const auto location_of_vs_normal       = glew::c::glGetAttribLocation( program_id, "normal"       );
              const auto location_of_vs_tangent      = glew::c::glGetAttribLocation( program_id, "tangent"      );
              const auto location_of_vs_bone_ids     = glew::c::glGetAttribLocation( program_id, "bone_ids"     );
              const auto location_of_vs_bone_weights = glew::c::glGetAttribLocation( program_id, "bone_weights" );
              
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
              
              if ( location_of_vs_bone_ids not_eq -1 )
              {
                glew::c::glVertexAttribPointer( location_of_vs_bone_ids , vertex_buffer_t::count_of_bone_ids_elements , attribute, normalize_on , vertex_buffer_t::size_of_memory, reinterpret_cast<void*>( vertex_buffer_t::memory_offset_of_bone_ids  ) );
                glew::c::glEnableVertexAttribArray( location_of_vs_bone_ids  );
              }
              
              if ( location_of_vs_bone_weights not_eq -1 )
              {
                glew::c::glVertexAttribPointer( location_of_vs_bone_weights , vertex_buffer_t::count_of_bone_weights_elements , attribute, normalize_on , vertex_buffer_t::size_of_memory, reinterpret_cast<void*>( vertex_buffer_t::memory_offset_of_bone_weights  ) );
                glew::c::glEnableVertexAttribArray( location_of_vs_bone_weights  );
              }
              
              // マテリアルの有効化
              _material.draw();
            };
            
            // 三角群描画
            {
              glew::c::glBindVertexArray( _triangle_vao_id );
              
              // http://www.opengl.org/sdk/docs/man/html/glBindBuffer.xhtml
              //  GLenum target, glew::gl_type::GLuint buffer
              glew::c::glBindBuffer( GL_ARRAY_BUFFER, _triangle_vb_id );
              
              glew::c::glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, _triangle_ib_id );
              
              set_vertex_attribute();
              
              // http://www.opengl.org/wiki/GLAPI/glDrawElements
              //  GLenum mode, GLsizei count, GLenum type, const GLvoid* indices
              glew::c::glDrawElements( GL_TRIANGLES, _count_of_indices, GL_UNSIGNED_INT, nullptr );
            }
            
            // 後始末
            glew::c::glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
            glew::c::glBindBuffer( GL_ARRAY_BUFFER, 0 );
            glew::c::glBindVertexArray( 0 );
          }
          
        };
      }
    }
  }
}