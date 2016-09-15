add_library (adapter_generic service.cpp adapter.cpp client.cpp condition.cpp cutting_tool.cpp device_datum.cpp logger.cpp server.cpp string_buffer.cpp)
#find_package(Threads REQUIRED)
#target_compile_options(adapter_generic PUBLIC "-pthread")

set_target_properties(adapter_generic PROPERTIES COMPILE_FLAGS "-m32 -pthread" LINK_FLAGS "-m32")


