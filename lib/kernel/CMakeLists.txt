add_library(print STATIC print.nasm)

set_target_properties(print PROPERTIES NASM_OBJ_FORMAT elf)
