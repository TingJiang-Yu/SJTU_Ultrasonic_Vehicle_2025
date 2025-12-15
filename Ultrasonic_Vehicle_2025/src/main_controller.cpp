#include "main_controller.h"

main_controller::main_controller() 
    : currentState(STATE_SEARCH),
      lastStateChangeTime(0),
      targetDistance(FOLLOW_TARGET_DISTANCE),
      directionPid(nullptr),
      targetDirection(0.0),
      currentDirection(0.0),
      pidOutput(0.0)
    {}

// ================= 初始化 =================
void main_controller::init() 
{
    Serial.begin(SERIAL_BAUDRATE);
    
    sonic.init();
    motor.init();
    sonic.startMeasurement();
    initPID();
    
    currentState = STATE_SEARCH;
    lastStateChangeTime = millis();
}

// ================= PID初始化 =================
void main_controller::initPID()
{
    directionPid = new pid_controller(&targetDirection, &pidOutput);
    
    directionPid->SetK(PID_KP, PID_KI, PID_KD);
    directionPid->SetL(100.0, -BASE_SPEED, BASE_SPEED);
}

// ================= 主控制循环 =================
void main_controller::update() 
{
    // 更新超声波传感器数据
    sonic.updateADC();
    
    // 根据状态执行相应控制
    switch (currentState) 
    {
    case STATE_SEARCH:
        handleSearchState();
        break;
    case STATE_FOLLOW:
        handleFollowState();
        break;
    case STATE_STOP:
        handleStopState();
        break;
    }
}

// ================= 搜索状态 =================
void main_controller::handleSearchState() 
{
    // 检查是否有信号
    if (sonic.hasValidSignal()) 
    {
        setState(STATE_FOLLOW);
        return;
    }
    
    // 没有信号，原地旋转搜索
    searchRotation();
}

// ================= 跟随状态（使用PID控制） =================
void main_controller::handleFollowState() 
{
    if (!sonic.hasValidSignal()) 
    {
        setState(STATE_SEARCH);
        return;
    }
    
    // 获取当前方向偏差
    // directionBias: -1.0（完全偏左）到 1.0（完全偏右），0表示在中间
    currentDirection = sonic.getDirectionBias();
    
    // 使用PID计算控制输出
    // PID目标：让 currentDirection 接近 0（信号在中间）
    directionPid->Calc(currentDirection);
    
    // pidOutput 现在包含PID控制器的输出值
    // 正值：需要右转（减小正偏差或增大负偏差）
    // 负值：需要左转（减小负偏差或增大正偏差）
    
    // 根据PID输出调整左右轮速度
    // 如果 pidOutput > 0，需要右转：左轮加速，右轮减速
    // 如果 pidOutput < 0，需要左转：右轮加速，左轮减速
    int leftSpeed = BASE_SPEED - pidOutput;
    int rightSpeed = BASE_SPEED + pidOutput;
    
    // 限制速度范围
    leftSpeed = constrain(leftSpeed, -MAX_SPEED, MAX_SPEED);
    rightSpeed = constrain(rightSpeed, -MAX_SPEED, MAX_SPEED);
    
    // 如果方向偏差很小，可以稍微降低速度
    if (fabs(currentDirection) < 0.05) 
    {
        leftSpeed = BASE_SPEED;
        rightSpeed = BASE_SPEED;
    }
    
    // 执行移动
    moveCar(leftSpeed, rightSpeed);
}

// ================= 停止状态 =================
void main_controller::handleStopState() 
{
    motor.stop();
}

// ================= 状态切换 =================
void main_controller::setState(ControlState newState) 
{
    if (currentState == newState) return;

    motor.stop();
    
    currentState = newState;
    lastStateChangeTime = millis();
}

// ================= 搜索旋转 =================
void main_controller::searchRotation() 
{
    moveCar(BASE_SPEED / 2, -BASE_SPEED / 2);
}

// ================= 移动控制 =================
void main_controller::moveCar(int leftSpeed, int rightSpeed) 
{
    motor.setLeftSpeed(leftSpeed);
    motor.setRightSpeed(rightSpeed);
}