#pragma once

#include <boost/optional.hpp>

#include <glm/vec3.hpp>
#include <glm/gtx/vec1.hpp>
#include <glm/gtc/constants.hpp>

#include "../glew.detail/c.hxx"
#include "../glew.detail/gl_type.hxx"
#include "../glew.detail/error.hxx"
#include "../glew.detail/wrapper.hxx"

#include "../stblib.hxx"
#include "../shader.detail/misc.hxx"
#include "../texture.hxx"
#include "../sampler.hxx"

// assimp::Importer
#include <assimp/Importer.hpp>
// aiPostProcessSteps for assimp::Importer::ReadFile
#include <assimp/postprocess.h>
// aiScene, aiNode
#include <assimp/scene.h>

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      namespace model
      {
        class material_t
        {
          glm::vec3 _diffuse;
          glm::vec3 _ambient;
          glm::vec3 _specular;
          glm::vec3 _emissive;
          glm::vec1 _transparent;
          glm::vec1 _reflective;
          
          std::vector< std::shared_ptr< renderer::texture_base_t > > _textures;
          std::vector< std::shared_ptr< renderer::sampler_t > > _samplers;
          
          std::vector< float > _texblends;
          
        public:
          
          material_t( aiMaterial* material, const std::string& path_prefix = "" )
          {
            // マテリアルカラー
            {
              // Note: Assimpの aiMaterial::Get のパラメーターは
              // カンマを含む黒魔法 AI_MATKEY_COLOR_DIFFUSE などで
              // ( const char* pkey, unsigned type, unsigned idx) を渡そうとする。
              const auto get_material_color_to_glm_vec3 =
              [ material ]
              ( const char* pkey, unsigned type, unsigned idx, float default_value )
                -> glm::vec3
              {
                aiColor3D color;
                
                const auto succeeded = material -> Get( pkey, type, idx, color );
                
                return ( succeeded == aiReturn_SUCCESS )
                  ? glm::vec3( color.r, color.g, color.b )
                  : glm::vec3( default_value )
                  ;
              };
              
              _diffuse  = get_material_color_to_glm_vec3( AI_MATKEY_COLOR_DIFFUSE , 1.0f );
              _ambient  = get_material_color_to_glm_vec3( AI_MATKEY_COLOR_AMBIENT , 0.0f );
              _specular = get_material_color_to_glm_vec3( AI_MATKEY_COLOR_SPECULAR, 0.0f );
              _emissive = get_material_color_to_glm_vec3( AI_MATKEY_COLOR_EMISSIVE, 0.0f );
              _transparent = get_material_color_to_glm_vec3( AI_MATKEY_COLOR_TRANSPARENT, 1.0f )[0];
              _reflective  = get_material_color_to_glm_vec3( AI_MATKEY_COLOR_REFLECTIVE , 0.0f )[0];
            }
            
            // テクスチャー
            const auto count_of_textures = material -> GetTextureCount( aiTextureType_DIFFUSE );
            
            _texblends.resize( count_of_textures, 0.0f );
            _textures.reserve( count_of_textures );
            _samplers.reserve( count_of_textures );
            
            for ( auto number_of_texture = 0u; number_of_texture < count_of_textures; ++number_of_texture )
            {
              aiString path;
              material -> GetTexture( aiTextureType_DIFFUSE, number_of_texture, &path, nullptr, nullptr, nullptr, nullptr, nullptr );
              
              stblib::image_loader_t loader;
              
              try
              {
                stblib::image_loader_t loader_( path_prefix.size() ? path_prefix + "/" + path.C_Str() : path.C_Str() );
                loader = std::move( loader_ );
              }
              catch( const std::runtime_error& e )
              {
                std::cerr << "warn: " << e.what() << " " << ( path_prefix.size() ? path_prefix + "/" + path.C_Str() : path.C_Str() ) << "; skip the texture.\n";
                continue;
              }
              
              const glew::gl_type::GLsizei width  = loader.width();
              const glew::gl_type::GLsizei height = loader.height();
              const void* data = loader.data();
              
              glew::c::glPixelStorei( GL_UNPACK_ALIGNMENT, loader.count_of_pixel_elements() == 4 ? 4 : 1 );
              WRP_GLEW_TEST_ERROR
              
              switch( loader.count_of_pixel_elements() )
              {
                case 4:
                {
                  auto t = std::make_shared< renderer::texture_2d_t< GL_RGBA8 > >();
                  t -> create( width, height, data );
                  _textures.emplace_back( std::move( t ) );
                  break;
                }
                case 3:
                {
                  auto t = std::make_shared< renderer::texture_2d_t< GL_RGB8 > >();
                  t -> create( width, height, data );
                  _textures.emplace_back( std::move( t ) );
                  break;
                }
                case 2:
                {
                  auto t = std::make_shared< renderer::texture_2d_t< GL_RG8 > >();
                  t -> create( width, height, data );
                  _textures.emplace_back( std::move( t ) );
                  break;
                }
                case 1:
                {
                  auto t = std::make_shared< renderer::texture_2d_t< GL_R8 > >();
                  t -> create( width, height, data );
                  _textures.emplace_back( std::move( t ) );
                  break;
                }
                default:
                  throw std::runtime_error( "unsupported count_of_pixel_elements" );
              }
              
              auto s = std::make_shared< renderer::sampler_t >();
              s -> parameter_wrap_st( GL_REPEAT )
                -> parameter_mag_filter( GL_LINEAR )
                -> parameter_min_filter( GL_LINEAR_MIPMAP_LINEAR )
                ;
              _samplers.emplace_back( std::move( s ) );
              
              _texblends [ number_of_texture ] = 1.0f;
            }
          }
          
          auto draw( glew::gl_type::GLint program_id ) const
            -> std::vector< destruct_invoker_t >
          {
            std::vector< destruct_invoker_t > ds;
            ds.reserve( _textures.size() );
            
            if ( program_id )
            {
              glew::uniform_t::uniform( program_id, "diffuse"    , _diffuse     );
              glew::uniform_t::uniform( program_id, "ambient"    , _ambient     );
              glew::uniform_t::uniform( program_id, "specular"   , _specular    );
              glew::uniform_t::uniform( program_id, "emissive"   , _emissive    );
              glew::uniform_t::uniform( program_id, "transparent", _transparent );
              glew::uniform_t::uniform( program_id, "reflective" , _reflective  );
              WRP_GLEW_TEST_ERROR
              
              {
                std::array< float, shader::count_of_textures > texblends;
                std::copy( std::begin( _texblends ), std::end( _texblends), texblends.begin() );
                std::fill( std::begin( texblends ) + _texblends.size(), std::end( texblends ), 0.0f );
                
                glew::wrapper_t::uniform( program_id, "texblends", texblends );
                WRP_GLEW_TEST_ERROR
              }
            }
            
            for ( glew::gl_type::GLuint n = 0u; n < _textures.size(); ++n )
            {
              glew::texture_t::active_texture<>( n );
              WRP_GLEW_TEST_ERROR
              
              ds.emplace_back( _textures[ n ] -> scoped_bind() );
              WRP_GLEW_TEST_ERROR
              glew::uniform_t::uniform( program_id, "diffuse_sampler" + std::to_string( n ), signed( n ) );
              WRP_GLEW_TEST_ERROR
              _samplers[ n ] -> bind( n );
              WRP_GLEW_TEST_ERROR
            }
            
            return ds;
          }
        };
      }
    }
  }
}