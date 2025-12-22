# 超声波跟随小车2025

## 使用方法：

首先，你需要安装好`PlatformIO`插件，`PlatformIO`插件的**安装及使用方法详见文末**。如果你希望使用 $LaTex$ 撰写实验报告，你还需要安装`TexLive`编译环境和`LaTeX Workshop`插件。MCU、电机控制板、超声波模块、电池模块等硬件材料老师会给准备好的。

将这个项目克隆到本地，在文件夹中双击 `open.bat` 文件，会自动打开 `vscode` 并加载项目，注意路径中最好不要含有中文字符。

打开 `vscode` 后（假设你已经启用`PlatformIO`插件），你应该会看到右下角有一个小房子图标，点击进入PlatformIO的主界面，用`PlatformIO`打开项目（如果没有这个选项，可能你需要查一下怎么打开已有项目），然后等待初始化完成。

接下来你就可以写代码了，如果有什么不会的，或者代码看不懂的地方就问问AI吧~

当然，也欢迎你在我的邮箱`tingjiang@sjtu.edu.cn`留言。

## 项目介绍

### 电路设计：

将调制正弦波转换为 $40kHz$ 的脉冲信号

### 程序结构：

- `main.cpp` 主程序入口
- `main_controller.h/.cpp` 作为上层控制器，负责集成底层信号并控制小车运行
- `motor_controller.h/.cpp` 作为电机控制板底层，负责驱动电机基础运动
- `pid_controller.h/.cpp` $PID$算法库，用来控制电机的转向
- `ultrasonic_controller.h/.cpp` 作为超声波模块底层，负责驱动超声波接收器模块，对信号进行处理
- `config.h` 用来存储所有的引脚定义，直接调用引脚定义，不要用数字，不然后期改起来很麻烦

## 实时进度

### 已完成的内容

- 超声波发生器T的组装
- 电机控制代码
- 超声波发生器T的驱动代码
- 超声波接收器R的驱动代码
- 整体控制算法
- 电池模块的组装

### On going

- 超声波接收器R的滤波电路
- 实验报告撰写
- 车体搭建

### 待完成的内容

- 超声波收发测试
- 整体测试

## tips:

### 指导书网站

https://www.kdocs.cn/l/cjcQNsTwbACK

### 关于代码的上传

**每次修改代码之前，先在源代码管理中点击“拉取”，同步最新进度**。

![alt text](Markdown_Image\image.png)

提交代码的时候，在`vscode`中右键点击`Ultrasonic_Vehicle_2025`文件夹，选择“在集成终端中打开”选项。

在终端输入

```bash
git add .
git commit -m "anything you want to say!"
```

然后快捷键`Ctrl+Shift+G`打开源代码管理，点击提交。如果无法正常提交代码，可以尝试添加SSH秘钥，具体方式请询问AI。或者你也可以**关闭SJTU网，然后连接手机流量热点提交。有可能是校园网限制的问题**。

如果你的小伙伴不擅长科学上网，你也可以考虑使用国内的Gitee进行小组合作。

### 关于 Platform IO 的使用

详见 https://blog.csdn.net/wcc243588569/article/details/139234425。