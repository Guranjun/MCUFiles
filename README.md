v1.0 裸机程序
基于野火指南者开发板ov7725摄像头QVAG图像读取  引进tiny_jpeg压缩图像数据
经实验,从串口发送被压缩后的图像数据,解码后图像压缩率在4:1左右
从摄像头模块读取的rgb数据转成rgb888 320*240*3=225kb 压缩后
<img width="1013" height="37" alt="image" src="https://github.com/user-attachments/assets/04f7fc50-356f-426e-af4a-5a45dd3ed742" />
能提高图像读取帧率.
v3.0 freertos+lvgl
在v2.0的基础上添加lvgl库，于gui_guider软件进行ui设计并移植进工程。
现有功能：主页面，display与upload两个按键，点击dispaly可进入实时摄像头图像显示，点击upload可以上传当前帧。
<img width="610" height="798" alt="image" src="https://github.com/user-attachments/assets/70dde8c0-87eb-498a-bb55-42502575e6d9" />
          子页面，实时摄像头显示页面，lcd屏幕下端有两个按钮为shot与exti，点击shot可以上传屏幕显示的当前帧，点击exti可以退出子页面回到主页面。
          <img width="946" height="647" alt="image" src="https://github.com/user-attachments/assets/f58fd81b-946c-41d2-94e5-fbe07dbc31e7" />
