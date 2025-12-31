cmake_minimum_required(VERSION 3.27)
project(gramarye-component-functions C)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# WebAssembly toggle
option(BUILD_WEB "Build with Emscripten (WebAssembly)" OFF)

if(EMSCRIPTEN OR BUILD_WEB)
    set(PLATFORM Web CACHE STRING "" FORCE)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif()

# Option to build as shared or static (default static)
option(BUILD_SHARED_LIBS "Build shared library" OFF)

# ============================================================================
# Dynamic Component Function Folder Selection
# ============================================================================
# This system allows you to selectively build component function folders.
# 
# Usage examples:
#   1. Build all folders (default):
#      cmake ..
# 
#   2. Build specific folders via list:
#      cmake -DCOMPONENT_FUNCTIONS_FOLDERS="core;textures;tilemap" ..
# 
#   3. Disable all, then enable specific ones:
#      cmake -DBUILD_COMPONENT_FUNCTIONS_ALL=OFF -DBUILD_COMPONENT_FUNCTIONS_CORE=ON ..
# 
#   4. For a 2D game, exclude 3D component functions:
#      cmake -DCOMPONENT_FUNCTIONS_FOLDERS="core;textures;tilemap" ..  # excludes "3d" folder if it exists
#
# Discover available component function folders
file(GLOB COMPONENT_FUNCTION_FOLDERS RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/src" "${CMAKE_CURRENT_SOURCE_DIR}/src/*")
list(FILTER COMPONENT_FUNCTION_FOLDERS INCLUDE REGEX "^[^/]+$")  # Only top-level directories
# Filter out files and keep only directories
set(VALID_COMPONENT_FUNCTION_FOLDERS "")
foreach(FOLDER ${COMPONENT_FUNCTION_FOLDERS})
    if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/${FOLDER}")
        list(APPEND VALID_COMPONENT_FUNCTION_FOLDERS ${FOLDER})
    endif()
endforeach()
set(COMPONENT_FUNCTION_FOLDERS ${VALID_COMPONENT_FUNCTION_FOLDERS})

# Option to build all folders (default ON)
option(BUILD_COMPONENT_FUNCTIONS_ALL "Build all component function folders" ON)

# Option to specify a list of folders to build (semicolon-separated)
# Example: -DCOMPONENT_FUNCTIONS_FOLDERS="core;textures;tilemap"
set(COMPONENT_FUNCTIONS_FOLDERS "" CACHE STRING "List of component function folders to build (semicolon-separated). Empty means use BUILD_COMPONENT_FUNCTIONS_ALL or individual flags.")

# Individual folder options (auto-generated)
set(ENABLED_COMPONENT_FUNCTION_FOLDERS "")
foreach(FOLDER ${COMPONENT_FUNCTION_FOLDERS})
    string(TOUPPER ${FOLDER} FOLDER_UPPER)
    option(BUILD_COMPONENT_FUNCTIONS_${FOLDER_UPPER} "Build ${FOLDER} component functions" ON)
    if(BUILD_COMPONENT_FUNCTIONS_${FOLDER_UPPER})
        list(APPEND ENABLED_COMPONENT_FUNCTION_FOLDERS ${FOLDER})
    endif()
endforeach()

# Process folder selection logic
if(COMPONENT_FUNCTIONS_FOLDERS)
    # Explicit list provided - validate and use it
    set(VALIDATED_FOLDERS "")
    foreach(FOLDER ${COMPONENT_FUNCTIONS_FOLDERS})
        if(FOLDER IN_LIST COMPONENT_FUNCTION_FOLDERS)
            list(APPEND VALIDATED_FOLDERS ${FOLDER})
        else()
            message(WARNING "Component function folder '${FOLDER}' does not exist. Available folders: ${COMPONENT_FUNCTION_FOLDERS}")
        endif()
    endforeach()
    set(ENABLED_COMPONENT_FUNCTION_FOLDERS ${VALIDATED_FOLDERS})
    message(STATUS "Building specified component function folders: ${ENABLED_COMPONENT_FUNCTION_FOLDERS}")
elseif(BUILD_COMPONENT_FUNCTIONS_ALL)
    # Build all folders
    set(ENABLED_COMPONENT_FUNCTION_FOLDERS ${COMPONENT_FUNCTION_FOLDERS})
    message(STATUS "Building all component function folders: ${ENABLED_COMPONENT_FUNCTION_FOLDERS}")
else()
    # Use individual flags
    message(STATUS "Building component function folders based on individual flags: ${ENABLED_COMPONENT_FUNCTION_FOLDERS}")
endif()

# Validate that at least one folder is enabled
if(NOT ENABLED_COMPONENT_FUNCTION_FOLDERS)
    message(WARNING "No component function folders enabled! Please enable at least one folder or set BUILD_COMPONENT_FUNCTIONS_ALL=ON")
    message(STATUS "Available folders: ${COMPONENT_FUNCTION_FOLDERS}")
endif()

# Summary
message(STATUS "=== Component Function Folder Selection Summary ===")
message(STATUS "Available folders: ${COMPONENT_FUNCTION_FOLDERS}")
message(STATUS "Enabled folders: ${ENABLED_COMPONENT_FUNCTION_FOLDERS}")
message(STATUS "===================================================")

# Fetch gramarye-libcore
include(FetchContent)
FetchContent_Declare(
    gramarye_libcore
    GIT_REPOSITORY "https://github.com/noahspoling/gramarye-libcore.git"
    GIT_TAG "main"
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(gramarye_libcore)

# Fetch gramarye-ecs
FetchContent_Declare(
    gramarye_ecs
    GIT_REPOSITORY "https://github.com/noahspoling/gramarye-ecs.git"
    GIT_TAG "main"
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(gramarye_ecs)

# Fetch gramarye-components (for component struct definitions)
FetchContent_Declare(
    gramarye_components
    GIT_REPOSITORY "https://github.com/noahspoling/gramarye-components.git"
    GIT_TAG "main"
    GIT_PROGRESS TRUE
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(gramarye_components)

# Collect source files from enabled folders
set(ALL_COMPONENT_FUNCTION_SRC_FILES "")
if(ENABLED_COMPONENT_FUNCTION_FOLDERS)
    message(STATUS "Collecting source files from enabled folders...")
    foreach(FOLDER ${ENABLED_COMPONENT_FUNCTION_FOLDERS})
        file(GLOB FOLDER_SRC_FILES "src/${FOLDER}/*.c")
        if(FOLDER_SRC_FILES)
            list(APPEND ALL_COMPONENT_FUNCTION_SRC_FILES ${FOLDER_SRC_FILES})
            message(STATUS "  [${FOLDER}] ${FOLDER_SRC_FILES}")
        else()
            message(STATUS "  [${FOLDER}] (no source files found)")
        endif()
    endforeach()
    message(STATUS "Total component function source files: ${ALL_COMPONENT_FUNCTION_SRC_FILES}")
else()
    message(WARNING "No component function folders enabled - library will be empty!")
endif()

# Create library
add_library(gramarye-component-functions 
    ${ALL_COMPONENT_FUNCTION_SRC_FILES}
)

# Add include directory
target_include_directories(gramarye-component-functions PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Make gramarye-components headers available to component-functions
# This allows component-functions headers to include "core/position.h" etc.
target_include_directories(gramarye-component-functions PRIVATE
    $<TARGET_PROPERTY:gramarye-components,INTERFACE_INCLUDE_DIRECTORIES>
)

# Link against dependencies
target_link_libraries(gramarye-component-functions PUBLIC 
    gramarye-libcore
    gramarye-ecs
    gramarye-components
)

# Link raylib for raylib-dependent functions (Atlas, Animation, etc.)
# Note: raylib should be available from the parent project
# If building standalone, uncomment and configure raylib here
# find_package(raylib REQUIRED)
# target_link_libraries(gramarye-component-functions PUBLIC raylib)

# Export for CMake
include(GNUInstallDirs)
install(TARGETS gramarye-component-functions
    EXPORT componentFunctionsTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(EXPORT componentFunctionsTargets
    FILE componentFunctionsTargets.cmake
    NAMESPACE gramarye-component-functions::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/component-functions
)

# For FetchContent compatibility
if(NOT TARGET gramarye-component-functions::gramarye-component-functions)
    add_library(gramarye-component-functions::gramarye-component-functions ALIAS gramarye-component-functions)
endif()

