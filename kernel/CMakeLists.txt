set(COMPILE_FILE
  main.cpp init.cpp interrupt.cpp
)

set(LIBRARY_FILE
  kernel.nasm
)

add_executable(kernel ${COMPILE_FILE})
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_library(kernelSelf STATIC ${LIBRARY_FILE})

target_link_libraries(kernel device kernelSelf  print)

set_target_properties(kernel
  PROPERTIES
  SUFFIX .bin
)

set_target_properties(kernelSelf PROPERTIES NASM_OBJ_FORMAT elf)

target_compile_options(kernel
  PRIVATE
  -m32 -no-pie -fno-stack-protector -fno-builtin -Wno-write-strings
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
