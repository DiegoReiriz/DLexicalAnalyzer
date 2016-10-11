cmake_minimum_required(VERSION 3.5)
project(analizadorLexico)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

set(SOURCE_FILES main.c SistemaDeEntrada/iosystem.h SistemaDeEntrada/iosystem.c SistemaDeEntrada/iosystem.h definitions.h)
add_executable(analizadorLexico ${SOURCE_FILES})