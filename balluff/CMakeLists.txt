cmake_minimum_required (VERSION 2.6) 

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake;${CMAKE_MODULE_PATH}")

set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_FIND_FRAMEWORK NEVER FORCE)
set(CMAKE_FIND_APPBUNDLE NEVER)

project(adapter)

set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_FIND_FRAMEWORK NEVER FORCE)
set(CMAKE_FIND_APPBUNDLE NEVER)

if(WIN32)
  add_library(libxml2 STATIC IMPORTED)
  set(LibXML2_INCLUDE_DIRS win32/libxml2-2.7.7/include )
  file(GLOB LibXML2_LIBRARIES win32/libxml2-2.7.7/lib/libxml2.lib)
  file(GLOB LibXML2_DEBUG_LIBRARIES win32/libxml2-2.7.7/lib/libxml2_a_dll.lib)
  set_property(TARGET libxml2 PROPERTY IMPORTED_LOCATION_RELEASE ${LibXML2_LIBRARIES})
  set_property(TARGET libxml2 PROPERTY IMPORTED_LOCATION_DEBUG ${LibXML2_DEBUG_LIBRARIES})
  
  add_library(curl STATIC IMPORTED)
  set(CURL_INCLUDE_DIR win32/curl-7.22.0/include )
  file(GLOB CURL_LIBRARY win32/curl-7.22.0/lib/libcurl_a.lib)
  file(GLOB CURL_DEBUG_LIBRARY win32/curl-7.22.0/lib/libcurl_a_debug.lib)
  set_property(TARGET curl PROPERTY IMPORTED_LOCATION_RELEAES ${CURL_LIBRARY})
  set_property(TARGET curl PROPERTY IMPORTED_LOCATION_DEBUG ${CURL_DEBUG_LIBRARY})
endif(WIN32)

if(UNIX)
  execute_process(COMMAND uname OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE CMAKE_SYSTEM_NAME)
  if(CMAKE_SYSTEM_NAME MATCHES Linux)
    set(LINUX_LIBRARIES pthread)
  endif(CMAKE_SYSTEM_NAME MATCHES Linux)
endif(UNIX)
project (balluff)

find_package(LibCurl REQUIRED)
find_package(LibXML2 REQUIRED)

if(UNIX)
  find_package(ZLib REQUIRED)
  add_library(libxml2 UNKNOWN IMPORTED)
  set_property(TARGET libxml2 PROPERTY IMPORTED_LOCATION ${LibXML2_LIBRARIES})
endif(UNIX)

set(YAML_CPP_SOURCE_DIR ../yaml/src)
set(YAML_CPP_INCLUDE_DIR ../yaml/include)
set(ADAPTER_CPP_SOURCE_DIR ../src)
set(ADAPTER_CPP_INCLUDE_DIR ../src)

set(B64_INCLUDE_DIR ../b64/include)
set(B64_SOURCE_DIR ../b64/src)

file(GLOB yaml_sources ${YAML_CPP_SOURCE_DIR}/[a-z]*.cpp)
file(GLOB adapter_sources ${ADAPTER_CPP_SOURCE_DIR}/[a-z_]*.cpp)
file(GLOB b64_sources ${B64_SOURCE_DIR}/[a-zA-Z_]*.c)
file(GLOB stream_sources [a-zA-Z_]*.cpp [a-z]*.c)
file(GLOB headers [a-z_]*.hpp [a-z_]*.h ../src/*[a-z_].h ../src/[a-z_]*.hpp ../src/[a-z_]*.inc)

add_definitions(-DDLIB_NO_GUI_SUPPORT ${LibXML2_DEFINITIONS} -DTHREADED)
include_directories(${LibXML2_INCLUDE_DIRS} ${CURL_INCLUDE_DIR} ${B64_INCLUDE_DIR} 
                    ${YAML_CPP_SOURCE_DIR} ${YAML_CPP_INCLUDE_DIR} ${ADAPTER_CPP_INCLUDE_DIR})

add_executable(balluff ${stream_sources}  ${yaml_sources} ${adapter_sources} ${b64_sources} ${headers})
target_link_libraries(balluff ${LINUX_LIBRARIES} z curl libxml2)

if(WIN32)
  target_link_libraries(balluff wsock32.lib)
endif(WIN32)




