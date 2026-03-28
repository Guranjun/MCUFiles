v1.0 裸机程序
基于野火指南者开发板ov7725摄像头QVAG图像读取  引进tiny_jpeg压缩图像数据
经实验,从串口发送被压缩后的图像数据,解码后图像压缩率在4:1左右
从摄像头模块读取的rgb数据转成rgb888 320*240*3=225kb 压缩后
<img width="1013" height="37" alt="image" src="https://github.com/user-attachments/assets/04f7fc50-356f-426e-af4a-5a45dd3ed742" />
能提高图像读取帧率.
v2.0 freertos程序
在原先裸机程序的基础上,修改代码引入freertos
引入python写了一个脚本识别串口信息,并截取其中的jpeg数据段并生成对应的.jpg图像
<img width="325" height="125" alt="image" src="https://github.com/user-attachments/assets/9cf2edff-f440-4d33-ab22-a9d7ea26b486" />


整个工程是建立在野火指南者开发板的源码基础上的，主要依靠于配套模块程序中的摄像头模块的液晶实时显示、屏幕模块的配套完整程序

除此之外移植freertos，lvgl库，tinyjpeg

个人贡献部分由rtos中的任务划分和对应任务的具体实现、tinyjpeg的修改、lvgl按键的事件处理逻辑

关于tinyjpeg的改动为，新增分块处理函数，做这个改动的原因是一帧图像的数据量太大了，指南者的ram只有64kb甚至还没有一张图片大，所以必须分块，但是tinyjpeg的源码里提供的是整个图像的压缩处理，所以新增了分块处理。
