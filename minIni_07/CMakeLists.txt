add_library (minIni minIni.c)
find_package(Threads REQUIRED)
  target_compile_options( minIni PUBLIC "-pthread")

set_target_properties(minIni PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")

