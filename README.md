v1.0 裸机程序
基于野火指南者开发板ov7725摄像头QVAG图像读取  引进tiny_jpeg压缩图像数据
经实验,从串口发送被压缩后的图像数据,解码后图像压缩率在4:1左右
从摄像头模块读取的rgb数据转成rgb888 320*240*3=225kb 压缩后
<img width="1013" height="37" alt="image" src="https://github.com/user-attachments/assets/04f7fc50-356f-426e-af4a-5a45dd3ed742" />
能提高图像读取帧率.
vv