cmake_minimum_required(VERSION 3.17)
project(PPP_Trabalho3_Parte2 C)

set(CMAKE_C_STANDARD 11)

add_compile_options(-Wall -Wextra)

add_executable(PPP_Trabalho3_Parte2 main.c aux_func.c aux_func.h struct.c struct.h lib-utf8.c lib-utf8.h)