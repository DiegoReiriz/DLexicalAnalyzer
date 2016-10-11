cmake_minimum_required(VERSION 3.5)
project(analizadorLexico)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

set(SOURCE_FILES main.c InputSystem/iosystem.h InputSystem/iosystem.c InputSystem/iosystem.h definitions.h LexicalAnalyzer/GrapghRegEx.c LexicalAnalyzer/GrapghRegEx.h LexicalAnalyzer/HashTable.c LexicalAnalyzer/HashTable.h LexicalAnalyzer/lexeme.c LexicalAnalyzer/lexeme.h)
add_executable(analizadorLexico ${SOURCE_FILES})