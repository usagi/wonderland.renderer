#pragma once

#include <map>
#include <unordered_map>
#include <functional>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/mat4x4.hpp>

// assimp::Importer
#include <assimp/Importer.hpp>
// aiPostProcessSteps for assimp::Importer::ReadFile
#include <assimp/postprocess.h>
// aiScene, aiNode
#include <assimp/scene.h>

//#include "glew.detail/c.hxx"
//#include "glew.detail/gl_type.hxx"

#include "helper.hxx"
#include "material.hxx"
#include "mesh.hxx"
#include "node.hxx"
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
        class model_t
        {
          // アニメーションデータ
          std::unordered_map< std::string, animation_t > _animations;
          
          // ボーン名 -> ボーンID
          std::unordered_map< std::string, unsigned > _bone_name_to_bone_index_mapping;
          
          // ボーンオフセット群
          std::vector< glm::mat4 > _bone_offsets;
          
          // アニメーション名
          std::vector< std::string > _animation_names;
          
          // モデルデータに含まれるマテリアル群を格納
          std::vector< material_t > _materials;
          
          // モデルデータに含まれるメッシュ群を格納
          std::vector< mesh_t > _meshes;
          
          // ノード
          node_t _node;
          
          // ルートノードの変形行列の逆行列（アニメーション適用で使用）
          glm::mat4 _global_inverse_transformation;
          
          inline auto apply_animation_recursive
          ( const animation_states_t& animation_states
          , const node_t& node
          , std::vector< glm::mat4 >& bone_animation_transformations
          , const glm::mat4& parent_transformation
          )
            -> void
          {
            for ( auto& animation_state : animation_states )
            {
              glm::mat4 node_transformation( node.transformation() );
              
              auto animation_iterator = _animations.find( animation_state.name );
              
              if( animation_iterator not_eq _animations.end() )
                node_transformation = animation_iterator -> second.transformation( node.name(), animation_state.time );
              else
                continue;
              
              const auto global_transformation = parent_transformation * node_transformation;
              
              const auto bone_name_to_index_iterator = _bone_name_to_bone_index_mapping.find( node.name() );
              if( bone_name_to_index_iterator not_eq _bone_name_to_bone_index_mapping.end() )
              {
                const auto bone_index = bone_name_to_index_iterator -> second;
                bone_animation_transformations[ bone_index ]
                  = _global_inverse_transformation * global_transformation * _bone_offsets[ bone_index ];
              }
              
              for ( const auto& child_node : node.nodes() )
                apply_animation_recursive( animation_states, child_node, bone_animation_transformations, global_transformation );
              
              // TODO: 複数のアニメーションの合成に対応する。現状: 最初の１つで break
              break;
            }
          }
          
          inline auto apply_animation( const animation_states_t& animation_states, const glew::gl_type::GLint program_id )
            -> void
          {
            const auto location_of_animate = glew::c::glGetUniformLocation( program_id, "animate" );
            
            if ( _bone_offsets.empty() or animation_states.empty() or animation_states.empty() )
              return;
            
            // update animation matrices
            
            // アニメーション最終変形行列
            std::vector< glm::mat4 > bone_animation_transformations;
            bone_animation_transformations.resize( _bone_offsets.size(), glm::mat4( 1.0f ) );
            
            apply_animation_recursive( animation_states, _node, bone_animation_transformations, glm::mat4( 1.0f ) );
            
            const auto location_of_bones = glew::c::glGetUniformLocation( program_id, "bones" );
            
            if ( location_of_bones not_eq -1 )
              glew::c::glUniformMatrix4fv
              ( location_of_bones
              , bone_animation_transformations.size()
              , false
              , &bone_animation_transformations[0][0][0]
              );
          }
          
        public:
          
          // const aiScene* からモデルデータ（メッシュ群、ノードグラフ）を生成
          explicit model_t( const aiScene* scene, const std::string& path_prefix = "", const bool transpose_node = false )
            : _node( scene -> mRootNode, _animations, transpose_node )
            , _global_inverse_transformation( glm::inverse( helper::to_glm_mat4( scene -> mRootNode -> mTransformation ) ) )
          {
            // シーンからマテリアル群を _materials に生成
            _materials.reserve( scene -> mNumMaterials );
            for ( auto n = 0; n < scene -> mNumMaterials; ++n )
              _materials.emplace_back( scene -> mMaterials[ n ], path_prefix );

            // シーンからメッシュ群を _meshes に生成
            _meshes.reserve( scene -> mNumMeshes );
            for ( auto n = 0; n < scene -> mNumMeshes; ++n )
              _meshes.emplace_back
              ( scene -> mMeshes[ n ]
              , _materials
              , _bone_offsets
              , _bone_name_to_bone_index_mapping
              , _animations
              );
            
            // アニメーション情報群を保存
            for ( auto n = 0; n < scene -> mNumAnimations; ++n )
            {
              const auto animation = scene -> mAnimations[ n ];
              
              animation_t data;
              
              data.duration         = animation -> mDuration;
              data.ticks_per_second = animation -> mTicksPerSecond == 0.0f ? 25.0f : animation -> mTicksPerSecond;
              
              for ( auto n_channels = 0; n_channels < animation -> mNumChannels; ++n_channels )
              {
                const auto channel = animation -> mChannels[ n_channels ];
                
                animation_t::channel_t ch;
                
                for ( auto n_key = 0; n_key < channel -> mNumScalingKeys; ++ n_key )
                {
                  const auto key = channel -> mScalingKeys[ n_key ];
                  ch.scalings.emplace( std::move( key.mTime ), helper::to_glm_vec3( &key.mValue ) );
                }
                
                for ( auto n_key = 0; n_key < channel -> mNumRotationKeys; ++ n_key )
                {
                  const auto key = channel -> mRotationKeys[ n_key ];
                  ch.rotations.emplace( std::move( key.mTime ), helper::to_glm_quat( &key.mValue ) );
                }
                
                for ( auto n_key = 0; n_key < channel -> mNumPositionKeys; ++ n_key )
                {
                  const auto key = channel -> mPositionKeys[ n_key ];
                  ch.translations.emplace( std::move( key.mTime ), helper::to_glm_vec3( &key.mValue ) );
                }
                
                data.channels.emplace( std::string( channel -> mNodeName.C_Str() ), std::move( ch ) );
              }
              
              const std::string animation_name = animation -> mName.C_Str();
              
              _animation_names.emplace_back( animation_name );
              
              _animations.emplace
              ( std::move( animation_name )
              , std::move( data )
              );
            }
          }
          
          // 描画
          auto draw( const animation_states_t& animation_states = animation_states_t() )
            -> void
          {
            glew::gl_type::GLint program_id;
            glew::c::glGetIntegerv( GL_CURRENT_PROGRAM, &program_id );
            
            apply_animation( animation_states, program_id );
            
            _node.draw
            ( _meshes
            , animation_states
            , program_id
            );
          }
          
          // ボーンの名前のリストを提供
          auto bone_names()
            -> std::vector< std::string >
          {
            std::vector< std::string > r;
            r.reserve( _bone_name_to_bone_index_mapping.size() );
            for ( const auto& m : _bone_name_to_bone_index_mapping )
              r.emplace_back( m.first );
            return r;
          }
          
          // アニメーションの名前のリストを提供
          auto animation_names()
            -> std::vector< std::string >
          { return _animation_names; }
          
          static constexpr unsigned int default_importer_readfile_flags
          = aiProcess_CalcTangentSpace
          //| aiProcess_JoinIdenticalVertices // 有効化すると結果的にボーン情報が削除される
          | aiProcess_GenNormals
          | aiProcess_GenSmoothNormals
          //| aiProcess_PreTransformVertices // 有効化するとアニメーション関連情報が削除される
          | aiProcess_ValidateDataStructure
          | aiProcess_ImproveCacheLocality
          | aiProcess_RemoveRedundantMaterials
          | aiProcess_FixInfacingNormals
          //| aiProcess_SortByPType
          | aiProcess_FindInvalidData
          | aiProcess_GenUVCoords
          | aiProcess_FindInstances
          | aiProcess_OptimizeMeshes
          | aiProcess_OptimizeGraph
          | aiProcess_Triangulate
          ;
          
          // ファイルからモデルデータを生成
          static auto create( const std::string& file_path, unsigned int importer_readfile_flags = default_importer_readfile_flags )
          -> model_t
          {
            // Assimp::Importer ctor
            //  http://assimp.sourceforge.net/lib_html/class_assimp_1_1_importer.html#a2c207299ed05f1db1ad1e6dab005f719
            //  ctorはパラメーター無し版とコピーctor版のみ。
            Assimp::Importer i;
            
            // Assimp::Importer::ReadFile
            //  http://assimp.sourceforge.net/lib_html/class_assimp_1_1_importer.html#a174418ab41d5b8bc51a044895cb991e5
            //  C文字列版、std::string版があり、ReadFileの他にメモリー上の任意のアドレスから読み込むReadFileMemoryもある。
            //  第二パラメーターで以下のポストプロセスのフラグを扱える。
            //    http://assimp.sourceforge.net/lib_html/postprocess_8h.html#a64795260b95f5a4b3f3dc1be4f52e410
            //      aiProcess_CalcTangentSpace        : 接線空間（タンジェント・バイタンジェント）を計算
            //      aiProcess_JoinIdenticalVertices   : 重複する頂点座標の削除
            //      aiProcess_MakeLeftHanded          : 左手座標系へ変換
            //      aiProcess_Triangulate             : 三角面へ変換
            //      aiProcess_RemoveComponent         : コンポーネント（アニメーション、材質、光源、カメラ、テクスチャー、頂点）を除去
            //      aiProcess_GenNormals              : 面法線を生成
            //      aiProcess_GenSmoothNormals        : スムーズ法線を頂点に生成
            //      aiProcess_SplitLargeMeshes        : 巨大なメッシュをサブメッシュに分割
            //      aiProcess_PreTransformVertices    : ノードグラフから変形前のローカル頂点座標と変形行列を除去（スケルタルアニメーション用データが消えます）
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
            
            auto flags = default_importer_readfile_flags;
            auto transpose_node = false;
            
            {
              auto ext = file_path.substr( file_path.rfind('.') + 1 );
              if ( ext == "x" )
                flags |= aiProcess_FlipUVs;
              else if ( ext == "cob" || ext == "fbx")
                transpose_node = true;
            }
            
            auto scene = i.ReadFile( file_path, flags );
            
            if ( not scene )
              throw std::runtime_error( i.GetErrorString() );
            
            model_t r( scene, file_path.substr(0, file_path.find_last_of('/')), transpose_node );
            
            i.FreeScene();
            
            return r;
          }
          
        };
      }
    }
  }
}