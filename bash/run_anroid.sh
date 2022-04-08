# 这个比较麻烦就是要把软件推到安卓上运行.
# mkdir /data/chao
# mkdir /data/chao/model

adb push ./data/test_img.jpg /data/chao
adb push ./build_android/linux_ncnn_nanodet /data/chao
adb push ./model /data/chao

adb shell "chmod +x /data/chao/linux_ncnn_nanodet"
adb shell "cd /data/chao \
         && export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH \
         && ./linux_ncnn_nanodet ./model ./test_img.jpg ./test_img.png "
adb pull "/data/chao/test_img.png" ~/Desktop
