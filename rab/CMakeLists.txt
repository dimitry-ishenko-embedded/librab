####################
## rab
add_library(rab OBJECT
    board.c board.h
    ivt.c ivt.h
    reg.c reg.h
    serial.c serial.h
    types.h
)
target_include_directories(rab INTERFACE .)
