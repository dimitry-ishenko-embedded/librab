cmake_minimum_required(VERSION 3.16) # focal

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_SYSTEM_NAME Generic)

set(CMAKE_C_COMPILER sdcc)
set(CMAKE_OBJCOPY sdobjcopy)

## convert .ihx to .bin
function(create_binary name)
    add_custom_command(TARGET ${name} POST_BUILD COMMAND ${CMAKE_OBJCOPY} -I ihex -O binary ${name}.ihx ${name}.bin)
endfunction()

########################################
project(hello_world VERSION 0.1)

set(PROJECT_COMPILE_OPTIONS -mr3ka --std-c23 --opt-code-speed --max-allocs-per-node 100000)
add_compile_options(${PROJECT_COMPILE_OPTIONS})
add_link_options(${PROJECT_COMPILE_OPTIONS})

add_compile_definitions(VERSION="${PROJECT_VERSION}")

add_subdirectory(rab)

add_executable(main
    main.c
)
target_link_libraries(main PRIVATE rab)
create_binary(main)
