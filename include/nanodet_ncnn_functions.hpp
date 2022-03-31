#ifndef nanodet_ncnn_functions
#define nanodet_ncnn_functions

/*
此文件的作用是为了分离开 src 里的文件，src 是不对外的，是封装好的文件，而


所以在main.cpp 中，它只需要有三行就可以使用：
loadModel(0,0);         // 加载模型，当前为第一个 m，即编号0，不用 gpu，
MyNdkCamera sampleimg;  // 随便创建一个叫做 sampleimg 的  MyNdkCamera 
sampleimg.on_image_render(frame);       // 模型运行，推理 & render
*/


#include "../src/nanodet.h" 
#include "../include/common_function.hpp"

static NanoDet* g_nanodet = 0;
static ncnn::Mutex lock;


/**
  * @brief 检测 & 画图。
  * @param[in] rgb - 我们在原图上进行render
  * @exception 是否会因为没有指针而导致失败？
  * @author ruichao
  * @date 2022 0330 0950
  * @version 1.0
  * @property 基于 MyNdkCamera
  **/
class MyNdkCamera
{
  public:
    virtual void on_image_render(cv::Mat& rgb) const;
};

void MyNdkCamera::on_image_render(cv::Mat& rgb) const
{
  // nanodet
  ncnn::MutexLockGuard g(ncnn::Mutex lock);

  if (g_nanodet)
  {
      // std::cout<<"nanodet 开始运行！" <<std::endl;   
      // 我们关心的是 detect 的时间
      Timer timer; 
      timer.tic();  
      std::vector<Object> objects;
      g_nanodet->detect(rgb, objects);
      timer.toc("ncnn nanodet, Detect ");
      g_nanodet->draw(rgb, objects);
      
  }
}



/**
  * @brief initialize detect object engine ，我猜测，这部分的目的肯定是为了根据 modelid 而选取model 载入。
  * @param[in] modelid - 选择我们需要的模型
  * @param[in] cpugpu - 选择是否用CPU GPU
  * @return 0: success, <0: failed
  * @exception 是否会因为没有指针而导致失败？
  * @author ruichao
  * @date 2022 0329 2026
  * @version 1.0
  * @property 此部分是基于 Java_com_tencent_nanodetncnn_NanoDetNcnn_loadModel 修改的，删除了大量没有用的参数。
  **/
int loadModel(int modelid, int cpugpu)
{
  const char* modeltypes[] =
  {  "m", "m-416", "g", "ELite0_320", "ELite1_416", "ELite2_512","RepVGG-A0_416" };

  const int target_sizes[] =
  { 320, 416, 416, 320, 416, 512, 416 };

  const float mean_vals[][3] =
  {
      {103.53f, 116.28f, 123.675f},
      {103.53f, 116.28f, 123.675f},
      {103.53f, 116.28f, 123.675f},
      {127.f, 127.f, 127.f},
      {127.f, 127.f, 127.f},
      {127.f, 127.f, 127.f},
      {103.53f, 116.28f, 123.675f}
  };

  const float norm_vals[][3] =
  {
      {1.f / 57.375f, 1.f / 57.12f, 1.f / 58.395f},
      {1.f / 57.375f, 1.f / 57.12f, 1.f / 58.395f},
      {1.f / 57.375f, 1.f / 57.12f, 1.f / 58.395f},
      {1.f / 128.f, 1.f / 128.f, 1.f / 128.f},
      {1.f / 128.f, 1.f / 128.f, 1.f / 128.f},
      {1.f / 128.f, 1.f / 128.f, 1.f / 128.f},
      {1.f / 57.375f, 1.f / 57.12f, 1.f / 58.395f}
  };

  // 
  // TODO 1 确定 lock 调用正确，
  // TODO 2 先默认不用GPU，但保留了接口，测试 GPU
  ncnn::MutexLockGuard g(ncnn::Mutex lock); 
  const char* modeltype = modeltypes[modelid];
  int target_size = target_sizes[modelid];
  bool use_gpu = cpugpu;

  if (use_gpu && ncnn::get_gpu_count() == 0)
  {
      // no gpu
      printf("找不到 GPU\n ");
      delete g_nanodet;
      g_nanodet = 0;
  }
  else
  {
      if (!g_nanodet)
          g_nanodet = new NanoDet;
      // printf("找到 GPU， 开始 Load \n ");
      g_nanodet->load(modeltype, target_size, mean_vals[modelid], norm_vals[modelid], use_gpu);
      // printf("找 GPU，Load 成功，当前炸掉 \n ");
  }

  return 0;
}


#endif 