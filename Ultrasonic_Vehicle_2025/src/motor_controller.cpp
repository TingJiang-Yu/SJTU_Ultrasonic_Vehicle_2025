// MotorController.cpp - ESP32版本
#include "motor_controller.h"

// ================= 构造函数和析构函数 =================
MotorController::MotorController() 
    : leftAPin(MOTOR_LEFT_A_PIN), 
      leftBPin(MOTOR_LEFT_B_PIN),
      rightAPin(MOTOR_RIGHT_A_PIN), 
      rightBPin(MOTOR_RIGHT_B_PIN),
      leftAChannel(PWM_CHANNEL_LEFT_A),
      leftBChannel(PWM_CHANNEL_LEFT_B),
      rightAChannel(PWM_CHANNEL_RIGHT_A),
      rightBChannel(PWM_CHANNEL_RIGHT_B),
      currentLeftSpeed(0), 
      currentRightSpeed(0),
      leftDirection(true),
      rightDirection(true),
      pwmFrequency(PWM_FREQUENCY),
      pwmResolution(PWM_RESOLUTION) {
}

MotorController::~MotorController() {
    shutdown();
}

// ================= 初始化函数 =================
void MotorController::init() {
    Serial.println("Initializing Motor Controller...");
    
    // 设置PWM配置
    setPWMConfig(pwmFrequency, pwmResolution);
    
    // 设置引脚模式
    pinMode(leftAPin, OUTPUT);
    pinMode(leftBPin, OUTPUT);
    pinMode(rightAPin, OUTPUT);
    pinMode(rightBPin, OUTPUT);
    
    // 初始化PWM为0
    setPWMOutput(leftAChannel, leftAPin, 0);
    setPWMOutput(leftBChannel, leftBPin, 0);
    setPWMOutput(rightAChannel, rightAPin, 0);
    setPWMOutput(rightBChannel, rightBPin, 0);
    
    // 初始停止
    stop();
    
    Serial.println("Motor Controller Initialized");
    Serial.print("PWM Frequency: "); Serial.print(pwmFrequency); Serial.println(" Hz");
    Serial.print("PWM Resolution: "); Serial.print(pwmResolution); Serial.println(" bits");
}

void MotorController::setPWMConfig(int freq, int resolution) {
    pwmFrequency = freq;
    pwmResolution = resolution;
    
    // 配置LEDC定时器
    ledcSetup(leftAChannel, pwmFrequency, pwmResolution);
    ledcSetup(leftBChannel, pwmFrequency, pwmResolution);
    ledcSetup(rightAChannel, pwmFrequency, pwmResolution);
    ledcSetup(rightBChannel, pwmFrequency, pwmResolution);
    
    // 关联引脚和通道
    ledcAttachPin(leftAPin, leftAChannel);
    ledcAttachPin(leftBPin, leftBChannel);
    ledcAttachPin(rightAPin, rightAChannel);
    ledcAttachPin(rightBPin, rightBChannel);
}

// ================= 基本运动控制 =================
int MotorController::constrainSpeed(int speed) {
    if (speed > MAX_SPEED) return MAX_SPEED;
    if (speed < MIN_SPEED) return MIN_SPEED;
    return speed;
}

void MotorController::setPWMOutput(int channel, int pin, int duty) {
    duty = constrain(duty, 0, (1 << pwmResolution) - 1);
    ledcWrite(channel, duty);
}

void MotorController::forward(int speed) {
    speed = constrainSpeed(speed);
    
    // 左轮：A=PWM, B=0
    setPWMOutput(leftAChannel, leftAPin, speed);
    setPWMOutput(leftBChannel, leftBPin, 0);
    
    // 右轮：A=PWM, B=0
    setPWMOutput(rightAChannel, rightAPin, speed);
    setPWMOutput(rightBChannel, rightBPin, 0);
    
    currentLeftSpeed = speed;
    currentRightSpeed = speed;
    leftDirection = true;
    rightDirection = true;
    
    #ifdef DEBUG_ENABLED
    Serial.print("Forward: "); Serial.println(speed);
    #endif
}

