u8R"(#version 100

attribute vec4 position;
attribute vec4 color;
//attribute vec3 normal;
attribute vec2 texcoord0;
//attribute vec2 texcoord1;
//attribute vec2 texcoord2;
//attribute vec2 texcoord3;
//attribute vec2 texcoord4;
//attribute vec2 texcoord5;
//attribute vec2 texcoord6;
//attribute vec2 texcoord7;
attribute vec4 bone_ids;
attribute vec4 bone_weights;

//varying vec3 var_position;
varying vec4 var_color;
//varying vec3 var_normal;
varying vec2 var_texcoords[ )" + std::to_string( count_of_textures ) + u8R"( ];

uniform mat4 world_view_projection_transformation;
//uniform mat4 world_transformation;
uniform mat4 bones[ )" + std::to_string( max_bones ) + u8R"( ];
//uniform float texblends[ )" + std::to_string( count_of_textures ) + u8R"( ];

void main(void)
{
  
}
)"