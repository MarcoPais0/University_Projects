cmake_minimum_required(VERSION 3.17)
project(PPP_Trabalho3_Parte1 C)

set(CMAKE_C_STANDARD 11)

add_compile_options(-Wall -Wextra)

add_executable(PPP_Trabalho3_Parte1 main.c aux_func.c aux_func.h)