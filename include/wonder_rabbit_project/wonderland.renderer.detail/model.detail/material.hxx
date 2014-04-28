#pragma once

#include <boost/optional.hpp>

#include <glm/vec3.hpp>
#include <glm/gtx/vec1.hpp>

#include "../glew.detail/c.hxx"
#include "../glew.detail/gl_type.hxx"

#include "../stblib.hxx"

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
          boost::optional< glm::vec3 > _diffuse;
          boost::optional< glm::vec3 > _ambient;
          boost::optional< glm::vec3 > _specular;
          boost::optional< glm::vec3 > _emissive;
          boost::optional< glm::vec1 > _transparent;
          boost::optional< glm::vec1 > _reflective;
          
          glew::gl_type::GLuint _texture_id;
          
        public:
          
          ~material_t()
          {
            glew::c::glDeleteTextures( 1, &_texture_id );
          }
          
          material_t( aiMaterial* material, const std::string& path_prefix = "" )
          {
            // マテリアルカラー
            {
              aiColor3D result;
              if( material -> Get( AI_MATKEY_COLOR_DIFFUSE, result ) == aiReturn_SUCCESS )
                _diffuse = { result.r, result.g, result.b };
              if( material -> Get( AI_MATKEY_COLOR_AMBIENT, result ) == aiReturn_SUCCESS )
                _ambient = { result.r, result.g, result.b };
              if( material -> Get( AI_MATKEY_COLOR_SPECULAR, result ) == aiReturn_SUCCESS )
                _specular = { result.r, result.g, result.b };
              if( material -> Get( AI_MATKEY_COLOR_EMISSIVE, result ) == aiReturn_SUCCESS )
                _emissive = { result.r, result.g, result.b };
              if( material -> Get( AI_MATKEY_COLOR_TRANSPARENT, result ) == aiReturn_SUCCESS )
                _transparent = result.r;
              if( material -> Get( AI_MATKEY_COLOR_REFLECTIVE, result ) == aiReturn_SUCCESS )
                _reflective = result.r;
            }
            
            // テクスチャー
            const auto count_of_textures = material -> GetTextureCount(aiTextureType_DIFFUSE);
            for ( auto number_of_texture = 0u; number_of_texture < count_of_textures; ++number_of_texture )
            {
              aiString path;
              material -> GetTexture( aiTextureType_DIFFUSE, number_of_texture, &path, nullptr, nullptr, nullptr, nullptr, nullptr );
              
              glew::c::glGenTextures( 1, &_texture_id );
              glew::c::glBindTexture( GL_TEXTURE_2D, _texture_id );
              
              stblib::image_loader_t loader( path_prefix.size() ? path_prefix + "/" + path.C_Str() : path.C_Str() );
              
              constexpr glew::gl_type::GLint level = 0;
              glew::gl_type::GLint internal_format;
              switch( loader.count_of_pixel_elements() )
              {
                case 4: internal_format = GL_RGBA; break;
                case 3: internal_format = GL_RGB; break;
                case 2: internal_format = GL_RG; break;
                case 1: internal_format = GL_RED; break;
                default: throw std::runtime_error( "unsupported count_of_pixel_elements" );
              }
              const glew::gl_type::GLsizei width = loader.width();
              const glew::gl_type::GLsizei height = loader.height();
              constexpr glew::gl_type::GLint border = 0; // this value must be 0: http://www.opengl.org/sdk/docs/man/html/glTexImage2D.xhtml
              const glew::gl_type::GLenum format = internal_format;
              constexpr glew::gl_type::GLenum type = GL_UNSIGNED_BYTE;
              const void* data = loader.data();
              glew::c::glPixelStorei( GL_UNPACK_ALIGNMENT, loader.count_of_pixel_elements() == 4 ? 4 : 1 );
              glew::c::glTexImage2D( GL_TEXTURE_2D, level, internal_format, width, height, border, format, type, data );
              glew::c::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
              glew::c::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
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
                if ( _ ## WRP_TMP_X ) \
                  glew::c::glUniform ## WRP_TMP_Y ## fv( location_of_ ## WRP_TMP_X , 1, & _ ## WRP_TMP_X .get().x ); \
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
            
            glew::c::glBindTexture( GL_TEXTURE_2D, _texture_id );
            
          }
        };
      }
    }
  }
}