if(UNIX)
	execute_process(COMMAND uname OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE CMAKE_SYSTEM_NAME)
	if(CMAKE_SYSTEM_NAME MATCHES Linux)
		set(LINUX_LIBRARIES pthread)
	endif(CMAKE_SYSTEM_NAME MATCHES Linux)
endif(UNIX)

set(YAML_CPP_SOURCE_DIR ${CMAKE_SOURCE_DIR}/yaml/src)
set(YAML_CPP_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/yaml/include)
set(ADAPTER_CPP_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(ADAPTER_CPP_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/src)

file(GLOB yaml_sources ${YAML_CPP_SOURCE_DIR}/[a-z]*.cpp)
file(GLOB yaml_headers ${YAML_CPP_SOURCE_DIR}/[a-z]*.h)
file(GLOB adapter_sources ${ADAPTER_CPP_SOURCE_DIR}/[a-z_]*.cpp)
file(GLOB adapter_headers ${ADAPTER_CPP_SOURCE_DIR}/[a-z_]*.hpp)
file(GLOB test_sources [a-zA-Z_]*.cpp [a-z]*.c)
file(GLOB headers [a-z_]*.hpp)

# Allow better viewing and grouping of files in Visual Studio by defining source groups
source_group("Headers Files" FILES ${headers})
source_group("Headers Files\\adapter" FILES ${adapter_headers})
source_group("Headers Files\\YAML" FILES ${yaml_headers})
source_group("Source Files" FILES ${test_sources})
source_group("Source Files\\adapter" FILES ${adapter_sources})
source_group("Source Files\\YAML" FILES ${yaml_sources})


add_executable(test
	${yaml_sources}
	${yaml_headers}
	${adapter_sources}
	${adapter_headers}
	${test_sources}
	${headers}
	)

target_include_directories(test
	PRIVATE ${YAML_CPP_SOURCE_DIR}
	PRIVATE ${YAML_CPP_INCLUDE_DIR}
	PRIVATE ${ADAPTER_CPP_INCLUDE_DIR}
	)

target_link_libraries(test
	PRIVATE ${LINUX_LIBRARIES}
	)

if(WIN32)
	target_link_libraries(test
		PRIVATE wsock32.lib
		PRIVATE Ws2_32.lib
	)
endif()

AddMsvcXPSupport(test) #This will only apply if an XP compatible toolset has been selected in CMake
AddCppUnitSupport(test)
AddDateSupport(test)
