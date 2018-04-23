cmake_minimum_required(VERSION 3.7 FATAL_ERROR) #This minimum version is to support Visual Studio 2017 and C++ feature checking

project(adapters)

# All of the C++ code and CppUnit require C++ 11 features to compile.
# We will define these properties by default for each CMake target to be created.
set_property(GLOBAL PROPERTY CXX_STANDARD 11)
set_property(GLOBAL PROPERTY CXX_STANDARD_REQUIRED ON)

# By default only generate 2 configurations (Debug and Release) for simplicity.
# The user can change this option if required for the additional ones such as 'RelWithDebInfo'.
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)
if(DEFINED CMAKE_BUILD_TYPE)
  set_property( CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${CMAKE_CONFIGURATION_TYPES} )
endif()

# Add our './cmake' sub-folder to the lists searched when calling functions
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

# Setup compiler options for Windows, OSX, Linux (each file guards against usage on an inappropriate platform)
if(MSVC)
	# Default winver to Vista and Later
	set(WINVER "0x0600" CACHE STRING "Windows Target Version: 0x0400 - 95 & NT 4.0, 0x0500 - 2000, 0x0501 - XP, 0x0600 - Vista, 0x0601 - 7, 0x0602 - 8")
endif()
include(cmake/osx_no_app_or_frameworks.cmake)
include(cmake/msvc_xp_support.cmake)
include(cmake/vs_set_working_directory.cmake)
include(cmake/adapter_fanuc.cmake)

# Add our dependency projects
set(MSVC_SHARED_RT ON) # Use shared runtime libraries for Microsoft compiles
include(cmake/CppUnit.cmake)

# Add our projects
if(${BUILD_FANUC}) # This is conditional as it requires the externally specified FOCAS-2 libraries to be purchased
	add_subdirectory(fanuc)
endif()

add_subdirectory(fake)
add_subdirectory(test)