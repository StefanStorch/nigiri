cmake_minimum_required(VERSION 3.22)
project(nigiri)

include(cmake/buildcache.cmake)
include(cmake/pkg.cmake)

option(NIGIRI_LINT "Run clang-tidy with the compiler." OFF)
if(NIGIRI_LINT)
  # clang-tidy will be run on all targets defined hereafter
  include(cmake/clang-tidy.cmake)
endif()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  set(nigiri-compile-options
        -Weverything
        -Wno-c++98-compat
        -Wno-c++98-compat-pedantic
        -Wno-newline-eof
        -Wno-missing-prototypes
        -Wno-padded
        -Wno-double-promotion
        -Wno-undef
        -Wno-undefined-reinterpret-cast
        -Wno-float-conversion
        -Wno-global-constructors
        -Wno-exit-time-destructors
        -Wno-switch-enum
        -Wno-c99-designator
        -Wno-zero-as-null-pointer-constant
        -Wno-missing-noreturn
        -Wno-undefined-func-template
        -Werror)
elseif(MSVC)
  set(nigiri-compile-options /WX)
else()
  set(nigiri-compile-options ${SORO_COMPILE_OPTIONS}
        -Wall
        -Wextra
        -Werror)
endif()

# --- LIB ---
file(GLOB_RECURSE nigiri-files src/*.cc)
add_library(nigiri ${nigiri-files})
target_include_directories(nigiri PUBLIC include)
target_link_libraries(nigiri PUBLIC cista geo utl fmt date miniz date-tz wyhash)
target_compile_features(nigiri PUBLIC cxx_std_20)
target_compile_options(nigiri PRIVATE ${nigiri-compile-options})

# --- MAIN ---
file(GLOB_RECURSE nigiri-server-files server/main.cc)
add_executable(nigiri-server ${nigiri-server-files})
target_link_libraries(nigiri-server PRIVATE nigiri)
target_compile_features(nigiri-server PUBLIC cxx_std_20)

# --- TEST ---
add_library(nigiri-generated INTERFACE)
target_include_directories(nigiri-generated INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/generated)
configure_file (
  ${CMAKE_CURRENT_SOURCE_DIR}/test/test_dir.h.in
  ${CMAKE_CURRENT_BINARY_DIR}/generated/test_dir.h
)
file(GLOB_RECURSE nigiri-test-files test/*.cc)
add_executable(nigiri-test ${nigiri-test-files})
target_link_libraries(nigiri-test nigiri doctest ianatzdb-res nigiri-generated)
target_compile_options(nigiri-test PRIVATE ${nigiri-compile-options})
