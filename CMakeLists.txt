cmake_minimum_required(VERSION 3.22.0)
project(linux_ncnn_nanodet VERSION 0.3.0)

message(STATUS "CMAKE_SYSTEM_VERSION 是 : ${CMAKE_SYSTEM_VERSION}"  )
message(STATUS "CMAKE_SYSTEM_NAME 是 : ${CMAKE_SYSTEM_NAME}"  )

if (Darwin STREQUAL ${CMAKE_SYSTEM_NAME})
    # 出此下测,因为这个参数是通用的参数,不会导致报错 
    message(STATUS " 给 mac 编译"  )
    find_package(OpenCV REQUIRED COMPONENTS core highgui imgproc videoio PATHS /opt/homebrew/Cellar/opencv/4.5.5/include/opencv4)
    set(ncnn_DIR /opt/homebrew/Cellar/ncnn/20220216 )  #3rdparty/ncnn
    find_package(ncnn REQUIRED)
    # find_package(ncnn REQUIRED ) #可以找到系统中用 brew 安装的
    # message(STATUS "找到的 OpenCV_INCLUDE_DIRS 地址 : ${OpenCV_INCLUDE_DIRS}")
    # message(STATUS "找到的 OpenCV_LIBS : ${OpenCV_LIBS}" )
elseif(${CMAKE_SYSTEM_NAME} STREQUAL Android)
    message(STATUS "给 andorid 编译" )
    set(OpenCV_DIR 3rdparty/opencv-mobile-4.5.4-android/sdk/native/jni)
    set(ncnn_DIR 3rdparty/ncnn-20220216-android-vulkan/arm64-v8a/lib/cmake/ncnn) 
    find_package(OpenCV REQUIRED COMPONENTS core highgui imgproc ) 
    find_package(ncnn REQUIRED)
else()
    message(STATUS "找不到系统"  )
endif()


#  Debug 信息
message(STATUS "找到的 OpenCV_INCLUDE_DIRS 地址 : ${OpenCV_INCLUDE_DIRS}")
message(STATUS "找到的 OpenCV_LIBS : ${OpenCV_LIBS}" )

add_executable(linux_ncnn_nanodet main.cpp
                src/nanodet.cpp
                include/nanodet_ncnn_functions.hpp
                )


target_link_libraries(linux_ncnn_nanodet ncnn ${OpenCV_LIBS} ) #