cmake_minimum_required(VERSION 2.8.10)

project(wonderland.renderer)

install( DIRECTORY include/ DESTINATION include )

if(WONDERLAND_RENDERER_EXAMPLES)
  subdirs(example)
endif()