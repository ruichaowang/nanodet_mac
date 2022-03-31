#include <iostream>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <chrono>
#include <stack>

using namespace std;
using namespace cv;
using namespace std::chrono;

// Timer
class Timer{ 
  public: 
    std::stack<high_resolution_clock::time_point> tictoc_stack; 
  void tic(){ 
    high_resolution_clock::time_point t1=high_resolution_clock::now(); 
    tictoc_stack.push(t1); 
  } 
  double toc(std::string msg="",bool flag=true ){ 
    double diff = duration_cast<nanoseconds> (high_resolution_clock::now() - tictoc_stack.top()).count(); 
    if(msg.size()>0){ 
    if(flag){ 
      printf("%s Time cost:%f ms\n",msg.c_str(),diff/1000000); 
    } 
    } 
    tictoc_stack.pop(); 
    return diff; 
  } 
  void reset(){ 
    tictoc_stack = std::stack<high_resolution_clock::time_point>(); 
  } 
}; 

// 读取图像到 buffer
int readfile(const char* filename,std::vector<unsigned char>& buf){
  FILE* fp=fopen(filename,"rb");
  if(NULL==fp){
    std::cout << "Cloud Not Opent:"<< filename <<std::endl;
    return -1;
  }

  fseek(fp,0,SEEK_END);
  unsigned int fileLen=ftell(fp);
  fseek(fp,0,SEEK_SET);
  buf.resize(fileLen);
  fread(&buf[0],fileLen,1,fp);
  fclose(fp);
  return 0;
}

/*
  读取 RGB 图像
  当前的问题是，如果图像地址错误，找不到图像，此时不回报错，之后程序就有可能炸了不容易debug
*/
Mat Read_Save_8bit_RGB_image(const char* raw_image_file, int width, int height){
  int flag = 0;
  // 为了方便使用，直接 Return 图像数据。
  // std::cout<<"Read_8bit_RGB_image: "<<raw_image_file<<" , width: "<<width<<" , height:"<<height<<std::endl;   // 这里没问题
  
  std::vector<unsigned char> raw_info;
  flag = readfile(raw_image_file, raw_info);
  if(flag == 0){std::cout << "load raw data is done"<<std::endl;}
  else{std::cout << "load raw data fail"<<std::endl;}
  
  // 创建出色彩空间
  int datalength = width * height;
  std::vector<unsigned char> rgb;
  std::vector<unsigned char> red;
  std::vector<unsigned char> green;
  std::vector<unsigned char> blue;
  rgb.resize(datalength*3);                     
  red.resize(datalength);
  green.resize(datalength);
  blue.resize(datalength);
  // std::cout << "datalength="<<datalength<<std::endl;
  for(int i=0;i<datalength;i++){
    red[i] = raw_info[3*i];
    green[i] = raw_info[3*i+1];
    blue[i] = raw_info[3*i+2];
  }

  Vec3b color;
  Mat RGB_image(height, width, CV_8UC3);        //RGB image  CV_8UC3
  for (int i = 0; i < height; i++){
    for (int j = 0; j < width; j++){
      color[0] = red[i * width + j];
      color[1] = green[i * width + j]; 
      color[2] = blue[i * width + j]; 
      RGB_image.at<Vec3b>(i,j) = color; 
    }
  }
return RGB_image;
}
