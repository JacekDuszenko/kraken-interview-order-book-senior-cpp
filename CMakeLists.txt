cmake_minimum_required(VERSION 3.16.3)
project(krakentask)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER ${CPP_COMPILER})
set(THREADS_PREFER_PTHREAD_FLAG ON)

set(SOURCE_FILES
    csv_parser.cc csv_parser.h 
    order.cc order.h
    order_book.cc order_book.h
    matching_engine.cc matching_engine.h)

add_library(core ${SOURCE_FILES} )
target_include_directories(core
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
  )

find_package(Threads REQUIRED)

add_executable(krakentask main.cc)
target_link_libraries(krakentask PRIVATE core )
target_link_libraries(krakentask PRIVATE Threads::Threads)

add_subdirectory(test)