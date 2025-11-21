# 超声波跟随小车2025

## 使用方法：

使用`vscode`打开`Ultrasonic_Vehicle_2025`文件夹，在`src`文件夹中，可以看到所有的文件。

目前搭建了一个基础的框架，但还要根据实际情况更改。

大体的思路：
- `main_controller`作为上层控制，直接调用底层库函数，采集底层的信息，处理信息并产生命令，有点类似于`int main()`函数。
  - `motor_controller`作为电机控制板底层，负责驱动电机（如果有现成的库，就不用这个了）。
  - `pid_controller`作为$PID$算法库，如果需要就用。
  - `ultrasonic_controller`作为超声波模块底层，负责驱动超声波模块。

## tips:

提交代码的时候，在`vscode`中右键点击`Ultrasonic_Vehicle_2025`文件夹，选择“在集成终端中打开”选项。

在终端输入
```bash
git add .
git commit -m "anything you want to say!"
```

然后快捷键`Ctrl+Shift+G`打开源代码管理，点击提交，这里如果报错就关掉梯子，然后重新提交就可以看到`github`上面更新了你的代码。

如果关闭梯子也无法正常提交代码，尝试添加SSH秘钥，详见`About SSH.md`。或者你也可以**关闭SJTU网，然后连接手机流量热点提交。我感觉大概率是校园网限制的问题**。烦死了，早知道用Gitee了……