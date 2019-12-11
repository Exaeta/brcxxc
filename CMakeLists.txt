cmake_minimum_required(VERSION 3.15)


project(brcxxc VERSION 2.1.1)


add_executable(brcxxc src/brcxxc.cpp)
set_target_properties(brcxxc PROPERTIES CXX_STANDARD 17)
target_link_libraries(brcxxc stdc++fs)

install(TARGETS brcxxc
        RUNTIME DESTINATION bin
)
