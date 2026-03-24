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
