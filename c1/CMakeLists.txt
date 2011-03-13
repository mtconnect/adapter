cmake_minimum_required (VERSION 2.6) 

set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_FIND_FRAMEWORK NEVER FORCE)
set(CMAKE_FIND_APPBUNDLE NEVER)

project(adapter)

set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_FIND_FRAMEWORK NEVER FORCE)
set(CMAKE_FIND_APPBUNDLE NEVER)

set(YAML_CPP_SOURCE_DIR ../yaml/src)
set(YAML_CPP_INCLUDE_DIR ../yaml/include)
set(ADAPTER_CPP_SOURCE_DIR ../src)
set(ADAPTER_CPP_INCLUDE_DIR ../src)

add_definitions(-DTHREADED)

#
file(GLOB yaml_sources ${YAML_CPP_SOURCE_DIR}/[a-z]*.cpp)
file(GLOB adapter_sources ${ADAPTER_CPP_SOURCE_DIR}/[a-z_]*.cpp)
file(GLOB zigbee_sources [a-zA-Z_]*.cpp [a-z]*.c)
file(GLOB headers [a-z_]*.hpp)

include_directories(${YAML_CPP_SOURCE_DIR} ${YAML_CPP_INCLUDE_DIR} ${ADAPTER_CPP_INCLUDE_DIR})

add_executable(adapter ${yaml_sources} ${adapter_sources} ${zigbee_sources})

if(WIN32)
  target_link_libraries(adapter ws2_32.lib)
  
  foreach(flag_var
          CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
          CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
     if(${flag_var} MATCHES "/MD")
        string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
     endif(${flag_var} MATCHES "/MD")
  endforeach(flag_var)
endif(WIN32)


