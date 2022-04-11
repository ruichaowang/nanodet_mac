# ncnn nanodet macOS (ARM)

此 Project 为的目的为学习 NCNN 所使用的， 验证移植到 macOS (ARM) ,以及为 安卓 crosscompiling 的使用可能性, 当前验证可以顺利移植 android 的程序(with Vulkan)到 mac 上（非 vulkan 版本），里面包括了图片和视频的测试 example,

## Build & Run example
```
./bash/build_mac.sh
./build_mac/linux_ncnn_nanodet ./model ./data/test_img.jpg ./data/test_img.png

./bash/build_android.sh
./bash/run_anroid.sh

如果是用 VScode 那就在 .vscode 的 launch 里设置.
```

## 程序逻辑
编译环境，加载模型，读取图像，读取结果，绘制结果

## 使用说明
0 请先用安装 android studio 体验一下原生的 Project 之后在看此 Project，
- 原始链接为： https://github.com/nihui/ncnn-android-nanodet
- 经测试，GPU 并不会提升帧率，仅仅是降低了CPU 的使用从 40% 到 20%。 
- 但是 Armmac 的 GPU 版本还没有编译成功，在build 的时候会提示 “ld: can't map file, errno=22 file '/Users/wangruichao/VulkanSDK/1.3.204.1/MoltenVK/dylib' for architecture arm64”, 调用时也会出现报错提示.
- cmake 的指令:

1.1 环境需要自己搭建 Opencv，OpenMP，Vulkan，ncnn
可以看 ncnn 的 faq
https://github.com/Tencent/ncnn/wiki/faq

1.2 加载模型参数
0408 已经更新, 改为参数输入, 路径加载时为相对地址,并且只需要把参数输入到 arg 就可以了.

1.3 GPU
- android 的直接设置 use gpu = 1 就可以使用了,cmake 中已经编译; 
- mac ARM 以后再研究吧,不会进项目
- https://github.com/Tencent/ncnn/wiki/FAQ-ncnn-vulkan
- https://github.com/nihui/ncnn-android-squeezenet


2 加载模型 
有关 nanodet 的说明看这里:
- YOLO之外的另一选择，手机端97FPS的Anchor-Free目标检测模型NanoDet现已开源~ - RangiLyu的文章 - 知乎 https://zhuanlan.zhihu.com/p/306530300
- 大白话 Generalized Focal Loss - 李翔的文章 - 知乎 https://zhuanlan.zhihu.com/p/147691786
- 'gfocal是继EfficientNet，FCOS，ATSS，PolarMask，BorderDet，D/CIoU，YOLOv4以来又一次让我感觉值得精读的文章'
- 超简单辅助模块加速训练收敛，精度大幅提升！移动端实时的NanoDet升级版NanoDet-Plus来了！ - RangiLyu的文章 - 知乎 https://zhuanlan.zhihu.com/p/449912627



3 后处理


## 与原始Project 的区别
- 原始的project https://github.com/RangiLyu/nanodet
- nanodet.h 中的 NanoDet::load共有两个版本，一个是纯 C ，一个是带 AAssetManager 的，我们舍弃掉后者
- nanodetncnn.cpp 中的  extern "C" 部分是 JNI 部分，需要看懂 JNI 是什么，然后修改 加载模型参数的函数，即 “Java_com_tencent_nanodetncnn_NanoDetNcnn_loadModel” 
- nanodetncnn.cpp 中的加载图像应该用不到 camera， 可以舍弃不要，而我们直接用 Opencv 手动送入图片；重点关注 MyNdkCamera::on_image_render 的  g_nanodet->detect & g_nanodet->draw， 而  draw_unsupported ， draw_fps 舍弃不用
- nanodet_ncnn_functions.hpp 主要解释了代码修改的部分以及其背后的原理。
 

## Build &  install 相关
- 若选择用 brew 安装 brew instal ncnn，他应该是自动安装到了 /opt/homebrew/Cellar/ncnn/20220216
- OpenCV， 我在 mac ARM 上的确是编译不成功（Ubuntu、 x86 mac 啥的还是完全OK），直接用 brew 安装的。。。。。
- OpenCV，下载链接： https://github.com/opencv/opencv/releases/tag/4.5.5
- 虽说官方说明带 vulkan 能编译成功：https://github.com/Tencent/ncnn/wiki/how-to-build#build-for-macos ，但是我的确是各种失败。
- 官方说明如何编译 ncnn ： https://github.com/Tencent/ncnn/wiki/how-to-build#build-for-macos


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
这里有它官方的说明：
https://github.com/Tencent/ncnn/wiki/how-to-build#build-for-macos

## Debug
- 如果说官方给的example 运行不了，很有可能是因为 Gradle 版本 3.5 版本有些函数不支持了，所以直接update 到 7.1.2， 
- grade 的版本说明： https://developer.android.com/studio/releases/gradle-plugin-roadmap

- Lock， 要用 ncnn::Mutex lock， 要不然会出现找不到真正要加载的lock （通俗易懂的话解释就是，重名了）
- The submodules were not downloaded! Please update submodules with "git",    那就直接安装 git submodule update --init --recursive

## 更新说明
增加一些有的没的, 代码通俗易懂优化。
