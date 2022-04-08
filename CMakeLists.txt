cmake_minimum_required(VERSION 3.22.0)
project(linux_ncnn_nanodet VERSION 0.1.0)

find_package(OpenCV REQUIRED COMPONENTS core highgui imgproc videoio PATHS /opt/homebrew/Cellar/opencv/4.5.5/include/opencv4)
message(STATUS "找到的 OpenCV_INCLUDE_DIRS 地址 : ${OpenCV_INCLUDE_DIRS}")
message(STATUS "找到的 OpenCV_LIBS : ${OpenCV_LIBS}" )

find_package(ncnn REQUIRED PATHS /Users/wangruichao/Work/NCNN/nanodet_mac/3rdparty/ncnn)
# 我不知道他找到的是哪里的路径，当前的应该是系统中的，
# find_package(ncnn REQUIRED ) #可以找到系统中用 brew 安装的

# add_library 的事以后再说吧。
add_executable(linux_ncnn_nanodet main.cpp
                src/nanodet.cpp
                include/nanodet_ncnn_functions.hpp
                )


target_link_libraries(linux_ncnn_nanodet ncnn ${OpenCV_LIBS} ) #