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
            "attribute vec3 normal;\n"
            "attribute vec2 texcoord;\n"
            "attribute vec3 tangent;\n"
            //"attribute ivec4 bone_ids;\n"
            "attribute vec4 bone_ids;\n"
            "attribute vec4 bone_weights;\n"
            "varying vec4 var_color;\n"
            "varying vec2 var_texcoord;"
            "uniform mat4 world_view_projection_transformation;\n"
            "uniform mat4 world_transformation;\n"
            "uniform vec3 diffuse;\n"
            "uniform vec3 ambient;\n"
            "uniform vec3 specular;\n"
            "uniform vec3 emissive;\n"
            "uniform float transparent;\n"
            "uniform float reflective;\n"
            "uniform mat4 bones[128];\n"
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
            "  var_color = vec4( diffuse, transparent );\n"
            "  var_texcoord = texcoord;\n"
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
            "varying vec2 var_texcoord;\n"
            "uniform sampler2D sampler;\n"
            
            "bool is_nan(float);\n"
            
            "void main(void)\n"
            "{\n"
            "  if ( is_nan( var_texcoord.x ) )\n"
            "    gl_FragColor = var_color;\n"
            "  else\n"
            "    gl_FragColor = texture2D( sampler, var_texcoord ).xyzw;\n"
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