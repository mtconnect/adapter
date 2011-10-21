#
# Find the native ZLIB includes and library
#
# CURL_INCLUDE_DIR - where to find curl.h, etc.
# CURL_LIBRARY      - Localtion of curl livrary.
# CURL_FOUND       - Do not attempt to use zlib if "no" or undefined.

FIND_PATH(CURL_INCLUDE_DIR curl/curl.h)
FIND_LIBRARY(CURL_LIBRARY NAMES curl)

IF (CURL_INCLUDE_DIR AND CURL_LIBRARY)
   SET(CURL_FOUND TRUE)
ENDIF (CURL_INCLUDE_DIR AND CURL_LIBRARY)

IF (CURL_FOUND)

   # show which CppUnit was found only if not quiet
   IF (NOT Curl_FIND_QUIETLY)
      MESSAGE(STATUS "Found Curl: ${CURL_LIBRARY}")
   ENDIF (NOT Curl_FIND_QUIETLY)

ELSE (CURL_FOUND)

   # fatal error if CppUnit is required but not found
   IF (Curl_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find Curl")
   ENDIF (Curl_FIND_REQUIRED)

ENDIF (CURL_FOUND)