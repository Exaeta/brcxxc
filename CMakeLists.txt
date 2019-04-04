project(brcc)
cmake_minimum_required(VERSION 2.8)


set(CMAKE_CXX_STANDARD 14)

add_executable(brcc src/brcc.cpp)
install(TARGETS brcc
        RUNTIME DESTINATION bin
)
