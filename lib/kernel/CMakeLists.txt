set(LIBRARY_CPP_FILE
  bitmap.cpp list.cpp
)

set(LIBRARY_ASSEMBLY_FILE
  print.nasm)

add_library(LibKernelCPP STATIC ${LIBRARY_CPP_FILE})
add_library(LibKernelASSEMBLY STATIC ${LIBRARY_ASSEMBLY_FILE})

set_target_properties(LibKernelASSEMBLY PROPERTIES NASM_OBJ_FORMAT elf)

target_compile_options(LibKernelCPP
  PRIVATE
  -m32 -fno-stack-protector -fno-builtin -Wno-write-strings
)
