add_subdirectory(./kernel)

set(LIBRARY_CPP_FILE
  string.cpp
)

add_library(Lib ${LIBRARY_CPP_FILE})

target_compile_options(Lib
  PRIVATE
  -m32 -fno-stack-protector -fno-builtin -Wno-write-strings
)