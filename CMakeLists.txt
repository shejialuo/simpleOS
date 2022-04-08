cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(simpleOS LANGUAGES CXX ASM_NASM)

# Globally set the NASM assembly
# See https://stackoverflow.com/questions/56420035/how-to-use-cmake-to-build-binaries-with-nasm
set(CMAKE_ASM_NASM_COMPILE_OBJECT "<CMAKE_ASM_NASM_COMPILER> <INCLUDES> <FLAGS> -o <OBJECT> <SOURCE>")
add_compile_options(
    "$<$<COMPILE_LANGUAGE:ASM_NASM>:-f $<IF:$<BOOL:$<TARGET_PROPERTY:NASM_OBJ_FORMAT>>, \
    $<TARGET_PROPERTY:NASM_OBJ_FORMAT>, ${CMAKE_ASM_NASM_OBJECT_FORMAT}>>"
)

include_directories(
  ${PROJECT_SOURCE_DIR}/lib
  ${PROJECT_SOURCE_DIR}/lib/kernel
)

add_subdirectory(./boot)
add_subdirectory(./lib)
add_subdirectory(./kernel)
