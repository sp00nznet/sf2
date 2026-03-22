cmake_minimum_required(VERSION 3.16)
project(sf2 C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

# SDL2
find_package(SDL2 REQUIRED)

# cps1recomp library (sibling directory)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../cps1recomp ${CMAKE_BINARY_DIR}/cps1recomp)

# Gather all generated recompiled source files
file(GLOB RECOMP_SOURCES "src/recomp/recomp_*.c")

# --- sf2 executable ---
add_executable(sf2
    src/main.c
    src/task_fiber.c
    ${RECOMP_SOURCES}
)

target_link_libraries(sf2
    PRIVATE cps1recomp
    PRIVATE SDL2::SDL2main
)

target_include_directories(sf2
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(MSVC)
    # Disable some noisy warnings in generated code
    target_compile_options(sf2 PRIVATE /W3 /wd4244 /wd4267 /wd4018 /wd4146)
else()
    target_compile_options(sf2 PRIVATE -Wall -Wno-unused-label -Wno-unused-variable)
endif()
