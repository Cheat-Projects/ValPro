# src/memory/CMakeLists.txt

add_library(Memory STATIC
    memory.cpp
)

target_include_directories(Memory
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/memory>
)


if(MSVC)
    target_link_libraries(Memory PRIVATE kernel32.lib)
endif()

target_link_libraries(Memory PRIVATE ProjectOptions)

# Include module for GNU standard installation directories
include(GNUInstallDirs)

# Install headers in include/memory
install(FILES memory.hpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/memory)

# Install library and executable and export as a set
install(TARGETS Memory EXPORT MemoryExportSet
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/memory
)


# Install the export set
install(EXPORT MemoryExportSet
    FILE MemoryTargets.cmake
    NAMESPACE Memory::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Memory
)

