cmake_minimum_required(VERSION 3.28)
project(BigInt C)

# Permitir establecer modo Debug o Release
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Configuraciones específicas por tipo de compilación
set(CMAKE_C_FLAGS_DEBUG "-g -static -O0 -Wall -Wpedantic -D_ExceptionHandler")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG  -D_ExceptionHandler")

set(CMAKE_C_STANDARD 11)

include_directories(.)
include_directories(include)
include_directories(lib/DebugLibC)
include_directories(lib/DebugLibC/colors-C-C-plus-plus)
include_directories(lib/DebugLibC/colors-C-C-plus-plus/old-code)

add_executable(example
        example/example.c
        include/BigInt.h
        include/Fractions.h
        lib/DebugLibC/colors-C-C-plus-plus/src/colors.c
        lib/DebugLibC/colors-C-C-plus-plus/colors.h
        lib/DebugLibC/src/debug_c.c
        lib/DebugLibC/src/error_c.c
        lib/DebugLibC/src/time_code.c
        lib/DebugLibC/debug_c.h
        lib/DebugLibC/error_c.h
        lib/DebugLibC/time_code.h
        src/BigInt.c
        src/Fractions.c)

add_executable(example1
        example/example1.c
        include/BigInt.h
        include/Fractions.h
        lib/DebugLibC/colors-C-C-plus-plus/src/colors.c
        lib/DebugLibC/colors-C-C-plus-plus/colors.h
        lib/DebugLibC/src/debug_c.c
        lib/DebugLibC/src/error_c.c
        lib/DebugLibC/src/time_code.c
        lib/DebugLibC/debug_c.h
        lib/DebugLibC/error_c.h
        lib/DebugLibC/time_code.h
        src/BigInt.c
        src/Fractions.c)