#pragma once

#include <boost/gil/gil_all.hpp>

// assimp::Importer
#include <assimp/Importer.hpp>
// aiPostProcessSteps for assimp::Importer::ReadFile
#include <assimp/postprocess.h>
// aiScene, aiNode
#include <assimp/scene.h>

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
        
        // assimp 依存のモデルデータ構造保持用型
        
        // モデルデータ（メッシュ群とノードグラフを管理するルートオブジェクト）
        class model_t;
        // メッシュデータ（モデルデータに含まれるメッシュ群を構成するメッシュデータ）
        class mesh_t;
        // ノードデータ（モデルデータに含まれるノードグラフを構成するノードデータ）
        class node_t;
        
        class mesh_t
        {
          // TODO: このモデルデータの扱いは wonderland.renderer に将来的に内蔵されるので今はこれで許してね・ｗ・
          using GLuint  = wonder_rabbit_project::wonderland::renderer::glew::gl_type::GLuint;
          using GLsizei = wonder_rabbit_project::wonderland::renderer::glew::gl_type::GLsizei;
          
          GLuint  _triangle_buffer_id;
          GLsizei _count_of_triangles;
          GLuint  _quad_buffer_id;
          GLsizei _count_of_quads;
          
          GLuint  _triangle_vao_id;
          GLuint  _quad_vao_id;
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
            { return reinterpret_cast<T*>(&position.x); }
            
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
            
            static constexpr auto memory_offset_of_position = std::size_t(0);
            static constexpr auto memory_offset_of_texcorrd = memory_offset_of_position + sizeof( position_t );
            static constexpr auto memory_offset_of_normal   = memory_offset_of_texcorrd + sizeof( texcoord_t );
            static constexpr auto memory_offset_of_tangent  = memory_offset_of_normal   + sizeof( normal_t   );
          };
          
          ~mesh_t()
          {
            // TODO: このモデルデータの扱いは wonderland.renderer に将来的に内蔵されるので今はこれで許してね・ｗ・
            using namespace wonder_rabbit_project::wonderland::renderer::glew::c;
            
            // http://www.opengl.org/sdk/docs/man/html/glDeleteBuffers.xhtml
            //  GLsizei n, const GLuint buffers
            glDeleteBuffers( 1, &_triangle_buffer_id );
            glDeleteBuffers( 1, &_quad_buffer_id );
            // http://www.opengl.org/sdk/docs/man/html/glDeleteVertexArrays.xhtml
            //  GLsizei n, const GLuint* arrays
            glDeleteVertexArrays( 1, &_triangle_vao_id );
            glDeleteVertexArrays( 1, &_quad_vao_id );
          }
          
          mesh_t( const aiMesh* mesh )
          {
            // TODO: このモデルデータの扱いは wonderland.renderer に将来的に内蔵されるので今はこれで許してね・ｗ・
            using namespace wonder_rabbit_project::wonderland::renderer::glew::c;
            using namespace wonder_rabbit_project::wonderland::renderer::glew;
            
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
            //  GLsizei n, GLuint* arrays
            glGenVertexArrays( 1, &_triangle_vao_id );
            // http://www.opengl.org/sdk/docs/man/html/glBindVertexArray.xhtml
            //  GLuint array
            glBindVertexArray( _triangle_vao_id );
            // http://www.opengl.org/sdk/docs/man/html/glGenBuffers.xhtml
            //  GLsizei n, GLuint* buffers
            glGenBuffers( 1, &_triangle_buffer_id);
            // http://www.opengl.org/sdk/docs/man/html/glBindBuffer.xhtml
            //  GLenum target, GLuint buffer
            glBindBuffer( GL_ARRAY_BUFFER, _triangle_buffer_id );
            // http://www.opengl.org/sdk/docs/man/html/glBufferData.xhtml
            //  GLenum target, GLsizeiptr size, const GLvoid* data, GLenum usage
            glBufferData
            ( GL_ARRAY_BUFFER
            , triangles_buffer.size() * vertex_buffer_t::size_of_memory
            , triangles_buffer.data() -> to_ptr<void>()
            , gl_type::GLenum( USAGE::STATIC_DRAW )
            );
            // 後始末
            glBindBuffer( GL_ARRAY_BUFFER, 0 );
            glBindVertexArray( 0 );
            _count_of_triangles = triangles_buffer.size();
            
            // 四角群のバッファーを生成
            glGenVertexArrays( 1, &_quad_vao_id );
            glBindVertexArray( _quad_vao_id );
            glGenBuffers( 1, &_quad_buffer_id);
            glBindBuffer( GL_ARRAY_BUFFER, _quad_buffer_id );
            glBufferData
            ( GL_ARRAY_BUFFER
            , quads_buffer.size() * vertex_buffer_t::size_of_memory
            , quads_buffer.data() -> to_ptr<void>()
            , gl_type::GLenum( USAGE::STATIC_DRAW )
            );
            glBindBuffer( GL_ARRAY_BUFFER, 0 );
            glBindVertexArray( 0 );
            _count_of_quads = quads_buffer.size();
          }
          
          auto operator()() const
          -> void
          {
            // TODO: このモデルデータの扱いは wonderland.renderer に将来的に内蔵されるので今はこれで許してね・ｗ・
            using namespace wonder_rabbit_project::wonderland::renderer::glew::c;
            using namespace wonder_rabbit_project::wonderland::renderer::glew;
            constexpr gl_type::GLenum gl_float = gl_type::GLenum( VERTEX_ATTRIBUTE::FLOAT );
            
            const auto set_vertex_attribute = []
            {
              // http://www.opengl.org/sdk/docs/man/html/glVertexAttribPointer.xhtml
              //  GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer
              glVertexAttribPointer( 0, vertex_buffer_t::count_of_position_elements, gl_type::GLenum(VERTEX_ATTRIBUTE::FLOAT), false, vertex_buffer_t::size_of_memory, reinterpret_cast<void*>( vertex_buffer_t::memory_offset_of_position ) );
              glVertexAttribPointer( 2, vertex_buffer_t::count_of_texcoord_elements, gl_type::GLenum(VERTEX_ATTRIBUTE::FLOAT), false, vertex_buffer_t::size_of_memory, reinterpret_cast<void*>( vertex_buffer_t::memory_offset_of_texcorrd ) );
              glVertexAttribPointer( 1, vertex_buffer_t::count_of_normal_elements  , gl_type::GLenum(VERTEX_ATTRIBUTE::FLOAT), true , vertex_buffer_t::size_of_memory, reinterpret_cast<void*>( vertex_buffer_t::memory_offset_of_normal   ) );
              glVertexAttribPointer( 3, vertex_buffer_t::count_of_tangent_elements , gl_type::GLenum(VERTEX_ATTRIBUTE::FLOAT), true , vertex_buffer_t::size_of_memory, reinterpret_cast<void*>( vertex_buffer_t::memory_offset_of_tangent  ) );
              // http://www.opengl.org/sdk/docs/man/html/glEnableVertexAttribArray.xhtml
              //  GLuint index
              glEnableVertexAttribArray( 0 );
              glEnableVertexAttribArray( 1 );
              glEnableVertexAttribArray( 2 );
              glEnableVertexAttribArray( 3 );
            };
            
            // 三角群描画
            {
              glBindVertexArray( _triangle_vao_id );
              // http://www.opengl.org/sdk/docs/man/html/glBindBuffer.xhtml
              //  GLenum target, GLuint buffer
              glBindBuffer( GL_ARRAY_BUFFER, _triangle_buffer_id );
              set_vertex_attribute();
              // http://www.opengl.org/sdk/docs/man/html/glDrawArrays.xhtml
              //  GLenum mode, GLint first, GLsizei count
              glDrawArrays( GL_TRIANGLES, 0, _count_of_triangles );
            }
            
            // 四角群描画
            {
              glBindVertexArray( _quad_vao_id );
              glBindBuffer( GL_ARRAY_BUFFER, _quad_buffer_id );
              set_vertex_attribute();
              glDrawArrays( GL_QUADS, 0, _count_of_quads );
            }
            
            // 後始末
            //glBindBuffer( GL_ARRAY_BUFFER, 0 );
            glBindVertexArray( 0 );
          }
          
        };
        
        class node_t
        {
          std::vector< node_t > _nodes;
          std::vector< int    > _indices;
          glm::mat4             _transformation;
        public:
          node_t( const aiNode* node )
          {
            _nodes.reserve( node -> mNumMeshes );
            for ( auto n = 0; n < node -> mNumChildren; ++n)
              _nodes.emplace_back( node -> mChildren[ n ] );
            
            _indices.reserve( node -> mNumMeshes );
            for ( auto n = 0; n < node -> mNumMeshes; ++n )
              _indices.emplace_back( node -> mMeshes[ n ] );
            
            // Assimp::aiMatrix4x4t
            //  http://assimp.sourceforge.net/lib_html/classai_matrix4x4t.html
            // glm::mat4
            //  https://github.com/g-truc/glm/blob/0.9.5/glm/detail/type_mat4x4.hpp
            const auto& nt = node -> mTransformation;
            _transformation = glm::mat4
            ( nt[0][0], nt[0][1], nt[0][2], nt[0][3]
            , nt[1][0], nt[1][1], nt[1][2], nt[1][3]
            , nt[2][0], nt[2][1], nt[2][2], nt[2][3]
            , nt[3][0], nt[3][1], nt[3][2], nt[3][3]
            );
          }
          
          auto operator()( const std::vector< mesh_t >& meshes ) const
          -> void
          {
            // TODO: このモデルデータの扱いは wonderland.renderer に将来的に内蔵されるので今はこれで許してね・ｗ・
            using namespace wonder_rabbit_project::wonderland::renderer::glew::c;
            
            #ifndef EMSCRIPTEN
            glPushMatrix();
            #endif
            glMultMatrixf( &_transformation[0][0] );
            
            for ( const auto& node : _nodes )
              node( meshes );
            for ( const auto& mesh : meshes )
              mesh();
            
            #ifndef EMSCRIPTEN
            glPopMatrix();
            #endif
          }
        };
        
        class model_t
        {
          // モデルデータに含まれるメッシュ群を格納
          std::vector<mesh_t> _meshes;
          // ノード
          node_t _node;
        public:
          
          // const aiScene* からモデルデータ（メッシュ群、ノードグラフ）を生成
          explicit model_t( const aiScene* scene )
            : _node( scene -> mRootNode )
          {
            // シーンからメッシュサイズを予約確保
            _meshes.reserve( scene -> mNumMeshes );
            
            // シーンからメッシュ群を _meshes に生成
            for( auto n = 0; n < scene -> mNumMeshes; ++n )
              _meshes.emplace_back( scene -> mMeshes[ n ] );
          }
          
          // 描画
          auto draw() const
            -> void
          { _node( _meshes ); }
          
          static constexpr unsigned int default_importer_readfile_flags
            = aiProcess_CalcTangentSpace
            | aiProcess_JoinIdenticalVertices
            | aiProcess_GenNormals
            | aiProcess_GenSmoothNormals
            | aiProcess_PreTransformVertices
            | aiProcess_ValidateDataStructure
            | aiProcess_ImproveCacheLocality
            | aiProcess_RemoveRedundantMaterials
            | aiProcess_FixInfacingNormals
            | aiProcess_SortByPType
            | aiProcess_FindInvalidData
            | aiProcess_GenUVCoords
            | aiProcess_FindInstances
            | aiProcess_OptimizeMeshes
            | aiProcess_OptimizeGraph
            ;
          
          // ファイルからモデルデータを生成
          static auto create( const std::string& file_path, unsigned int importer_readfile_flags = default_importer_readfile_flags )
          -> model_t
          {
            // Assimp::Importer ctor
            //  http://assimp.sourceforge.net/lib_html/class_assimp_1_1_importer.html#a2c207299ed05f1db1ad1e6dab005f719
            //  ctorはパラメーター無し版とコピーctr版のみ。
            Assimp::Importer i;
            
            // Assimp::Importer::ReadFile
            //  http://assimp.sourceforge.net/lib_html/class_assimp_1_1_importer.html#a174418ab41d5b8bc51a044895cb991e5
            //  C文字列版、std::string版があり、ReadFileの他にメモリー上の任意のアドレスから読み込むReadFileMemoryもある。
            //  第二パラメーターで以下のポストプロセスのフラグを扱える。
            //    http://assimp.sourceforge.net/lib_html/postprocess_8h.html#a64795260b95f5a4b3f3dc1be4f52e410
            //      aiProcess_CalcTangentSpace        : 接線空間（法線・従法線）を計算
            //      aiProcess_JoinIdenticalVertices   : 重複する頂点座標の削除
            //      aiProcess_MakeLeftHanded          : 左手座標系へ変換
            //      aiProcess_Triangulate             : 三角面へ変換
            //      aiProcess_RemoveComponent         : コンポーネント（アニメーション、材質、光源、カメラ、テクスチャー、頂点）を除去
            //      aiProcess_GenNormals              : 面法線を生成
            //      aiProcess_GenSmoothNormals        : スムーズ法線を頂点に生成
            //      aiProcess_SplitLargeMeshes        : 巨大なメッシュをサブメッシュに分割
            //      aiProcess_PreTransformVertices    : ノードグラフから変形前のローカル頂点座標と変形行列を除去
            //      aiProcess_LimitBoneWeights        : １つの頂点に影響するボーン数を制限
            //      aiProcess_ValidateDataStructure   : データ構造の整合性を確認
            //      aiProcess_ImproveCacheLocality    : 頂点キャッシュの局所性により三角形を最適化
            //      aiProcess_RemoveRedundantMaterials: 冗長または未使用の材質を除去
            //      aiProcess_FixInfacingNormals      : 面法線が内向きの場合に逆転させる
            //      aiProcess_SortByPType             : 異なる複数のプリミティブ型からなるメッシュをサブメッシュに分割
            //      aiProcess_FindDegenerates         : メッシュを線と点にする
            //      aiProcess_FindInvalidData         : 不正な法線や不正なUV座標を除去または修正する
            //      aiProcess_GenUVCoords             : 非UVマップ（球状または円筒状）からUVマップへ変換
            //      aiProcess_TransformUVCoords       : UV変形を適用する
            //      aiProcess_FindInstances           : 重複するメッシュを探し初出のメッシュに置き換える
            //      aiProcess_OptimizeMeshes          : メッシュ最適化を行う
            //      aiProcess_OptimizeGraph           : シーングラフ最適化を行う
            //      aiProcess_FlipUVs                 : UV座標を反転
            //      aiProcess_FlipWindingOrder        : 面生成を時計回りにする
            //      aiProcess_SplitByBoneCount        : ボーン数によりメッシュを分割
            //      aiProcess_Debone                  : ボーンをロスレスないし閾値まで除去
            //  このポストプロセスは ApplyPostProcessing() を後で呼んで行う事もできる。
            auto scene = i.ReadFile( file_path, importer_readfile_flags );
            return model_t( scene );
          }
          
        };
        
        /*
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
            
            //model_invoke_vertex_attribute_pointer<vertex_type>::invoke();
            c::glVertexAttribPointer(0, 2, gl_type::GLenum(VERTEX_ATTRIBUTE::FLOAT), false, sizeof(float)*4, 0);
            c::glVertexAttribPointer(2, 2, gl_type::GLenum(VERTEX_ATTRIBUTE::FLOAT), false, sizeof(float)*4, reinterpret_cast<void*>(sizeof(float)*2) );
            c::glEnableVertexAttribArray(0);
            c::glEnableVertexAttribArray(2);
            
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
            //model_invoke_vertex_attribute_pointer<vertex_type>::invoke();
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
        */
      }
    }
  }
}