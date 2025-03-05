
if(NOT TARGET ProjectWarnings)
    add_library(ProjectWarnings INTERFACE)
endif()

function(set_project_warnings target)
    if (MSVC)
        add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
        target_compile_options(${target} INTERFACE
            /W4                # Enables a high level of warnings (Recommended for production)
            /WX                # Treat warnings as errors (Prevents ignoring important issues)
            /permissive-       # Enforces strict C++ standard compliance (Prevents old MSVC extensions)
            /sdl               # Enables additional security checks (Helps find buffer overflows)
            /Zc:preprocessor   # Uses the conforming preprocessor (Fixes some non-standard behavior)
        )
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(${target} INTERFACE
            -Wall              # Enables a basic set of warnings
            -Wextra            # Enables extra warnings for better code quality
            -Wpedantic         # Ensures strict compliance with the C++ standard
            -Wshadow           # Warns if a variable in an inner scope shadows an outer variable
            -Wconversion       # Warns about implicit type conversions that may lose data
            -Wsign-conversion  # Warns when signed and unsigned values are mixed in expressions
            -Wunused           # Warns about unused variables, parameters, functions, etc.
            -Werror            # Treats all warnings as errors (prevents compiling bad code)
        )
    endif()

    # Link the warnings library to the target
    target_link_libraries(${target} INTERFACE ProjectWarnings)
endfunction()