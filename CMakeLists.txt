cmake_minimum_required(VERSION 3.5)
project(analizadorLexico)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

set(SOURCE_FILES main.c InputSystem/iosystem.h InputSystem/iosystem.c InputSystem/iosystem.h definitions SimbolTable/HashTable.c SimbolTable/HashTable.h LexicalAnalyzer/lexeme.c LexicalAnalyzer/lexeme.h SimbolTable/Loader.c SimbolTable/Loader.h LexicalAnalyzer/lexicalAnalyzer.c LexicalAnalyzer/lexicalAnalyzer.h LexicalAnalyzer/Errors.c LexicalAnalyzer/Errors.h)
add_executable(analizadorLexico ${SOURCE_FILES})