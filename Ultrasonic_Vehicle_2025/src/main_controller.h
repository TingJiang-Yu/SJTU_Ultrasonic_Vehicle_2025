#ifndef MAIN_CONTROLLER_H
#define MAIN_CONTROLLER_H

#include <Arduino.h>
#include "config.h"
#include "motor_controller.h"
#include "ultrasonic_controller.h"
#include "pid_controller.h"

class main_controller
{
private:
    motor_controller motor;
    ultrasonic_controller sonic;
    pid_controller* directionPid;  // 方向PID控制器
    
    // 控制状态
    enum ControlState 
    {
        STATE_SEARCH,    // 搜索信号
        STATE_FOLLOW,    // 跟随信号
        STATE_STOP       // 停止
    };
    
    ControlState currentState;
    uint32_t lastStateChangeTime;
    float targetDistance;
    
    // PID控制变量
    float targetDirection;      // 目标方向（应该为0，表示信号在中间）
    float currentDirection;     // 当前方向
    float pidOutput;            // PID输出
    
public:
    main_controller();
    
    // 初始化控制器
    void init();
    
    // 主控制循环
    void update();
    
    // 状态切换
    void setState(ControlState newState);
    
    // 获取当前状态
    ControlState getState() { return currentState; }
    
private:
    // 各个状态的处理函数
    void handleSearchState();
    void handleFollowState();
    void handleStopState();
    
    // 控制小车行动
    void moveCar(int leftSpeed, int rightSpeed);
    void searchRotation();
    
    // PID初始化
    void initPID();
};

#endif