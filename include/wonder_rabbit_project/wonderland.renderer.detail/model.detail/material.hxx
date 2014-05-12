#pragma once

#include <boost/optional.hpp>

#include <glm/vec3.hpp>
#include <glm/gtx/vec1.hpp>

#include "../glew.detail/c.hxx"
#include "../glew.detail/gl_type.hxx"
#include "../glew.detail/error.hxx"

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
#ifdef GL_VERSION_3_3
          // need  for modern GL3
          glew::gl_type::GLuint _sampler_id;
#endif
          
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
              
              glew::gl_type::GLenum internal_format;
              glew::gl_type::GLenum format;
              switch( loader.count_of_pixel_elements() )
              {
                case 4: internal_format = GL_RGBA8; format = GL_RGBA; break;
                case 3: internal_format = GL_RGB8;  format = GL_RGB;  break;
                case 2: internal_format = GL_RG8;   format = GL_RG;   break;
                case 1: internal_format = GL_R8;    format = GL_R;    break;
                default: throw std::runtime_error( "unsupported count_of_pixel_elements" );
              }
              const glew::gl_type::GLsizei width = loader.width();
              const glew::gl_type::GLsizei height = loader.height();
              constexpr glew::gl_type::GLenum type = GL_UNSIGNED_BYTE;
              const void* data = loader.data();
              
              glew::c::glPixelStorei( GL_UNPACK_ALIGNMENT, loader.count_of_pixel_elements() == 4 ? 4 : 1 );
              
              glew::test_error( __FILE__, __LINE__ );
              
              // step.1: generate texture buffer
              // glGenTextures / GL_1_1
              //  http://www.opengl.org/wiki/GLAPI/glGenTextures
              glew::c::glGenTextures( 1, &_texture_id );

              glew::test_error( __FILE__, __LINE__ );
              
              // glBindTexture / GL_1_1
              //  http://www.opengl.org/wiki/GLAPI/glBindTexture
              glew::c::glBindTexture( GL_TEXTURE_2D, _texture_id );

              glew::test_error( __FILE__, __LINE__ );
              
              // step.2: generate storage and load image
#if defined( GL_VERSION_4_2 )
              
              const glew::gl_type::GLsizei level = glm::log2<float>( std::min( width, height ) );
              // glTexStorage2D / GL_4_2
              //  http://www.opengl.org/wiki/GLAPI/glTexStorage2D
              //static auto counter = 0;
              //if ( counter++ == 0 ) internal_format = GL_RGB8; else internal_format = GL_RGB8;
              glew::c::glTexStorage2D( GL_TEXTURE_2D, level, internal_format, width, height );
              
              glew::test_error( __FILE__, __LINE__ );
              
              // glTexSubImage2D / GL_1_0
              //  http://www.opengl.org/wiki/GLAPI/glTexSubImage2D
              glew::c::glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, width, height, format, type, data );
              
              glew::test_error( __FILE__, __LINE__ );
              
#elif defined( GL_VERSION_3_0 )
              
              constexpr glew::gl_type::GLint border = 0; // this value must be 0: http://www.opengl.org/sdk/docs/man/html/glTexImage2D.xhtml
              constexpr glew::gl_type::GLint level  = 0;
  #if EMSCRIPTEN
              internal_format = format;
  #endif
              glew::c::glTexImage2D( GL_TEXTURE_2D, level, internal_format, width, height, border, format, type, data );
              glew::test_error( __FILE__, __LINE__ );
              
#elif defined( GL_VERSION_1_4 )
              
              // warning: these medhod is deprecated in GL_3_0, removed in GL_3_1.
              //  within throught step.3 and step.4.
              glew::c::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
              glew::test_error( __FILE__, __LINE__ );
              glew::c::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
              glew::test_error( __FILE__, __LINE__ );
              glew::c::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
              glew::test_error( __FILE__, __LINE__ );
              glew::c::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
              glew::test_error( __FILE__, __LINE__ );
              glew::c::glTexParameteri( GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE );
              glew::test_error( __FILE__, __LINE__ );
              glew::c::glTexImage2D( GL_TEXTURE_2D, level, internal_format, width, height, border, format, type, data );
              glew::test_error( __FILE__, __LINE__ );
              
