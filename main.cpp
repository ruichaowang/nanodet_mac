#include <iostream>
#include <fstream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
// #include <opencv2/calib3d.hpp>
#include <opencv2/core/types_c.h>


#include "./include/nanodet_ncnn_functions.hpp"         //使用的时候只需要直接加载此header，当然之后可以选择封装好 so 包。
using namespace std;
// using namespace cv;

int main(int, char* argv[]) {
  std::cout << "Hello, World!\n";
  const char* model_path=  argv[1]; // 定义为我们真正要添加的路径
  const char* sample_image_path =  argv[2]; //输入的图像路径
  const char* image_saving_path =  argv[3]; //输入的图像路径

  loadModel(model_path, 0, 1);         // 加载模型，当前为第一个 m，即编号0，不用 gpu，且需要把model_path 传进去
  MyNdkCamera sampleimg;  // 随便创建一个叫做 sampleimg 的  MyNdkCamera 

  //这部分是图片的sample 
  cv::Mat img = cv::imread(sample_image_path);    // 读取
  sampleimg.on_image_render(img);                 // 运行模型
  cv::imwrite(image_saving_path, img);            // 保存图像
  


  //这部分是视频sample 
  // cv::VideoCapture capture("/Users/wangruichao/Work/NCNN/nanodet_mac/data/IMG_6221.MOV"); 
  // if ( !capture.isOpened( ) ){std::cout << "fail toopen!" << std::endl;} 
  // long totalFrameNumber = capture.get( CAP_PROP_FRAME_COUNT ); 
  // std::cout << "整个视频共" << totalFrameNumber << "帧" << std::endl;   

  // long frameToStart = 1; //设置开始帧() 
  // capture.set( CAP_PROP_FRAME_COUNT, frameToStart ); 

  // std::cout << "从第" << frameToStart << "帧开始读" << std::endl; 
  // int frameToStop = 300; // 设置结束帧 

  // if ( frameToStop < frameToStart ){ 
  //   std::cout << "结束帧小于开始帧，程序错误，即将退出！" << std::endl; 
  // return -1; 
  // } 
  // else{std::cout << "结束帧为：第" << frameToStop << "帧" << std::endl;} 

  // double rate = capture.get( CAP_PROP_FPS ); // 获取帧率 
  //   std::cout << "帧率为:" << rate << std::endl; 

  // bool stop = false; // 定义一个用来控制读取视频循环结束的变量 

  // cv::Mat frame; // 承载每一帧的图像 
  // namedWindow( "Extractedframe" ); // 显示每一帧的窗口 

  // double delay = 1000 / rate; //两帧间的间隔时间: 
  //   long currentFrame = frameToStart; 
  //   if ( !capture.read( frame ) ) 
  //   { 
  //       std::cout << "读取视频失败" << std::endl; 
  //       return -1; 
  //   } 


  // if ( !capture.read( frame ) ) 
  // { 
  //     std::cout << "读取视频失败" << std::endl; 
  //     return -1; 
  // } 

  // while ( !stop ) 
  // { 
  //     if ( !capture.read( frame ) ) 
  //     { 
  //         std::cout << "结束" << std::endl; 
  //         return 0; 
  //     } 
  //     sampleimg.on_image_render(frame);       // 模型运行，推理 & render
  //     imshow( "Extractedframe", frame ); 
  //     waitKey(1);                              //加入此代码,按下后会显示动画,30ms 内如果按住按键则会继续. 
  //     currentFrame = currentFrame + 1; 
  // } 

   
  return 0;
}