cmake_minimum_required(VERSION 2.8)
project(MyOpenCVProject)

add_definitions( -std=c++17 -Wall )

# Create Main project
set(SOURCES
  main.cpp 
)

add_executable(main
    ${SOURCES}
)

target_include_directories(main PUBLIC include)

# For OpenCV
find_package(OpenCV REQUIRED)
if(OpenCV_FOUND)
    target_include_directories(main PUBLIC ${OpenCV_INCLUDE_DIRS})
    target_link_libraries(main ${OpenCV_LIBS})
endif()


