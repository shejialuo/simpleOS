include_directories(./include)

enable_language(ASM_NASM)

# By default, cmake only recognizes .asm and .nasm. So we need to extend this.
# See https://stackoverflow.com/questions/56420035/how-to-use-cmake-to-build-binaries-with-nasm/
set(CMAKE_ASM_NASM_SOURCE_FILE_EXTENSIONS ${CMAKE_ASM_NASM_SOURCE_FILE_EXTENSIONS} s S)

# We need the output format to be `bin`.
set(CMAKE_ASM_NASM_OBJECT_FORMAT bin)

set(CMAKE_ASM_NASM_FLAGS "-e")

set(CMAKE_ASM_NASM_LINK_EXECUTABLE "nasm <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")

add_compile_options(-I ${CMAKE_CURRENT_SOURCE_DIR}/include/)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_executable(loader loader.nasm)
add_executable(mbr mbr.nasm)

set_target_properties(mbr loader
  PROPERTIES
  SUFFIX .bin
)

add_custom_command(TARGET mbr POST_BUILD
  COMMAND
  dd if=${PROJECT_SOURCE_DIR}/bin/mbr.bin
     of=${PROJECT_SOURCE_DIR}/img/hd60M.img bs=512 count=1 conv=notrunc
)

add_custom_command(TARGET loader POST_BUILD
   COMMAND
    dd if=${PROJECT_SOURCE_DIR}/bin/loader.bin
       of=${PROJECT_SOURCE_DIR}/img/hd60M.img bs=512 count=4 seek=2 conv=notrunc
)