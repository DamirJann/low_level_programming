cmake_minimum_required(VERSION 3.21)
project(github C)

set(CMAKE_C_STANDARD 99)

add_executable(github ./solution/src/main.c solution/include/abstract_image.h solution/src/abstract_image.c solution/include/bpm_image.h solution/src/bpm_image.c solution/include/image_transformation.h solution/src/image_transformation.c solution/include/utils.h solution/src/utils.c)
