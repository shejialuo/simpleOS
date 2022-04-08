add_executable(kernel main.cpp)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

target_link_libraries(kernel print)

set_target_properties(kernel
  PROPERTIES
  SUFFIX .bin
)

target_compile_options(kernel
  PRIVATE
  -m32 -no-pie -fno-stack-protector -fno-builtin
)

target_link_options(kernel
  PRIVATE "LINKER:-melf_i386"
  PRIVATE -Ttext 0xc0001500 -e main
  PRIVATE -m32
)

add_custom_command(TARGET kernel POST_BUILD
  COMMAND
  dd if=${PROJECT_SOURCE_DIR}/bin/kernel.bin
     of=${PROJECT_SOURCE_DIR}/img/hd60M.img bs=512 count=200 seek=9 conv=notrunc
)
