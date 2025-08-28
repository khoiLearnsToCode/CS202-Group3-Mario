cmake_minimum_required(VERSION 3.31.0)
project(mariogame VERSION 0.1.0 LANGUAGES C CXX)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/vcpkg_installed/x64-windows")
find_package(raylib CONFIG REQUIRED)

include_directories(header)

# Comment out the old include configurations to enable relative path includes
#include all header files in the header directory
file(GLOB_RECURSE HEADERS "header/*.hpp" "header/*.h")
foreach(h ${HEADERS})
	get_filename_component(hDir ${h} DIRECTORY)
    include_directories(${hDir})
endforeach() 

# include all src files in the src directory
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.c")

# Custom target to sync resource directory
add_custom_target(sync_resources ALL
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/resource
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resource ${CMAKE_BINARY_DIR}/resource
    COMMENT "Syncing resource directory..."
)

add_executable(mariogame ${SOURCES})

# Make sure resources are synced before building the executable
add_dependencies(mariogame sync_resources)

target_link_libraries(mariogame PRIVATE raylib)

# Comment out the direct header include to enable relative path includes
target_include_directories(mariogame PRIVATE ${PROJECT_SOURCE_DIR}/header)

# Set the base directory for relative includes from src files
# This allows using #include "../header/hello.h" from src files
#target_include_directories(mariogame PRIVATE ${PROJECT_SOURCE_DIR}/src)