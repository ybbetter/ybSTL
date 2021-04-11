cmake_minimum_required(VERSION 3.17)
project(learncpp)

set(CMAKE_CXX_STANDARD 14)

add_executable(learncpp main.cpp stl/alloc.h stl/alloc.cpp stl/typeTraits.h stl/Iterator.h stl/reverseIterator.h stl/vector.h stl/vector.cpp stl/allocator.h stl/construct.h stl/uninitialized.h)