cmake_minimum_required(VERSION 3.12)

add_executable(build-hello-world-class
        hello-world.cpp
)

target_link_libraries(build-hello-world-class
        PRIVATE jvm::ClassBuilder
)
