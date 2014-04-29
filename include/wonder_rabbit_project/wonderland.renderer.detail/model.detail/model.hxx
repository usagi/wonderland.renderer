#pragma once

#include <boost/gil/gil_all.hpp>

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
          // モデルデータに含まれるマテリアル群を格納
          std::vector<material_t> _materials;
          // モデルデータに含まれるメッシュ群を格納
          std::vector<mesh_t> _meshes;
          // ノード
          node_t _node;
          
        public:
          
          // const aiScene* からモデルデータ（メッシュ群、ノードグラフ）を生成
          explicit model_t( const aiScene* scene, const std::string& path_prefix = "" )
            : _node( scene -> mRootNode )
          {
            
            // シーンからマテリアル群を _materials に生成
            _materials.reserve( scene -> mNumMaterials );
            for( auto n = 0; n < scene -> mNumMaterials; ++n )
              _materials.emplace_back( scene -> mMaterials[ n ], path_prefix );

            // シーンからメッシュ群を _meshes に生成
            _meshes.reserve( scene -> mNumMeshes );
            for( auto n = 0; n < scene -> mNumMeshes; ++n )
              _meshes.emplace_back( scene -> mMeshes[ n ], _materials );
            
          }
          
          // 描画
          auto draw() const
          -> void
          { _node.draw( _meshes ); }
          
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
          //| aiProcess_SortByPType
          | aiProcess_FindInvalidData
          | aiProcess_GenUVCoords
          | aiProcess_FindInstances
          | aiProcess_OptimizeMeshes
          | aiProcess_OptimizeGraph
          | aiProcess_FlipUVs
          | aiProcess_Triangulate
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
            
            return model_t( scene, file_path.substr(0, file_path.find_last_of('/')) );
            
          }
          
        };
      }
    }
  }
}