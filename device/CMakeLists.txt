set(LIBRARY_FILE
  timer.cpp
)

add_library(device ${LIBRARY_FILE})

target_compile_options(device
  PRIVATE
  -m32 -fno-stack-protector -fno-builtin -Wno-write-strings
)