void MotorController::backward(int speed) {
    speed = constrainSpeed(speed);
    
    // 左轮：A=0, B=PWM
    setPWMOutput(leftAChannel, leftAPin, 0);
    setPWMOutput(leftBChannel, leftBPin, speed);
    
    // 右轮：A=0, B=PWM
    setPWMOutput(rightAChannel, rightAPin, 0);
    setPWMOutput(rightBChannel, rightBPin, speed);
    
    currentLeftSpeed = speed;
    currentRightSpeed = speed;
    leftDirection = false;
    rightDirection = false;
    
    #ifdef DEBUG_ENABLED
    Serial.print("Backward: "); Serial.println(speed);
    #endif
}

void MotorController::turnLeft(int speed) {
    speed = constrainSpeed(speed);
    int turnSpeed = speed * 0.7;  // 内侧轮速度稍慢
    
    // 左轮后退（内侧）
    setPWMOutput(leftAChannel, leftAPin, 0);
    setPWMOutput(leftBChannel, leftBPin, turnSpeed);
    
    // 右轮前进（外侧）
    setPWMOutput(rightAChannel, rightAPin, speed);
    setPWMOutput(rightBChannel, rightBPin, 0);
    
    currentLeftSpeed = -turnSpeed;
    currentRightSpeed = speed;
    leftDirection = false;
    rightDirection = true;
    
    #ifdef DEBUG_ENABLED
    Serial.print("Turn Left: L="); 
    Serial.print(turnSpeed); 
    Serial.print(", R="); 
    Serial.println(speed);
    #endif
}

void MotorController::turnRight(int speed) {
    speed = constrainSpeed(speed);
    int turnSpeed = speed * 0.7;  // 内侧轮速度稍慢
    
    // 左轮前进（外侧）
    setPWMOutput(leftAChannel, leftAPin, speed);
    setPWMOutput(leftBChannel, leftBPin, 0);
    
    // 右轮后退（内侧）
    setPWMOutput(rightAChannel, rightAPin, 0);
    setPWMOutput(rightBChannel, rightBPin, turnSpeed);
    
    currentLeftSpeed = speed;
    currentRightSpeed = -turnSpeed;
    leftDirection = true;
    rightDirection = false;
    
    #ifdef DEBUG_ENABLED
    Serial.print("Turn Right: L="); 
    Serial.print(speed); 
    Serial.print(", R="); 
    Serial.println(turnSpeed);
    #endif
}

void MotorController::stop() {
    brakeAllMotors();
    currentLeftSpeed = 0;
    currentRightSpeed = 0;
    
    #ifdef DEBUG_ENABLED
    Serial.println("Stop");
    #endif
}

// ================= 差速控制 =================
void MotorController::differentialControl(int baseSpeed, int turnOffset) {
    baseSpeed = constrainSpeed(baseSpeed);
    
    // 计算左右轮速度
    int leftSpeed = baseSpeed + turnOffset;
    int rightSpeed = baseSpeed - turnOffset;
    
    // 限制速度范围
    leftSpeed = constrain(leftSpeed, -MAX_SPEED, MAX_SPEED);
    rightSpeed = constrain(rightSpeed, -MAX_SPEED, MAX_SPEED);
    
    // 分别设置左右轮
    setLeftSpeed(leftSpeed);
    setRightSpeed(rightSpeed);
    
    #ifdef DEBUG_ENABLED
    if (millis() % 500 == 0) {  // 每500ms打印一次
        Serial.print("Diff Control: L="); 
        Serial.print(leftSpeed); 
        Serial.print(", R="); 
        Serial.print(rightSpeed);
        Serial.print(", Offset="); 
        Serial.println(turnOffset);
    }
    #endif
}

// ================= 独立控制 =================
void MotorController::setLeftMotor(int speed, bool forward) {
    speed = constrainSpeed(speed);
    
    if (forward) {
        setPWMOutput(leftAChannel, leftAPin, speed);
        setPWMOutput(leftBChannel, leftBPin, 0);
        leftDirection = true;
        currentLeftSpeed = speed;
    } else {
        setPWMOutput(leftAChannel, leftAPin, 0);
        setPWMOutput(leftBChannel, leftBPin, speed);
        leftDirection = false;
        currentLeftSpeed = -speed;
    }
}

