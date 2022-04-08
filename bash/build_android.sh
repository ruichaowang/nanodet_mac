#清除已经生成的文件
if [ ! -d build_android ]; then
    mkdir -p build_android
fi
cd build_android
rm -rf *

# 生成
cd ..
export ANDROID_NDK="/Users/wangruichao/Library/Android/sdk/ndk/24.0.8215888"
cmake -S . -B build_android -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake \
      -DANDROID_ABI="arm64-v8a" \
        -DANDROID_PLATFORM=android-24 \
          -DNCNN_OPENMP=ON
cmake --build build_android -j16 