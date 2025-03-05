# Create an INTERFACE library for project options
add_library(ProjectOptions INTERFACE)

# Ensure compiler warnings are available BEFORE calling set_project_warnings()
include(cmake/CompilerWarnings.cmake)

# Apply project warnings to ProjectOptions
if(COMMAND set_project_warnings)
    set_project_warnings(ProjectOptions)
else()
    message(FATAL_ERROR "set_project_warnings() function is missing! Check CompilerWarnings.cmake.")
endif()



# Include GNU standard installation module
include(GNUInstallDirs)

# Install both ProjectOptions and ProjectWarnings under the SAME EXPORT SET
install(TARGETS 
    ProjectOptions 
    ProjectWarnings
    EXPORT ProjectExportSet
)

install(EXPORT ProjectExportSet
    FILE ProjectTargets.cmake
    NAMESPACE Project::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Project
)
