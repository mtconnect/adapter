# The name of our project is "HELLO". CMakeLists files in this project can 
# refer to the root source directory of the project as ${HELLO_SOURCE_DIR} and 
# to the root binary directory of the project as ${HELLO_BINARY_DIR}. 
cmake_minimum_required (VERSION 2.6) 

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake;${CMAKE_MODULE_PATH}")

set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_FIND_FRAMEWORK NEVER FORCE)
set(CMAKE_FIND_APPBUNDLE NEVER)

if(WIN32)
  set(CPPUNIT_INCLUDE_DIR ../win32/cppunit-1.12.1/include)
  file(GLOB CPPUNIT_LIBRARY ../win32/cppunit-1.12.1/lib/cppunitd.lib)
endif(WIN32)

if(UNIX)
  execute_process(COMMAND uname OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE CMAKE_SYSTEM_NAME)
  if(CMAKE_SYSTEM_NAME MATCHES Linux)
    set(LINUX_LIBRARIES pthread)
  endif(CMAKE_SYSTEM_NAME MATCHES Linux)
endif(UNIX)

project (test)

set(test_srcs test.cpp
	      ../src/adapter.cpp
	      ../src/client.cpp
	      ../src/condition.cpp
	      ../src/device_datum.cpp
	      ../src/logger.cpp
	      ../src/serial.cpp
	      ../src/server.cpp
	      ../src/service.cpp
	      ../src/string_array.cpp
	      ../src/string_buffer.cpp
	      
)


include_directories(../src)

find_package(CppUnit REQUIRED)

add_definitions()
include_directories(${CPPUNIT_INCLUDE_DIR})

add_executable(test ${test_srcs})
target_link_libraries(test ${CPPUNIT_LIBRARY} ${LINUX_LIBRARIES})
