# 超声波跟随小车2025

## 使用方法：

使用`vscode`打开`Ultrasonic_Vehicle_2025`文件夹，在`src`文件夹中，可以看到所有的文件。

或者直接在文件夹中双击 `open.bat` 文件，会自动打开 `vscode` 并加载项目。

## 项目介绍

- `main.cpp` 主程序入口
- `main_controller.h/.cpp` 作为上层控制器，负责集成底层信号并控制小车运行
- `motor_controller.h/.cpp` 作为电机控制板底层，负责驱动电机基础运动
- `pid_controller.h/.cpp` $PID$算法库，用来控制电机的转向
- `ultrasonic_controller.h/.cpp` 作为超声波模块底层，负责驱动超声波接收器模块，对信号进行处理
- `config.h` 用来存储所有的引脚定义，直接调用引脚定义，不要用数字，不然后期改起来很麻烦

## 实时进度

### 已完成的内容

- 电机控制代码
- 超声波发生器T的组装
- 超声波发生器T的驱动代码
- 超声波接收器R的驱动代码
- 整体控制算法

### 待完成的内容

- 超声波接收器R的滤波电路
- 超声波收发测试
- 车体搭建
- 整体测试
- 实验报告

## tips:

### 关于代码的上传

**每次修改代码之前，先在源代码管理中点击“拉取”，同步最新进度**。

![alt text](Markdown_Image\image.png)

提交代码的时候，在`vscode`中右键点击`Ultrasonic_Vehicle_2025`文件夹，选择“在集成终端中打开”选项。

在终端输入

```bash
git add .
git commit -m "anything you want to say!"
```

然后快捷键`Ctrl+Shift+G`打开源代码管理，点击提交，这里如果报错就关掉梯子，然后重新提交就可以看到`github`上面更新了你的代码。

如果关闭梯子也无法正常提交代码，尝试添加SSH秘钥，详见`About SSH.md`。或者你也可以**关闭SJTU网，然后连接手机流量热点提交。我感觉大概率是校园网限制的问题**。烦死了，早知道用Gitee了……

### 关于 Platform IO 的使用

详见 `Platformio.md` 文件，也可以看老师发的金山文档里的`csdn`网站。