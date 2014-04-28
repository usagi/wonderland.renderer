#pragma once

#include <boost/gil/gil_all.hpp>
#include <boost/optional.hpp>

#include <glm/vec3.hpp>
#include <glm/gtx/vec1.hpp>

#include "../glew.detail/c.hxx"
#include "../glew.detail/gl_type.hxx"

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
          boost::optional< glm::vec3 > diffuse;
          boost::optional< glm::vec3 > ambient;
          boost::optional< glm::vec3 > specular;
          boost::optional< glm::vec3 > emissive;
          boost::optional< glm::vec1 > transparent;
          boost::optional< glm::vec1 > reflective;
          
        public:
          
          material_t( aiMaterial* material, const std::string& path_prefix = "" )
          {
            std::cerr << " *** material ***\n";
            
            {
              aiColor3D result;
              if( material -> Get( AI_MATKEY_COLOR_DIFFUSE, result ) == aiReturn_SUCCESS )
                diffuse = { result.r, result.g, result.b };
              if( material -> Get( AI_MATKEY_COLOR_AMBIENT, result ) == aiReturn_SUCCESS )
                ambient = { result.r, result.g, result.b };
              if( material -> Get( AI_MATKEY_COLOR_SPECULAR, result ) == aiReturn_SUCCESS )
                specular = { result.r, result.g, result.b };
              if( material -> Get( AI_MATKEY_COLOR_EMISSIVE, result ) == aiReturn_SUCCESS )
                emissive = { result.r, result.g, result.b };
              if( material -> Get( AI_MATKEY_COLOR_TRANSPARENT, result ) == aiReturn_SUCCESS )
                transparent = result.r;
              if( material -> Get( AI_MATKEY_COLOR_REFLECTIVE, result ) == aiReturn_SUCCESS )
                reflective = result.r;
            }
            
            const auto count_of_textures = material -> GetTextureCount(aiTextureType_DIFFUSE);
            for ( auto number_of_texture = 0u; number_of_texture < count_of_textures; ++number_of_texture )
            {
              
              aiString path;
              material -> GetTexture(aiTextureType_DIFFUSE, number_of_texture, &path, nullptr, nullptr, nullptr, nullptr, nullptr );
              std::cerr << "texture[" << number_of_texture << "]: " << path_prefix << "/" << path.C_Str() << "\n";
            }
          }
          
          auto draw() const -> void
          {
            glew::gl_type::GLint program_id;
            glew::c::glGetIntegerv( GL_CURRENT_PROGRAM, &program_id );
            
            if ( program_id )
            {

#define WRP_TMP( WRP_TMP_X, WRP_TMP_Y, WRP_TMP_Z ) \
              const auto location_of_ ## WRP_TMP_X = glew::c::glGetUniformLocation( program_id, # WRP_TMP_X ); \
              if ( location_of_ ## WRP_TMP_X not_eq -1 ) \
                if ( WRP_TMP_X ) \
                  glew::c::glUniform ## WRP_TMP_Y ## fv( location_of_ ## WRP_TMP_X , 1, & WRP_TMP_X .get().x ); \
                else \
                  glew::c::glUniform ## WRP_TMP_Y ## fv( location_of_ ## WRP_TMP_X , 1, & WRP_TMP_Z .x );
              
              WRP_TMP( diffuse    , 3, glm::vec3( 1.0f ) )
              WRP_TMP( ambient    , 3, glm::vec3( 0.0f ) )
              WRP_TMP( specular   , 3, glm::vec3( 0.0f ) )
              WRP_TMP( emissive   , 3, glm::vec3( 0.0f ) )
              WRP_TMP( transparent, 1, glm::vec1( 1.0f ) )
              WRP_TMP( reflective , 1, glm::vec1( 0.0f ) )
              
#undef WRP_TMP
            }
            
          }
        };
      }
    }
  }
}