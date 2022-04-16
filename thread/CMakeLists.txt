set(LIBRARY_FILE
  thread.cpp
)

set(ASSEMBLY_FILE
  switch.nasm
)

add_library(thread STATIC ${LIBRARY_FILE})
add_library(threadASSEMBLY STATIC ${ASSEMBLY_FILE})

target_compile_options(thread
  PRIVATE
  -m32 -fno-stack-protector -fno-builtin -Wno-write-strings
)

target_link_libraries(thread PRIVATE threadASSEMBLY)

set_target_properties(threadASSEMBLY PROPERTIES NASM_OBJ_FORMAT elf)
