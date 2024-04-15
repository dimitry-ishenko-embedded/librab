####################
## rab
add_library(rab OBJECT
    board.c board.h
    ivt.c ivt.h
    reg.c reg.h
    serial.c serial.h
    timera.c timera.h
    types.h
)
target_include_directories(rab INTERFACE .)

####################
## crt0
add_custom_command(OUTPUT crt0.rel
    COMMAND sdasrab -g -o crt0.rel ${CMAKE_CURRENT_SOURCE_DIR}/crt0.s
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/crt0.s
)
add_custom_target(crt0_rel DEPENDS crt0.rel)

add_library(crt0 OBJECT IMPORTED GLOBAL)
add_dependencies(crt0 crt0_rel)
set_target_properties(crt0 PROPERTIES IMPORTED_OBJECTS ${CMAKE_CURRENT_BINARY_DIR}/crt0.rel)

####################
# in order to make executables with custom crt0 we have to add an intermediate
# target to compile object files and then separately link them with crt0
# specifying it (crt0) as the first item to link
function(add_executable_crt0 name)
    add_library(_${name} OBJECT ${ARGN})
    add_executable(${name} $<TARGET_OBJECTS:crt0> $<TARGET_OBJECTS:_${name}>)
    target_link_options(${name} PRIVATE --no-std-crt0 --code-loc 0x100)

    add_custom_command(TARGET ${name} POST_BUILD COMMAND ${CMAKE_OBJCOPY} -I ihex -O binary ${name}.ihx ${name}.bin)
endfunction()

function(target_link_libraries_crt0 name)
    target_link_libraries(_${name} ${ARGN})
    target_link_libraries(${name} ${ARGN})
endfunction()
