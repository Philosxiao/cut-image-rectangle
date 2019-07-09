cmake_minimum_required(VERSION 2.8.12)
project(show)

# Find rquired packeges
find_package(OpenCV 3.4.2 REQUIRED)

find_package(PCL REQUIRED)
include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

add_executable(${PROJECT_NAME} 
src/main.cpp 
)

include_directories(include)
include_directories(${OpenCV_INCLUDE_DIRS})

# Enable OPENCV 
add_definitions(-DOPENCV)

target_link_libraries(${PROJECT_NAME}    
${OpenCV_LIBRARIES}
${OpenCV_LIBS}
)