cmake_minimum_required(VERSION 3.0)
project(MyCmakeProject VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

add_executable(MyCmakeProject MyCppCode.cpp)
set_property(TARGET MyCmakeProject PROPERTY CXX_STANDARD 17)
