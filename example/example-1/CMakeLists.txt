cmake_minimum_required(VERSION 2.8.10)

project(example-1)
set(TARGET example-1)
set(SOURCE main.cxx)

find_file(WONDERLAND_SUBSYSTEM_HXX
  wonder_rabbit_project/wonderland.subsystem.hxx
  PATHS /usr/include /usr/local/include ~/opt/include
)

if(NOT WONDERLAND_SUBSYSTEM_HXX)
  message(FATAL_ERROR
    "wonderland.subsystem.hxx is not found.
    this example required wonderland.subsystem
    for easy to use window and input management."
  )
else()
  message(" * found wonderland.subsystem.hxx: ${WONDERLAND_SUBSYSTEM_HXX}")
endif()

if(NOT SUBSYSTEM)
  set(SUBSYSTEM GLFW3)
endif()

message(" * SUBSYSTEM: ${SUBSYSTEM}")

if(SUBSYSTEM MATCHES GLFW3)
  set(CMAKE_CXX_FLAGS "-DWRP_WONDERLAND_SUBSYSTEM_GLFW3 ${CMAKE_CXX_FLAGS}")
elseif(SUBSYSTEM MATCHES GLFW2)
  set(CMAKE_CXX_FLAGS "-DWRP_WONDERLAND_SUBSYSTEM_GLFW2 ${CMAKE_CXX_FLAGS}")
elseif(SUBSYSTEM MATCHES SDL2)
  set(CMAKE_CXX_FLAGS "-DWRP_WONDERLAND_SUBSYSTEM_SDL2 ${CMAKE_CXX_FLAGS}")
elseif(SUBSYSTEM MATCHES SDL1)
  set(CMAKE_CXX_FLAGS "-DWRP_WONDERLAND_SUBSYSTEM_SDL1 ${CMAKE_CXX_FLAGS}")
endif()

if(CMAKE_CXX_COMPILER MATCHES "/em\\+\\+(-[a-zA-Z0-9.])?$")

  message(" * C++ compiler: Emscripten")
  
  set(CMAKE_CXX_COMPILER_ID "Emscripten")
  
  #set(CMAKE_CXX_FLAGS "-s ALLOW_MEMORY_GROWTH=1 ${CMAKE_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS "-s DISABLE_EXCEPTION_CATCHING=0 ${CMAKE_CXX_FLAGS}")
  
  set(CMAKE_CXX_FLAGS       "-std=c++11 ${CMAKE_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS            "-Wall ${CMAKE_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS "-pedantic-errors ${CMAKE_CXX_FLAGS}")
  set(CMAKE_CXX_FLAGS_RELEASE        "-O2 -DNDEBUG")
  set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -g")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
  
  set(TARGET "${TARGET}.html")

else()

  if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    
    message(" * C++ compiler: Clang")
    
    set(CMAKE_CXX_FLAGS       "-std=c++11 ${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS            "-Wall ${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS "-pedantic-errors ${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS_RELEASE        "-O3 -march=native -DNDEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -march=native -g")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -march=native -g")
  
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  
    message(" * C++ compiler: GCC")
    
    set(CMAKE_CXX_FLAGS       "-std=c++11 ${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS            "-Wall ${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS "-pedantic-errors ${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS_RELEASE        "-O3 -march=native -DNDEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -march=native -g -pg")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -march=native -g -pg")
    
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    message(" * C++ compiler: ICC")
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    message(" * C++ compiler: MSVC++")
  else()
    message(" * C++ compiler: unknown")
  endif()

endif()

if(CMAKE_BUILD_TYPE MATCHES release)
  message(" * build type: release")
  set(CXX_FLAGS "${CMAKE_CXX_FLAGS}${CMAKE_CXX_FLAGS_RELEASE}")
  message(" * CXX_FLAGS: ${CXX_FLAGS}")
elseif(CMAKE_BUILD_TYPE MATCHES debug)
  message(" * build type: debug")
  set(CXX_FLAGS "${CMAKE_CXX_FLAGS}${CMAKE_CXX_FLAGS_DEBUG}")
  message(" * CXX_FLAGS: ${CXX_FLAGS}")
elseif(CMAKE_BUILD_TYPE MATCHES relwithdebinfo)
  message(" * build type: relwithdebinfo")
  set(CXX_FLAGS "${CMAKE_CXX_FLAGS}${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
  message(" * CXX_FLAGS: ${CXX_FLAGS}")
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../include)

message(" * target: ${TARGET}")
message(" * source: ${SOURCE}")

add_executable(${TARGET} ${SOURCE})
add_dependencies(${TARGET}
  ${CMAKE_CURRENT_SOURCE_DIR}/../../include/wonder_rabbit_project/wonderland.subsystem.hxx
)

if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "Emscripten")
  find_package(Threads)
  target_link_libraries(${TARGET} ${CMAKE_THREAD_LIBS_INIT})
  
  if(SUBSYSTEM MATCHES GLFW3)
    find_package(PkgConfig REQUIRED)
    pkg_search_module(GLFW REQUIRED glfw3)
    include_directories(${GLFW_INCLUDE_DIRS})
    target_link_libraries(${TARGET} ${GLFW_LIBRARIES})
    target_link_libraries(${TARGET} GL)
    target_link_libraries(${TARGET} X11 Xrandr Xcursor Xi Xxf86vm)
  elseif(SUBSYSTEM MATCHES GLFW2)
    find_package(PkgConfig REQUIRED)
    pkg_search_module(GLFW REQUIRED libglfw)
    include_directories(${GLFW_INCLUDE_DIRS})
    target_link_libraries(${TARGET} ${GLFW_LIBRARIES})
    target_link_libraries(${TARGET} GL)
    target_link_libraries(${TARGET} X11 Xrandr Xcursor Xi Xxf86vm)
  elseif(SUBSYSTEM MATCHES SDL2)
    find_library(SDL2_LIBRARY SDL2 REQUIRED)
    target_link_libraries(${TARGET} ${SDL2_LIBRARY})
  elseif(SUBSYSTEM MATCHES SDL1)
    find_library(SDL1_LIBRARY SDL REQUIRED)
    target_link_libraries(${TARGET} ${SDL1_LIBRARY})
    target_link_libraries(${TARGET} GL GLEW)
  endif()
endif()
