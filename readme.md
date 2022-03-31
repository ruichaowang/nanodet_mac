# NCNN—nanoDet——Mac
可以顺利移植 android 的程序到 mac 上（非 vulkan 版本），里面包括了图片和视频的测试 example 

## 介绍：
此 Project 为的目的为学习 NCNN 所使用的， 验证移植到 Mac（arm） 的使用可能性

## 程序逻辑
编译环境，加载模型，读取图像，读取结果，绘制结果

## 使用说明
0 请先用安装 android studio 体验一下原生的 Project 之后在看此 Project，
- 原始链接为： https://github.com/nihui/ncnn-android-nanodet
- 经测试，GPU 并不会提升帧率，仅仅是降低了CPU 的使用从 40% 到 20%。 

当前代码已经验证OK，可以使用 CPU，
但是 GPU 版本还没有编译成功，在build 的时候会提示 “ld: can't map file, errno=22 file '/Users/wangruichao/VulkanSDK/1.3.204.1/MoltenVK/dylib' for architecture arm64”

1.1 环境需要自己搭建 Opencv，OpenMP，Vulkan，ncnn
可以看 ncnn 的 faq
https://github.com/Tencent/ncnn/wiki/faq

1.2 加载模型参数
为了图省事，加载 model 中 的 .bin 和 .param，我选择了直接在添加了我的系统路径，
如果是运行时提示 "fopen nanodet-m-416.param failed", "fopen nanodet-m-416.bin failed"， 那就是说明这里有问题
```
char parampath[256];
char modelpath[256];
sprintf(parampath, "/Users/wangruichao/Work/NCNN/nanodet_mac/model/nanodet-%s.param", modeltype);
sprintf(modelpath, "/Users/wangruichao/Work/NCNN/nanodet_mac/model/nanodet-%s.bin", modeltype);
```
2 加载模型 （Easy）。。。懒得解释。。。。


## 与原始Project 的区别
- nanodet.h 中的 NanoDet::load共有两个版本，一个是纯 C ，一个是带 AAssetManager 的，我们舍弃掉后者
- nanodetncnn.cpp 中的  extern "C" 部分是 JNI 部分，需要看懂 JNI 是什么，然后修改 加载模型参数的函数，即 “Java_com_tencent_nanodetncnn_NanoDetNcnn_loadModel” 
- nanodetncnn.cpp 中的加载图像应该用不到 camera， 可以舍弃不要，而我们直接用 Opencv 手动送入图片；重点关注 MyNdkCamera::on_image_render 的  g_nanodet->detect & g_nanodet->draw， 而  draw_unsupported ， draw_fps 舍弃不用
- nanodet_ncnn_functions.hpp 主要解释了代码修改的部分以及其背后的原理。
 

## Build &  install 相关
若选择用 brew 安装 brew instal ncnn，他应该是自动安装到了 /opt/homebrew/Cellar/ncnn/20220216
OpenCV， 我的确是编译不成功，直接用 brew 安装的。

### ncnn build without vulcan
```
cd <ncnn-root-dir> 
mkdir -p build 
cd build 

cmake -DCMAKE_OSX_ARCHITECTURES="arm64" -DNCNN_BUILD_EXAMPLES=OFF ..
cmake --build . -j8
cmake --build . --target install
```
它生成的文档就在 install 中，我会复制到 3rd party 中，然后用cmake 链接就好。

## Debug
- 如果说官方给的example 运行不了，很有可能是因为 Gradle 版本 3.5 版本有些函数不支持了，所以直接update 到 7.1.2， 
- grade 的版本说明： https://developer.android.com/studio/releases/gradle-plugin-roadmap

- Lock， 要用 ncnn::Mutex lock， 要不然会出现找不到真正要加载的lock （通俗易懂的话解释就是，重名了）
- The submodules were not downloaded! Please update submodules with "git",    那就直接安装 git submodule update --init --recursive

## 更新说明
通俗易懂优化。
