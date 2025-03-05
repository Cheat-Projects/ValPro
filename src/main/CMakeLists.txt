# src/main/CMakeLists.txt

add_library(main main.cpp)

target_include_directories(main PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/memory/include>
    $<INSTALL_INTERFACE:include/memory>
)


target_link_libraries(main PRIVATE
    ProjectOptions 
    ProjectWarnings
    Memory
)

add_executable(test_main main.cpp)
target_link_libraries(test_main PRIVATE main)