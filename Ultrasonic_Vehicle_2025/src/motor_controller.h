// MotorController.h - ESP32版本
#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#include <Arduino.h>
#include <driver/ledc.h>  // ESP32 LEDC PWM库
#include "Config.h"

class MotorController {
private:
    // 电机引脚
    int leftAPin, leftBPin;
    int rightAPin, rightBPin;
    
    // PWM通道
    int leftAChannel, leftBChannel;
    int rightAChannel, rightBChannel;
    
    // 当前状态
    int currentLeftSpeed;
    int currentRightSpeed;
    bool leftDirection;  // true=前进, false=后退
    bool rightDirection;
    
    // PWM配置
    int pwmFrequency;
    int pwmResolution;
    
public:
    // 构造函数
    MotorController();
    
    // 析构函数
    ~MotorController();
    
    // 初始化电机
    void init();
    
    // 设置PWM参数（可选）
    void setPWMConfig(int freq = PWM_FREQUENCY, int resolution = PWM_RESOLUTION);
    
    // 基本运动控制
    void forward(int speed);
    void backward(int speed);
    void turnLeft(int speed);
    void turnRight(int speed);
    void stop();
    
    // 差速转向
    void differentialControl(int baseSpeed, int turnOffset);
    
    // 独立控制左右轮
    void setLeftMotor(int speed, bool forward = true);
    void setRightMotor(int speed, bool forward = true);
    
    // 精确速度控制
    void setLeftSpeed(int speed);  // -255 to 255
    void setRightSpeed(int speed); // -255 to 255
    
    // 获取状态
    int getLeftSpeed() { return currentLeftSpeed; }
    int getRightSpeed() { return currentRightSpeed; }
    bool getLeftDirection() { return leftDirection; }
    bool getRightDirection() { return rightDirection; }
    
    // 停止所有电机并释放PWM通道
    void shutdown();
    
private:
    // 限制速度范围
    int constrainSpeed(int speed);
    
    // 设置单个PWM输出
    void setPWMOutput(int channel, int pin, int duty);
    
    // 电机刹车功能
    void brakeLeftMotor();
    void brakeRightMotor();
    void brakeAllMotors();
};

#endif