#else
              
              // no mipmap
              constexpr glew::gl_type::GLint level = 0;
              glew::c::glTexImage2D( GL_TEXTURE_2D, level, internal_format, width, height, border, format, type, data );
              
              glew::test_error( __FILE__, __LINE__ );
              
#endif
              
              // step.3: generate mipmaps
#if defined( GL_VERSION_3_0 )
              
              // glGenerateMipmap / GL_3_0
              //  http://www.opengl.org/wiki/GLAPI/glGenerateMipmap
              glew::c::glGenerateMipmap( GL_TEXTURE_2D );
              
              glew::test_error( __FILE__, __LINE__ );
              
#endif
              
              // step.4: set sampler params
#if defined( GL_VERSION_3_3 )
              // glGenSamplers / GL_3_3
              //  http://www.opengl.org/wiki/GLAPI/glGenSamplers
              glew::c::glGenSamplers( 1, &_sampler_id );
              
              glew::c::glSamplerParameteri( _sampler_id, GL_TEXTURE_WRAP_S, GL_REPEAT );
              glew::test_error( __FILE__, __LINE__ );
              glew::c::glSamplerParameteri( _sampler_id, GL_TEXTURE_WRAP_T, GL_REPEAT );
              glew::test_error( __FILE__, __LINE__ );
              glew::c::glSamplerParameteri( _sampler_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
              glew::test_error( __FILE__, __LINE__ );
              glew::c::glSamplerParameteri( _sampler_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
              glew::test_error( __FILE__, __LINE__ );
              
#elif defined( GL_VERSION_3_0 )
              glew::c::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
              glew::test_error( __FILE__, __LINE__ );
              glew::c::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
              glew::test_error( __FILE__, __LINE__ );
              glew::c::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
              glew::test_error( __FILE__, __LINE__ );
              glew::c::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
              glew::test_error( __FILE__, __LINE__ );
              #elif defined( GL_VERSION_1_4 )
              // nothing to do. this step done before at this version.
#else
              // no mipmap
              glew::c::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
              glew::test_error( __FILE__, __LINE__ );
              glew::c::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
              glew::test_error( __FILE__, __LINE__ );
              glew::c::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
              glew::test_error( __FILE__, __LINE__ );
              glew::c::glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
              glew::test_error( __FILE__, __LINE__ );
#endif
              
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
              { \
                if ( _ ## WRP_TMP_X ) \
                  glew::c::glUniform ## WRP_TMP_Y ## fv( location_of_ ## WRP_TMP_X , 1, & _ ## WRP_TMP_X .get().x ); \
                else \
                  glew::c::glUniform ## WRP_TMP_Y ## fv( location_of_ ## WRP_TMP_X , 1, & WRP_TMP_Z [0] ); \
              }
              
              WRP_TMP( diffuse    , 3, glm::vec3( 1.0f ) )
              WRP_TMP( ambient    , 3, glm::vec3( 0.0f ) )
              WRP_TMP( specular   , 3, glm::vec3( 0.0f ) )
              WRP_TMP( emissive   , 3, glm::vec3( 0.0f ) )
              WRP_TMP( transparent, 1, glm::vec1( 1.0f ) )
              WRP_TMP( reflective , 1, glm::vec1( 0.0f ) )
              
#undef WRP_TMP
            }
#ifdef GL_VERSION_1_3
            glew::c::glActiveTexture( GL_TEXTURE0 );
#endif
            // GL_1_1
            glew::c::glBindTexture( GL_TEXTURE_2D, _texture_id );
#ifdef GL_VERSION_3_3
            glew::c::glBindSampler( 0, _sampler_id );
#endif
          }
        };
      }
    }
  }
}