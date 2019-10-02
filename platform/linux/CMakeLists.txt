cmake_minimum_required (VERSION 2.6)
project (withmyengine)
add_executable (withmyengine main.cpp)
target_link_libraries (withmyengine GLESv2 EGL xcb)


