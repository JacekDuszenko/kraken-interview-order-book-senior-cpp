cmake_minimum_required(VERSION 3.16.3)
project(krakentask)

set(CMAKE_CXX_STANDARD 20)

set(
        SOURCE_FILES

        csv_parser.cc csv_parser.h order.cc order.h)

add_library(core ${SOURCE_FILES})
target_include_directories(core
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
  )
add_executable(krakentask main.cc)
target_link_libraries(krakentask PRIVATE core)

add_subdirectory(test)