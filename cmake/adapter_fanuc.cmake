option(BUILD_FANUC "Build the FOCAS-2 Fanuc Adapter" OFF)

# If we do not have a cache entry for the FOCAS directory, create one now
# and default its value relative to the project root
if(NOT DEFINED FOCAS2_DIR)
	set(FOCAS2_DIR "${CMAKE_SOURCE_DIR}/../FOCAS2" CACHE PATH "Path to the top-level Fanuc FOCAS2 directory")
endif()

# Define a drop-list of FANUC controls that can be targeted
if(NOT DEFINED FANUC_CONTROL)
	set(FANUC_CONTROL "FS16/18/21, 0i-B, 0i-C, 16i/18i-W, PMi" CACHE STRING "Control to target within the FOCAS-2 library, affects maximum number of axis")
endif()

# Constrain this list in the UI
#	|-----------------------------------------------|---------------|-------------------|
#	|	Value										|	MAX_AXIS	|	Compile Symbol	|
#	|-----------------------------------------------|---------------|-------------------|
#	|	FS16/18/21, 0i-B, 0i-C, 16i/18i-W, PMi (8)	|		8		|		-			|
#	|	FS0i-A (4)									|		4		|		FS0ID		|
#	|	FS0i-D (32)									|		32		|		FS0IDD		|
#	|	FS0i-F (32)									|		32		|		FS30D		|
#	|	FS15-B (10)									|		10		|		FS15BD		|
#	|	FS15-B Multi axes (15)						|		15		|		M_AXIS1		|
#	|	FS15-B/15i Multi axes (24)					|		24		|		M_AXIS2		|
#	|	FS30i/31i/32i/35i (32)						|		32		|		FS30D		|
#	|	PMi-A (32)									|		32		|		FS30D		|
#	|	PM (6)										|		32		|		PMD			|
#	|-----------------------------------------------|---------------|-------------------|
#
set_property(CACHE FANUC_CONTROL PROPERTY STRINGS
	"FS16/18/21, 0i-B, 0i-C, 16i/18i-W, PMi (8)"
	"FS0i-A (4)"
	"FS0i-D (32)"
	"FS0i-F (32)"
	"FS15-B (10)"
	"FS15-B Multi axes (15)"
	"FS15-B/15i Multi axes (24)"
	"FS30i/31i/32i/35i (32)"
	"PMi-A (32)"
	"PM (6)"
	)

# Perform global checks if the user has chosen to build the Fanuc adapter
if(${BUILD_FANUC})
	if(NOT EXISTS ${FOCAS2_DIR})
		message(WARNING "The FOCAS2 directory '${FOCAS2_DIR}' does not exists. Skipping Fanuc adapter generation")
		set(BUILD_FANUC OFF)
	endif()
endif()

# FANUC only supports Windows for 64-bit compilations
if( ("${CMAKE_SIZEOF_VOID_P}" EQUAL "8") AND UNIX)
	message(WARNING "FOCAS2 only supports 32-bit compilations when targeting Linux. Skipping Fanuc adapter generation")
	set(BUILD_FANUC OFF)
endif()

# Fanuc include directories are based upon architecture and platform
# Default to 32-bit systems
set(FANUC_INC_BASE_DIR ${FOCAS2_DIR}/Fwlib)
if("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
	# 64-bit system
	set(FANUC_INC_BASE_DIR ${FOCAS2_DIR}/Fwlib64)
endif()
set(FANUC_INC_CTRL_DIR ${FANUC_INC_BASE_DIR}/e1) # This header is suitable for all controls if compilation definitions are set

# These properties are different for Linux
if(UNIX AND (CMAKE_SYSTEM_NAME MATCHES Linux) )
	set(FANUC_INC_BASE_DIR ${FOCAS2_DIR}/Fwlib/Linux)
	set(FANUC_INC_CTRL_DIR ${FANUC_INC_BASE_DIR})
endif()

#################################################################################
#																				#
#								Functions										#
#																				#
#################################################################################
function(AddFocas2Support projectTarget)

	# Add compile definitions based upon the current value of FANUC_CONTROL
	if(${FANUC_CONTROL} STREQUAL "FS0i-A (4)")
		target_compile_definitions(${projectTarget} PRIVATE FS0ID)
	elseif(${FANUC_CONTROL} STREQUAL "FS0i-D (32)")
		target_compile_definitions(${projectTarget} PRIVATE FS0IDD)
	elseif(${FANUC_CONTROL} STREQUAL "FS0i-F (32)")
		target_compile_definitions(${projectTarget} PRIVATE FS30D)
	elseif(${FANUC_CONTROL} STREQUAL "FS15-B (10)")
		target_compile_definitions(${projectTarget} PRIVATE FS15BD)
	elseif(${FANUC_CONTROL} STREQUAL "FS15-B Multi axes (15)")
		target_compile_definitions(${projectTarget} PRIVATE M_AXIS1)
	elseif(${FANUC_CONTROL} STREQUAL "FS15-B/15i Multi axes (24)")
		target_compile_definitions(${projectTarget} PRIVATE M_AXIS2)
	elseif(${FANUC_CONTROL} STREQUAL "FS30i/31i/32i/35i (32)")
		target_compile_definitions(${projectTarget} PRIVATE FS30D)
	elseif(${FANUC_CONTROL} STREQUAL "PMi-A (32)")
		target_compile_definitions(${projectTarget} PRIVATE FS30D)
	elseif(${FANUC_CONTROL} STREQUAL "PM (6)")
		target_compile_definitions(${projectTarget} PRIVATE PMD)
	endif()

	# Target the correct link libraries
	if(WIN32)
		if("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
			target_link_libraries(${projectTarget}
				PRIVATE ${FANUC_INC_BASE_DIR}/Fwlib64.lib
				)
		else()
			target_link_libraries(${projectTarget}
				PRIVATE ${FANUC_INC_BASE_DIR}/Fwlib32.lib
				)
		endif()
	elseif(UNIX)
		if(CMAKE_SYSTEM_PROCESSOR EQUAL "arm")
			target_link_libraries(${projectTarget}
				PRIVATE ${FANUC_INC_BASE_DIR}/arm/libfwlib32.so.1.0.1
				)
		else()
			target_link_libraries(${projectTarget}
				PRIVATE ${FANUC_INC_BASE_DIR}/x86/libfwlib32.so.1.0.2
				)
		endif()
	endif()

	# Add include directories
	target_include_directories(${projectTarget}
		PRIVATE ${FANUC_INC_BASE_DIR}
		PRIVATE ${FANUC_INC_CTRL_DIR}
		)

endfunction()
