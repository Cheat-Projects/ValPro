cmake_minimum_required(VERSION 3.27.0)

# Set global C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


# Create InterviewPracticeQuestions project
project(
    ValPro
    VERSION 1.0
    DESCRIPTION "ValPro"
    LANGUAGES CXX
)


# Add the src directory to the build
add_subdirectory(src)

# Set standard project settings
include(cmake/StandardProjectSettings.cmake)

# Set static analysers
include(cmake/StaticAnalysers.cmake)

# set compiler warnings
include(cmake/CompilerWarnings.cmake)

# Set project options
include(cmake/ProjectOptions.cmake)