void MotorController::setRightMotor(int speed, bool forward) {
    speed = constrainSpeed(speed);
    
    if (forward) {
        setPWMOutput(rightAChannel, rightAPin, speed);
        setPWMOutput(rightBChannel, rightBPin, 0);
        rightDirection = true;
        currentRightSpeed = speed;
    } else {
        setPWMOutput(rightAChannel, rightAPin, 0);
        setPWMOutput(rightBChannel, rightBPin, speed);
        rightDirection = false;
        currentRightSpeed = -speed;
    }
}

void MotorController::setLeftSpeed(int speed) {
    speed = constrain(speed, -MAX_SPEED, MAX_SPEED);
    
    if (speed > 0) {
        // 前进
        setPWMOutput(leftAChannel, leftAPin, speed);
        setPWMOutput(leftBChannel, leftBPin, 0);
        leftDirection = true;
        currentLeftSpeed = speed;
    } else if (speed < 0) {
        // 后退
        setPWMOutput(leftAChannel, leftAPin, 0);
        setPWMOutput(leftBChannel, leftBPin, -speed);
        leftDirection = false;
        currentLeftSpeed = speed;
    } else {
        // 停止
        setPWMOutput(leftAChannel, leftAPin, 0);
        setPWMOutput(leftBChannel, leftBPin, 0);
        currentLeftSpeed = 0;
    }
}

void MotorController::setRightSpeed(int speed) {
    speed = constrain(speed, -MAX_SPEED, MAX_SPEED);
    
    if (speed > 0) {
        // 前进
        setPWMOutput(rightAChannel, rightAPin, speed);
        setPWMOutput(rightBChannel, rightBPin, 0);
        rightDirection = true;
        currentRightSpeed = speed;
    } else if (speed < 0) {
        // 后退
        setPWMOutput(rightAChannel, rightAPin, 0);
        setPWMOutput(rightBChannel, rightBPin, -speed);
        rightDirection = false;
        currentRightSpeed = speed;
    } else {
        // 停止
        setPWMOutput(rightAChannel, rightAPin, 0);
        setPWMOutput(rightBChannel, rightBPin, 0);
        currentRightSpeed = 0;
    }
}

// ================= 刹车功能 =================
void MotorController::brakeLeftMotor() {
    // 短接电机两端实现快速刹车
    setPWMOutput(leftAChannel, leftAPin, MAX_SPEED);
    setPWMOutput(leftBChannel, leftBPin, MAX_SPEED);
    delay(50);  // 短时刹车
    setPWMOutput(leftAChannel, leftAPin, 0);
    setPWMOutput(leftBChannel, leftBPin, 0);
}

void MotorController::brakeRightMotor() {
    setPWMOutput(rightAChannel, rightAPin, MAX_SPEED);
    setPWMOutput(rightBChannel, rightBPin, MAX_SPEED);
    delay(50);
    setPWMOutput(rightAChannel, rightAPin, 0);
    setPWMOutput(rightBChannel, rightBPin, 0);
}

void MotorController::brakeAllMotors() {
    setPWMOutput(leftAChannel, leftAPin, MAX_SPEED);
    setPWMOutput(leftBChannel, leftBPin, MAX_SPEED);
    setPWMOutput(rightAChannel, rightAPin, MAX_SPEED);
    setPWMOutput(rightBChannel, rightBPin, MAX_SPEED);
    delay(50);
    setPWMOutput(leftAChannel, leftAPin, 0);
    setPWMOutput(leftBChannel, leftBPin, 0);
    setPWMOutput(rightAChannel, rightAPin, 0);
    setPWMOutput(rightBChannel, rightBPin, 0);
}

// ================= 系统关闭 =================
void MotorController::shutdown() {
    stop();
    
    // 分离PWM通道
    ledcDetachPin(leftAPin);
    ledcDetachPin(leftBPin);
    ledcDetachPin(rightAPin);
    ledcDetachPin(rightBPin);
    
    Serial.println("Motor Controller Shutdown");
}