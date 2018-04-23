set(FAKE_HEADERS
	fake_adapter.hpp
)
	
set(FAKE_SOURCES
	fake.cpp
	fake_adapter.cpp
)

set(ADDITIONAL_HEADERS
	${CMAKE_SOURCE_DIR}/src/adapter.hpp
	${CMAKE_SOURCE_DIR}/src/client.hpp
	${CMAKE_SOURCE_DIR}/src/condition.hpp
	${CMAKE_SOURCE_DIR}/src/device_datum.hpp
	${CMAKE_SOURCE_DIR}/src/server.hpp
	${CMAKE_SOURCE_DIR}/src/service.hpp
	${CMAKE_SOURCE_DIR}/src/string_buffer.hpp
)

set(ADDITIONAL_SOURCES
	${CMAKE_SOURCE_DIR}/src/adapter.cpp
	${CMAKE_SOURCE_DIR}/src/client.cpp
	${CMAKE_SOURCE_DIR}/src/condition.cpp
	${CMAKE_SOURCE_DIR}/src/device_datum.cpp
	${CMAKE_SOURCE_DIR}/src/logger.cpp
	${CMAKE_SOURCE_DIR}/src/server.cpp
	${CMAKE_SOURCE_DIR}/src/service.cpp
	${CMAKE_SOURCE_DIR}/src/string_buffer.cpp
)

add_executable(fake_adapter
	${FAKE_HEADERS}
	${FAKE_SOURCES}
	${ADDITIONAL_HEADERS}
	${ADDITIONAL_SOURCES}
)

target_include_directories(fake_adapter
	PRIVATE ${CMAKE_SOURCE_DIR}/src
)


if(WIN32)
	target_link_libraries(fake_adapter
		PRIVATE wsock32.lib
		PRIVATE Ws2_32.lib
	)
endif()

AddMsvcXPSupport(fake_adapter) #This will only apply if an XP compatible toolset has been selected in CMake