set(FANUC_HEADERS
	fanuc_adapter.hpp
	fanuc_axis.hpp
	fanuc_path.hpp
)
	
set(FANUC_SOURCES
	fanuc_adapter.cpp
	fanuc_axis.cpp
	fanuc_path.cpp
	FanucAdapter.cpp
)

set(MIN_INI_HEADERS
	${CMAKE_SOURCE_DIR}/minIni_07/minIni.h
)

set(MIN_INI_SOURCES
	${CMAKE_SOURCE_DIR}/minIni_07/minIni.c
)

set(ADDITIONAL_HEADERS
	${CMAKE_SOURCE_DIR}/src/adapter.cpp
	${CMAKE_SOURCE_DIR}/src/client.cpp
	${CMAKE_SOURCE_DIR}/src/condition.cpp
	${CMAKE_SOURCE_DIR}/src/device_datum.cpp
	${CMAKE_SOURCE_DIR}/src/logger.cpp
	${CMAKE_SOURCE_DIR}/src/server.cpp
	${CMAKE_SOURCE_DIR}/src/service.cpp
	${CMAKE_SOURCE_DIR}/src/string_array.cpp
	${CMAKE_SOURCE_DIR}/src/string_buffer.cpp
)

set(ADDITIONAL_SOURCES
	${CMAKE_SOURCE_DIR}/src/adapter.hpp
	${CMAKE_SOURCE_DIR}/src/client.hpp
	${CMAKE_SOURCE_DIR}/src/condition.hpp
	${CMAKE_SOURCE_DIR}/src/device_datum.hpp
	${CMAKE_SOURCE_DIR}/src/internal.hpp
	${CMAKE_SOURCE_DIR}/src/logger.hpp
	${CMAKE_SOURCE_DIR}/src/server.hpp
	${CMAKE_SOURCE_DIR}/src/service.hpp
	${CMAKE_SOURCE_DIR}/src/string_array.hpp
	${CMAKE_SOURCE_DIR}/src/string_buffer.hpp
)

set(CONFIGURATION_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/adapter.ini
	${CMAKE_CURRENT_SOURCE_DIR}/fanuc.xml
)

# Allow better viewing and grouping of files in Visual Studio by defining source groups
source_group("Headers Files" FILES ${FANUC_HEADERS})
source_group("Headers Files\\adapter" FILES ${ADDITIONAL_HEADERS})
source_group("Headers Files\\Min Ini" FILES ${MIN_INI_HEADERS})
source_group("Source Files" FILES ${FANUC_SOURCES})
source_group("Source Files\\adapter" FILES ${ADDITIONAL_SOURCES})
source_group("Source Files\\Min Ini" FILES ${MIN_INI_SOURCES})
source_group("Configuration" FILES ${CONFIGURATION_FILES})


add_executable(fanuc
	${FANUC_HEADERS}
	${FANUC_SOURCES}
	${MIN_INI_HEADERS}
	${MIN_INI_SOURCES}
	${ADDITIONAL_HEADERS}
	${ADDITIONAL_SOURCES}
)

target_include_directories(fanuc
	PRIVATE ${CMAKE_SOURCE_DIR}/minIni_07
	PRIVATE ${CMAKE_SOURCE_DIR}/src
)

target_compile_definitions(fanuc
	PRIVATE INI_ANSIONLY
)

if(WIN32)
	target_link_libraries(fanuc
		PRIVATE wsock32.lib
	)
endif()

AddFocas2Support(fanuc)
AddMsvcXPSupport(fanuc) #This will only apply if an XP compatible toolset has been selected in CMake

# Copy the ini file and devices XML to our output directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/adapter.ini DESTINATION ./Debug/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/adapter.ini DESTINATION ./Release/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/fanuc.xml DESTINATION ./Debug/)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/fanuc.xml DESTINATION ./Release/)

SetVisualStudioWorkingDirectory(fanuc "\$\(OutDir\)") # Set the working directory for debugging to the MSBuild property $(OutDir)
