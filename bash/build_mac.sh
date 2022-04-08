#清除已经生成的文件
if [ ! -d build_mac ]; then
    mkdir -p build_mac
fi
cd build_mac
rm -rf *


# 生成
cd ..
cmake -S . -B build_mac -DCMAKE_OSX_ARCHITECTURES="arm64" 
cmake --build build_mac -j16 
# VERBOSE=1 暂时关闭 Build 的细节,调成了16 为了增加速度， -DNCNN_OPENMP=ON -DTARGET_OS=macOS