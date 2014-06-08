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

#include "../shader.detail/constant.hxx"

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
          glm::vec4 color;
          glm::vec2 texcoord0;
          glm::vec2 texcoord1;
          glm::vec2 texcoord2;
          glm::vec2 texcoord3;
          glm::vec2 texcoord4;
          glm::vec2 texcoord5;
          glm::vec2 texcoord6;
          glm::vec2 texcoord7;
          glm::vec3 normal;
          glm::vec3 tangent;
          glm::vec3 bitangent;
          glm::vec4 bone_ids;
          glm::vec4 bone_weights;
          
          vertex_buffer_t
          ( glm::vec4&& pos
          , glm::vec4&& col
          , glm::vec2&& tex0
          , glm::vec2&& tex1
          , glm::vec2&& tex2
          , glm::vec2&& tex3
          , glm::vec2&& tex4
          , glm::vec2&& tex5
          , glm::vec2&& tex6
          , glm::vec2&& tex7
          , glm::vec3&& nor
          , glm::vec3&& tan
          , glm::vec3&& bit
          , glm::vec4&& bid
          , glm::vec4&& bwt
          )
          : position     ( std::move( pos ) )
          , color        ( std::move( col ) )
          , texcoord0    ( std::move( tex0 ) )
          , texcoord1    ( std::move( tex1 ) )
          , texcoord2    ( std::move( tex2 ) )
          , texcoord3    ( std::move( tex3 ) )
          , texcoord4    ( std::move( tex4 ) )
          , texcoord5    ( std::move( tex5 ) )
          , texcoord6    ( std::move( tex6 ) )
          , texcoord7    ( std::move( tex7 ) )
          , normal       ( std::move( nor ) )
          , tangent      ( std::move( tan ) )
          , bitangent    ( std::move( bit ) )
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
          + sizeof( decltype( color ) )
          + sizeof( decltype( texcoord0 ) )
          + sizeof( decltype( texcoord1 ) )
          + sizeof( decltype( texcoord2 ) )
          + sizeof( decltype( texcoord3 ) )
          + sizeof( decltype( texcoord4 ) )
          + sizeof( decltype( texcoord5 ) )
          + sizeof( decltype( texcoord6 ) )
          + sizeof( decltype( texcoord7 ) )
          + sizeof( decltype( normal ) )
          + sizeof( decltype( tangent ) )
          + sizeof( decltype( bitangent ) )
          + sizeof( decltype( bone_ids ) )
          + sizeof( decltype( bone_weights ) )
          );
          
          static constexpr auto count_of_elements = size_of_memory / size_of_element;
          
          static constexpr auto count_of_position_elements     = sizeof( decltype( position )     ) / size_of_element;
          static constexpr auto count_of_color_elements        = sizeof( decltype( color )        ) / size_of_element;
          static constexpr auto count_of_texcoord0_elements    = sizeof( decltype( texcoord0 )    ) / size_of_element;
          static constexpr auto count_of_texcoord1_elements    = sizeof( decltype( texcoord1 )    ) / size_of_element;
          static constexpr auto count_of_texcoord2_elements    = sizeof( decltype( texcoord2 )    ) / size_of_element;
          static constexpr auto count_of_texcoord3_elements    = sizeof( decltype( texcoord3 )    ) / size_of_element;
          static constexpr auto count_of_texcoord4_elements    = sizeof( decltype( texcoord4 )    ) / size_of_element;
          static constexpr auto count_of_texcoord5_elements    = sizeof( decltype( texcoord5 )    ) / size_of_element;
          static constexpr auto count_of_texcoord6_elements    = sizeof( decltype( texcoord6 )    ) / size_of_element;
          static constexpr auto count_of_texcoord7_elements    = sizeof( decltype( texcoord7 )    ) / size_of_element;
          static constexpr auto count_of_normal_elements       = sizeof( decltype( normal )       ) / size_of_element;
          static constexpr auto count_of_tangent_elements      = sizeof( decltype( tangent )      ) / size_of_element;
          static constexpr auto count_of_bitangent_elements    = sizeof( decltype( bitangent )    ) / size_of_element;
          static constexpr auto count_of_bone_ids_elements     = sizeof( decltype( bone_ids )     ) / size_of_element;
          static constexpr auto count_of_bone_weights_elements = sizeof( decltype( bone_weights ) ) / size_of_element;
          
          static constexpr auto memory_offset_of_position     = std::size_t( 0 );
          static constexpr auto memory_offset_of_color        = memory_offset_of_position  + sizeof( decltype( position  ) );
          static constexpr auto memory_offset_of_texcoord0    = memory_offset_of_color     + sizeof( decltype( color     ) );
          static constexpr auto memory_offset_of_texcoord1    = memory_offset_of_texcoord0 + sizeof( decltype( texcoord0 ) );
          static constexpr auto memory_offset_of_texcoord2    = memory_offset_of_texcoord1 + sizeof( decltype( texcoord1 ) );
          static constexpr auto memory_offset_of_texcoord3    = memory_offset_of_texcoord2 + sizeof( decltype( texcoord2 ) );
          static constexpr auto memory_offset_of_texcoord4    = memory_offset_of_texcoord3 + sizeof( decltype( texcoord3 ) );
          static constexpr auto memory_offset_of_texcoord5    = memory_offset_of_texcoord4 + sizeof( decltype( texcoord4 ) );
          static constexpr auto memory_offset_of_texcoord6    = memory_offset_of_texcoord5 + sizeof( decltype( texcoord5 ) );
          static constexpr auto memory_offset_of_texcoord7    = memory_offset_of_texcoord6 + sizeof( decltype( texcoord6 ) );
          static constexpr auto memory_offset_of_normal       = memory_offset_of_texcoord7 + sizeof( decltype( texcoord7 ) );
          static constexpr auto memory_offset_of_tangent      = memory_offset_of_normal    + sizeof( decltype( normal    ) );
          static constexpr auto memory_offset_of_bitangent    = memory_offset_of_tangent   + sizeof( decltype( tangent   ) );
          static constexpr auto memory_offset_of_bone_ids     = memory_offset_of_bitangent + sizeof( decltype( bitangent ) );
          static constexpr auto memory_offset_of_bone_weights = memory_offset_of_bone_ids  + sizeof( decltype( bone_ids  ) );
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
            
            for ( auto n_vertex = 0u; n_vertex < mesh -> mNumVertices; ++ n_vertex )
              vb.emplace_back
              ( std::move( helper::to_glm_vec4( mesh -> mVertices           + n_vertex ) )
              , std::move( mesh -> mColors[ 0 ]        ? helper::to_glm_vec4( mesh -> mColors[ 0 ]        + n_vertex ) : glm::vec4( std::nanf("") ) )
              , std::move( mesh -> mTextureCoords[ 0 ] ? helper::to_glm_vec2( mesh -> mTextureCoords[ 0 ] + n_vertex ) : glm::vec2( std::nanf("") ) )
              , std::move( mesh -> mTextureCoords[ 1 ] ? helper::to_glm_vec2( mesh -> mTextureCoords[ 1 ] + n_vertex ) : glm::vec2( std::nanf("") ) )
              , std::move( mesh -> mTextureCoords[ 2 ] ? helper::to_glm_vec2( mesh -> mTextureCoords[ 2 ] + n_vertex ) : glm::vec2( std::nanf("") ) )
              , std::move( mesh -> mTextureCoords[ 3 ] ? helper::to_glm_vec2( mesh -> mTextureCoords[ 3 ] + n_vertex ) : glm::vec2( std::nanf("") ) )
              , std::move( mesh -> mTextureCoords[ 4 ] ? helper::to_glm_vec2( mesh -> mTextureCoords[ 4 ] + n_vertex ) : glm::vec2( std::nanf("") ) )
              , std::move( mesh -> mTextureCoords[ 5 ] ? helper::to_glm_vec2( mesh -> mTextureCoords[ 5 ] + n_vertex ) : glm::vec2( std::nanf("") ) )
              , std::move( mesh -> mTextureCoords[ 6 ] ? helper::to_glm_vec2( mesh -> mTextureCoords[ 6 ] + n_vertex ) : glm::vec2( std::nanf("") ) )
              , std::move( mesh -> mTextureCoords[ 7 ] ? helper::to_glm_vec2( mesh -> mTextureCoords[ 7 ] + n_vertex ) : glm::vec2( std::nanf("") ) )
              , std::move( mesh -> mNormals            ? helper::to_glm_vec3( mesh -> mNormals            + n_vertex ) : glm::vec3( std::nanf("") ) )
              , std::move( mesh -> mTangents           ? helper::to_glm_vec3( mesh -> mTangents           + n_vertex ) : glm::vec3( std::nanf("") ) )
              , std::move( mesh -> mBitangents         ? helper::to_glm_vec3( mesh -> mBitangents         + n_vertex ) : glm::vec3( std::nanf("") ) )
              , std::move( glm::vec4( 0.0f ) )
              , std::move( glm::vec4( 0.0f ) )
              );
            
            for ( auto n_face = 0u; n_face < mesh -> mNumFaces; ++ n_face )
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
            
            for ( auto n_bone = 0u; n_bone < mesh -> mNumBones; ++n_bone )
            {
              const auto bone = bones[ n_bone ];
              
              const std::string bone_name( bone -> mName.C_Str() );
              
              unsigned bone_index = 0;
              
              if ( _bone_name_to_bone_index_mapping.find( bone_name ) == _bone_name_to_bone_index_mapping.end() )
              {
                bone_index = _bone_name_to_bone_index_mapping.size();
                _bone_offsets.push_back( glm::mat4( 1.0f ) );
                
                if ( _bone_offsets.size() > shader::max_bones )
                  throw std::runtime_error
                  ( "bone offset size " + std::to_string( _bone_offsets.size() )
                  + " over shader::max_bones " + std::to_string( shader::max_bones )
                  + "."
                  );
              }
              else
                bone_index = _bone_name_to_bone_index_mapping[ bone_name ];
              
              _bone_name_to_bone_index_mapping[ bone_name ] = bone_index;
              
              // TODO: もしかしたら .x 以外では bone_offset に transpose していると怪奇現象化するかも。要確認
              // pattern: .x is ok
              _bone_offsets[ bone_index ] = glm::transpose( helper::to_glm_mat4( bone -> mOffsetMatrix ) );
              //_bone_offsets[ bone_index ] = helper::to_glm_mat4( bone -> mOffsetMatrix );
              
              for ( auto n_weight = 0u; n_weight < bone -> mNumWeights; ++n_weight )
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
          
          auto draw( const glew::gl_type::GLint program_id )
            -> void
          {
            // vertex transfar
            
            constexpr glew::gl_type::GLenum    attribute = GL_FLOAT;
            constexpr glew::gl_type::GLboolean normalize_on  = true;
            constexpr glew::gl_type::GLboolean normalize_off = false;
            
            const auto set_vertex_attribute = [ this, program_id ]
            {
              const auto location_of_vs_position     = glew::c::glGetAttribLocation( program_id, "position"     );
              const auto location_of_vs_color        = glew::c::glGetAttribLocation( program_id, "color"        );
              const auto location_of_vs_texcoord0    = glew::c::glGetAttribLocation( program_id, "texcoord0"    );
              const auto location_of_vs_texcoord1    = glew::c::glGetAttribLocation( program_id, "texcoord1"    );
              const auto location_of_vs_texcoord2    = glew::c::glGetAttribLocation( program_id, "texcoord2"    );
              const auto location_of_vs_texcoord3    = glew::c::glGetAttribLocation( program_id, "texcoord3"    );
              const auto location_of_vs_texcoord4    = glew::c::glGetAttribLocation( program_id, "texcoord4"    );
              const auto location_of_vs_texcoord5    = glew::c::glGetAttribLocation( program_id, "texcoord5"    );
              const auto location_of_vs_texcoord6    = glew::c::glGetAttribLocation( program_id, "texcoord6"    );
              const auto location_of_vs_texcoord7    = glew::c::glGetAttribLocation( program_id, "texcoord7"    );
              const auto location_of_vs_normal       = glew::c::glGetAttribLocation( program_id, "normal"       );
              const auto location_of_vs_tangent      = glew::c::glGetAttribLocation( program_id, "tangent"      );
              const auto location_of_vs_bitangent    = glew::c::glGetAttribLocation( program_id, "bitangent"    );
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
              
              if ( location_of_vs_color not_eq -1 )
              { 
                glew::c::glVertexAttribPointer( location_of_vs_color, vertex_buffer_t::count_of_color_elements, attribute, normalize_off, vertex_buffer_t::size_of_memory, reinterpret_cast<void*>( vertex_buffer_t::memory_offset_of_color ) );
                glew::c::glEnableVertexAttribArray( location_of_vs_color );
              }
              
              if ( location_of_vs_texcoord0 not_eq -1 )
              { 
                glew::c::glVertexAttribPointer( location_of_vs_texcoord0, vertex_buffer_t::count_of_texcoord0_elements, attribute, normalize_off, vertex_buffer_t::size_of_memory, reinterpret_cast<void*>( vertex_buffer_t::memory_offset_of_texcoord0 ) );
                glew::c::glEnableVertexAttribArray( location_of_vs_texcoord0 );
              }
              if ( location_of_vs_texcoord1 not_eq -1 )
              { 
                glew::c::glVertexAttribPointer( location_of_vs_texcoord1, vertex_buffer_t::count_of_texcoord1_elements, attribute, normalize_off, vertex_buffer_t::size_of_memory, reinterpret_cast<void*>( vertex_buffer_t::memory_offset_of_texcoord1 ) );
                glew::c::glEnableVertexAttribArray( location_of_vs_texcoord1 );
              }
              if ( location_of_vs_texcoord2 not_eq -1 )
              { 
                glew::c::glVertexAttribPointer( location_of_vs_texcoord2, vertex_buffer_t::count_of_texcoord2_elements, attribute, normalize_off, vertex_buffer_t::size_of_memory, reinterpret_cast<void*>( vertex_buffer_t::memory_offset_of_texcoord2 ) );
                glew::c::glEnableVertexAttribArray( location_of_vs_texcoord2 );
              }
              if ( location_of_vs_texcoord3 not_eq -1 )
              { 
                glew::c::glVertexAttribPointer( location_of_vs_texcoord3, vertex_buffer_t::count_of_texcoord3_elements, attribute, normalize_off, vertex_buffer_t::size_of_memory, reinterpret_cast<void*>( vertex_buffer_t::memory_offset_of_texcoord3 ) );
                glew::c::glEnableVertexAttribArray( location_of_vs_texcoord3 );
              }
              if ( location_of_vs_texcoord4 not_eq -1 )
              { 
                glew::c::glVertexAttribPointer( location_of_vs_texcoord4, vertex_buffer_t::count_of_texcoord4_elements, attribute, normalize_off, vertex_buffer_t::size_of_memory, reinterpret_cast<void*>( vertex_buffer_t::memory_offset_of_texcoord4 ) );
                glew::c::glEnableVertexAttribArray( location_of_vs_texcoord4 );
              }
              if ( location_of_vs_texcoord5 not_eq -1 )
              { 
                glew::c::glVertexAttribPointer( location_of_vs_texcoord5, vertex_buffer_t::count_of_texcoord5_elements, attribute, normalize_off, vertex_buffer_t::size_of_memory, reinterpret_cast<void*>( vertex_buffer_t::memory_offset_of_texcoord5 ) );
                glew::c::glEnableVertexAttribArray( location_of_vs_texcoord5 );
              }
              if ( location_of_vs_texcoord6 not_eq -1 )
              { 
                glew::c::glVertexAttribPointer( location_of_vs_texcoord6, vertex_buffer_t::count_of_texcoord6_elements, attribute, normalize_off, vertex_buffer_t::size_of_memory, reinterpret_cast<void*>( vertex_buffer_t::memory_offset_of_texcoord6 ) );
                glew::c::glEnableVertexAttribArray( location_of_vs_texcoord6 );
              }
              if ( location_of_vs_texcoord7 not_eq -1 )
              { 
                glew::c::glVertexAttribPointer( location_of_vs_texcoord7, vertex_buffer_t::count_of_texcoord7_elements, attribute, normalize_off, vertex_buffer_t::size_of_memory, reinterpret_cast<void*>( vertex_buffer_t::memory_offset_of_texcoord7 ) );
                glew::c::glEnableVertexAttribArray( location_of_vs_texcoord7 );
              }
              
              if ( location_of_vs_normal not_eq -1 )
              {
                glew::c::glVertexAttribPointer( location_of_vs_normal  , vertex_buffer_t::count_of_normal_elements  , attribute, normalize_on , vertex_buffer_t::size_of_memory, reinterpret_cast<void*>( vertex_buffer_t::memory_offset_of_normal   ) );
                glew::c::glEnableVertexAttribArray( location_of_vs_normal   );
              }
              
              if ( location_of_vs_tangent not_eq -1 )
              {
                glew::c::glVertexAttribPointer( location_of_vs_tangent , vertex_buffer_t::count_of_tangent_elements , attribute, normalize_off , vertex_buffer_t::size_of_memory, reinterpret_cast<void*>( vertex_buffer_t::memory_offset_of_tangent  ) );
                glew::c::glEnableVertexAttribArray( location_of_vs_tangent  );
              }
              
              if ( location_of_vs_bitangent not_eq -1 )
              {
                glew::c::glVertexAttribPointer( location_of_vs_bitangent , vertex_buffer_t::count_of_bitangent_elements , attribute, normalize_off , vertex_buffer_t::size_of_memory, reinterpret_cast<void*>( vertex_buffer_t::memory_offset_of_bitangent  ) );
                glew::c::glEnableVertexAttribArray( location_of_vs_bitangent  );
              }
              
              if ( location_of_vs_bone_ids not_eq -1 )
              {
                glew::c::glVertexAttribPointer( location_of_vs_bone_ids , vertex_buffer_t::count_of_bone_ids_elements , attribute, normalize_off , vertex_buffer_t::size_of_memory, reinterpret_cast<void*>( vertex_buffer_t::memory_offset_of_bone_ids  ) );
                glew::c::glEnableVertexAttribArray( location_of_vs_bone_ids  );
              }
              
              if ( location_of_vs_bone_weights not_eq -1 )
              {
                glew::c::glVertexAttribPointer( location_of_vs_bone_weights , vertex_buffer_t::count_of_bone_weights_elements , attribute, normalize_off , vertex_buffer_t::size_of_memory, reinterpret_cast<void*>( vertex_buffer_t::memory_offset_of_bone_weights  ) );
                glew::c::glEnableVertexAttribArray( location_of_vs_bone_weights  );
              }
            };
            
            // 三角群描画
            {
              glew::c::glBindVertexArray( _triangle_vao_id );
              WRP_GLEW_TEST_ERROR
              
              // http://www.opengl.org/sdk/docs/man/html/glBindBuffer.xhtml
              //  GLenum target, glew::gl_type::GLuint buffer
              glew::c::glBindBuffer( GL_ARRAY_BUFFER, _triangle_vb_id );
              WRP_GLEW_TEST_ERROR
              
              glew::c::glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, _triangle_ib_id );
              WRP_GLEW_TEST_ERROR
              
              set_vertex_attribute();
              WRP_GLEW_TEST_ERROR
              
              // マテリアルの有効化
              auto materia_scopes = _material.draw( program_id );
              WRP_GLEW_TEST_ERROR
              
              // http://www.opengl.org/wiki/GLAPI/glDrawElements
              //  GLenum mode, GLsizei count, GLenum type, const GLvoid* indices
              glew::c::glDrawElements( GL_TRIANGLES, _count_of_indices, GL_UNSIGNED_INT, nullptr );
              WRP_GLEW_TEST_ERROR
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