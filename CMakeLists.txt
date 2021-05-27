cmake_minimum_required(VERSION 3.19)
project(RWheader)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(
        RWheader
        trial.cpp trial2.cpp trial3.cpp trial4.cpp
        rw_bignum.cpp bignum_times.cpp main.cpp
)
