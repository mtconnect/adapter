# This module allows the addition of Date to a target

#################################################################################
#																				#
#								Functions										#
#																				#
#################################################################################

function(AddDateSupport projectTarget)
	# Only support the header-only implementation of date, so just add an include path to the target
	target_include_directories(${projectTarget} PRIVATE ${CMAKE_SOURCE_DIR}/date/include)
endfunction()