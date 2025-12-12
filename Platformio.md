# Platform IO 使用

`Platform IO` 插件平时似乎会与cpp插件冲突，所以建议平时禁用，需要用到时再启用。

挂梯子，打开 `Platform IO` 插件。这时你会看到左侧有一个小蚂蚁的头像，点进去。

![alt text](Markdown_Image\image_2.png)

然后在 PROJECT TASKS 一栏选择 `Pick a folder` ，打开 `Ultrasonic_Vehicle_2025` 文件夹， **注意，是有`.pio`的那个文件夹**。

然后，等待 `Platfrom IO` 自动初始化，完成后在VScode左下角就能看到一个 $\rightarrow$ 标志，这就是烧录按钮。写好代码之后，将`main.cpp`烧录进芯片。

![alt text](Markdown_Image\image_3.png)