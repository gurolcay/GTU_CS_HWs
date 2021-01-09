cmake_minimum_required(VERSION 3.10)
project(Final C)

set(CMAKE_C_STANDARD 11)

add_executable(Final server.c server.h client.c client.h)