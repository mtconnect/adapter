add_executable (adapter_fanuc fanuc_axis.cpp fanuc_path.cpp fanuc_adapter.cpp FanucAdapter.cpp)
target_include_directories (adapter_fanuc PUBLIC ../src ../minIni_07)

find_package(Threads REQUIRED)

set_target_properties(adapter_fanuc PROPERTIES COMPILE_FLAGS "-m32 -lfwlib32 -pthread" LINK_FLAGS "-m32 -lfwlib32")

target_link_libraries(adapter_fanuc fwlib32 adapter_generic minIni pthread)

install (TARGETS adapter_fanuc DESTINATION /usr/local/bin)
