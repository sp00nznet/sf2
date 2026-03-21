cmake_minimum_required(VERSION 3.16)
project(sf2 C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

# SDL2
find_package(SDL2 REQUIRED)

# cps1recomp library (sibling directory)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../cps1recomp ${CMAKE_BINARY_DIR}/cps1recomp)

# --- sf2 executable ---
add_executable(sf2
    src/main.c
    # Generated recompiled code files will be added here:
    # src/recomp/recomp_000000_00FFFF.c
    # src/recomp/recomp_010000_01FFFF.c
    # ...
)

target_link_libraries(sf2
    PRIVATE cps1recomp
    PRIVATE SDL2::SDL2main
)

target_include_directories(sf2
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(MSVC)
    target_compile_options(sf2 PRIVATE /W4)
else()
    target_compile_options(sf2 PRIVATE -Wall -Wextra)
endif()
