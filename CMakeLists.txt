cmake_minimum_required(VERSION 3.1)
project(Project)

## main project files
set (includes "maths.h" "triangle.h" "canvas.h" "mesh.h" "camera.h")
set (files "main.cpp" "triangle.cpp" "mesh.cpp" "canvas.cpp" "camera.cpp" "maths.cpp")

add_executable(${PROJECT_NAME} ${files} ${includes})

target_link_libraries(${PROJECT_NAME} PRIVATE glut GLU GL)

