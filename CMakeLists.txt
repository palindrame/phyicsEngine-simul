cmake_minimum_required(VERSION 3.16)
project(cloth_sfml LANGUAGES CXX)

# Require C++17 (SFML 3 requires modern C++)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML 3 (note components capitalized in SFML 3)
find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

add_executable(cloth_sim
    main.cpp
)

# Link SFML targets (modern CMake targets provided by SFML's config)
target_link_libraries(cloth_sim PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
)

# On Linux, it's common to need pthreads; let CMake handle it if required.
find_package(Threads)
if (Threads_FOUND)
    target_link_libraries(cloth_sim PRIVATE Threads::Threads)
endif()

# Set output directory
set_target_properties(cloth_sim PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

