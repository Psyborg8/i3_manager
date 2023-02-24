cmake_minimum_required( VERSION 3.9 )

# Build commands
set( CMAKE_CXX_STANDARD 20 )
set( CMAKE_BUILD_TYPE "Debug" )
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

option( BUILD_SHARED_LIBS "Build using shared libraries" ON )

set( PROJECT_BINARY_DIR ${CMAKE_CURRENT_LIST_DIR}/bin )
set( PROJECT_SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/src )

# Dependancies
set( DEPS deps/i3ipcpp deps/cpp_logger )
foreach( DEP ${DEPS} )
    add_subdirectory( ${DEP} )
endforeach()

# Create executable
project( i3_manager VERSION 0.0.1 LANGUAGES CXX )

file( GLOB_RECURSE SRC src/*.cpp )
add_executable( i3_manager ${SRC} )

add_dependencies( i3_manager cpp_logger i3ipc++ )

target_include_directories( i3_manager PUBLIC ${PROJECT_SOURCE_DIR} )
target_link_libraries( i3_manager cpp_logger i3ipc++ )

# Compile
target_compile_options( i3_manager 
    PRIVATE 
        -std=c++20 
        -Wall 
        -Wextra
        -Wno-unused-parameter 
        )

if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
    target_compile_options( i3_manager PUBLIC -ggdb3 -Og )
    target_compile_definitions( i3_manager PRIVATE _DEBUG=1 )
elseif( CMAKE_BUILD_TYPE STREQUAL "Release" )
    target_compile_options( i3_manager PUBLIC -O2 )
endif()

# Install
install( TARGETS i3_manager )
