cmake_minimum_required(VERSION 3.17)
project(imageConverter)

set(CMAKE_CXX_STANDARD 17)

# libs

find_package(OpenCV REQUIRED)
find_package(range-v3 REQUIRED)
find_package(fmt REQUIRED)
find_path(CSV_INCLUDE_DIR csv.h REQUIRED)

add_library(neededLibs INTERFACE)

target_link_libraries(neededLibs INTERFACE
        ${OpenCV_LIBS}
        fmt::fmt
        range-v3::range-v3)

target_include_directories(neededLibs INTERFACE
        ${OpenCV_INCLUDE_DIRS}
        ${CSV_INCLUDE_DIR})

# targets

add_executable(imageConverter main.cpp)
target_link_libraries(imageConverter PRIVATE neededLibs)

add_executable(dataSplit dataSplit.cpp)
target_link_libraries(dataSplit PRIVATE neededLibs)