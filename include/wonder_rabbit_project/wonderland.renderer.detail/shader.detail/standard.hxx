#pragma once

#include <string>

namespace wonder_rabbit_project
{
  namespace wonderland
  {
    namespace renderer
    {
      namespace shader
      {
        namespace standard
        {
          auto vs_source() -> std::string
          { return 
            "#version 100\n"
            //"#extension GL_EXT_gpu_shader4 : enable\n"
            "attribute vec4 position;\n"
            "attribute vec4 color;\n"
            "attribute vec3 normal;\n"
            "attribute vec2 texcoord0;\n"
            "attribute vec2 texcoord1;\n"
            "attribute vec2 texcoord2;\n"
            "attribute vec2 texcoord3;\n"
            "attribute vec2 texcoord4;\n"
            "attribute vec2 texcoord5;\n"
            "attribute vec2 texcoord6;\n"
            "attribute vec2 texcoord7;\n"
            "attribute vec3 tangent;\n"
            "attribute vec3 bitangent;\n"
            //"attribute ivec4 bone_ids;\n"
            "attribute vec4 bone_ids;\n"
            "attribute vec4 bone_weights;\n"
            "varying vec4 var_color;\n"
            "varying vec2 var_texcoords[ 8 ];"
            "varying vec3 var_diffuse;\n"
            "varying vec3 var_ambient;\n"
            "varying vec3 var_specular;\n"
            "varying vec3 var_emissive;\n"
            "varying float var_transparent;\n"
            "varying float var_texblends[ 8 ];\n"
            "uniform mat4 world_view_projection_transformation;\n"
            "uniform mat4 world_transformation;\n"
            "uniform vec3 diffuse;\n"
            "uniform vec3 ambient;\n"
            "uniform vec3 specular;\n"
            "uniform vec3 emissive;\n"
            "uniform float transparent;\n"
            "uniform float reflective;\n"
            "uniform mat4 bones[ 128 ];\n"
            "uniform float texblends[ 8 ];\n"
            
            "bool is_nan(float);\n"
            
            "void main(void)\n"
            "{\n"
            "  mat4 animation_transformation\n"
            "    = bone_weights[ 0 ] + bone_weights[ 1 ] + bone_weights[ 2 ] + bone_weights[ 3 ] == 0.0\n"
            "      ? mat4( 1.0 )\n"
            "      : bones[ int( bone_ids[ 0 ] ) ] * bone_weights[ 0 ]\n"
            "      + bones[ int( bone_ids[ 1 ] ) ] * bone_weights[ 1 ]\n"
            "      + bones[ int( bone_ids[ 2 ] ) ] * bone_weights[ 2 ]\n"
            "      + bones[ int( bone_ids[ 3 ] ) ] * bone_weights[ 3 ]\n"
            "      ;\n"
            "  vec4 local_position = animation_transformation * position;\n"
            "  gl_Position = world_view_projection_transformation * local_position;\n"
            "  var_color       = color;\n"
            "  var_texcoords[ 0 ] = texcoord0;\n"
            "  var_texcoords[ 1 ] = texcoord1;\n"
            "  var_texcoords[ 2 ] = texcoord2;\n"
            "  var_texcoords[ 3 ] = texcoord3;\n"
            "  var_texcoords[ 4 ] = texcoord4;\n"
            "  var_texcoords[ 5 ] = texcoord5;\n"
            "  var_texcoords[ 6 ] = texcoord6;\n"
            "  var_texcoords[ 7 ] = texcoord7;\n"
            "  var_diffuse     = diffuse;\n"
            "  var_ambient     = ambient;\n"
            "  var_specular    = specular;\n"
            "  var_emissive    = emissive;\n"
            "  var_transparent = transparent;\n"
            "  for ( int n = 0; n < 8; ++n )\n"
            "    var_texblends[ n ] = texblends[ n ];\n"
            "}"
            ;
          }
          
          auto fs_source() -> std::string
          { return
            "#version 100\n"
            "#ifdef GL_FRAGMENT_PRECISION_HIGH\n"
            "  precision highp float;\n"
            "#else\n"
            "  precision mediump float;\n"
            "#endif\n"
            "varying vec4 var_color;\n"
            "varying vec2 var_texcoords[ 8 ];\n"
            "varying vec3 var_diffuse;\n"
            "varying vec3 var_ambient;\n"
            "varying vec3 var_specular;\n"
            "varying vec3 var_emissive;\n"
            "varying float var_transparent;\n"
            "varying float var_texblends[ 8 ];\n"
            "uniform sampler2D sampler;\n"
            
            "bool is_nan(float);\n"
            
            "void main(void)\n"
            "{\n"
            "  if ( ! is_nan( var_color.x ) )\n"
            "    gl_FragColor = var_color;\n"
            "  else\n"
            "    gl_FragColor = vec4((var_diffuse + var_ambient), var_transparent );\n"
            "\n"
            "  float texblend = 0.0;\n"
            "  vec4 texture_color = vec4( 0.0 );\n"
            "  for ( int n = 0; n < 8; ++n )\n"
            "    if ( var_texblends[ n ] > 0.0 )\n"
            "    {\n"
            "      texblend += var_texblends[ n ];\n"
            "      texture_color += texture2D( sampler, var_texcoords[ n ] );\n"
            "    }\n"
            "  if ( texblend > 0.0 )\n"
            "  {\n"
            "    gl_FragColor *= 1.0 - texblend;\n"
            "    gl_FragColor += texture_color * texblend;\n"
            "  }\n"
            "}\n"
            
            "bool is_nan(float val)\n"
            "{\n"
            "  return (val <= 0.0 || 0.0 <= val) ? false : true;\n"
            "}\n"
            ;
          }
          
        }
      }
    }
  }
}