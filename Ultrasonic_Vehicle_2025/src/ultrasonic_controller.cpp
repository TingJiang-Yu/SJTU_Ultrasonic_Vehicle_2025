#include "ultrasonic_controller.h"
#include "config.h"
#include <math.h>

// 静态成员初始化
ultrasonic_controller* ultrasonic_controller::instance = nullptr;
ultrasonic_controller ultrasonic;

// 中断服务函数
void IRAM_ATTR ultrasonic_controller::leftRiseISR() 
{
    if (instance) 
    {
        instance->leftRiseTime = micros();
        instance->leftDetected = true;
        
        // 如果右侧也已检测到，则处理测量
        if (instance->rightDetected) 
        {
            instance->processMeasurement();
        }
    }
}

void IRAM_ATTR ultrasonic_controller::rightRiseISR() 
{
    if (instance) 
    {
        instance->rightRiseTime = micros();
        instance->rightDetected = true;
        
        // 如果左侧也已检测到，则处理测量
        if (instance->leftDetected) 
        {
            instance->processMeasurement();
        }
    }
}

// ================= 构造函数 =================
ultrasonic_controller::ultrasonic_controller() 
    : leftRiseTime(0), rightRiseTime(0),
      leftDetected(false), rightDetected(false),
      measurementReady(false), historyIndex(0),
      filteredTimeDiff(0), timeOffset(0),
      lastMeasurementTime(0), isCalibrated(false) 
{
    
    // 清空历史缓冲区
    for (int i = 0; i < HISTORY_SIZE; i++) timeDiffHistory[i] = 0;
    
    // 设置静态实例指针
    instance = this;
}

// ================= 初始化 =================
void ultrasonic_controller::init() 
{
    // 设置引脚模式
    pinMode(ULTRASONIC_RX_LEFT_PIN, INPUT);
    pinMode(ULTRASONIC_RX_RIGHT_PIN, INPUT);
    
    // 配置外部中断
    attachInterrupt(digitalPinToInterrupt(ULTRASONIC_RX_LEFT_PIN), 
                   leftRiseISR, RISING);
    attachInterrupt(digitalPinToInterrupt(ULTRASONIC_RX_RIGHT_PIN), 
                   rightRiseISR, RISING);
    
    // 初始化定时器
    initTimer();
    
    // 校准
    calibrate();
}

// ================= 定时器初始化 =================
void ultrasonic_controller::initTimer() {
    // 使用ESP32的硬件定时器（可选，用于超时检测）
    // 这里暂时使用软件超时
}

// ================= 开始测量 =================
void ultrasonic_controller::startMeasurement() 
{
    resetMeasurement();
}

// ================= 更新测量 =================
void ultrasonic_controller::update() 
{
    uint32_t currentTime = millis();
    
    // 检查超时（超过50ms没有新测量）
    if (measurementReady) 
    {
        // 测量已准备好，可以读取
        lastMeasurementTime = currentTime;
    } 
    else if (currentTime - lastMeasurementTime > 50) 
    {
        // 超时，重置测量
        resetMeasurement();
    }
}

// ================= 获取时间差 =================
int32_t ultrasonic_controller::getTimeDifference() 
{
    return filteredTimeDiff;
}

// ================= 获取方向角度 =================
float ultrasonic_controller::getDirectionAngle() 
{
    return calculateAngleFromTimeDiff(filteredTimeDiff);
}

// ================= 获取方向偏差 =================
float ultrasonic_controller::getDirectionBias() 
{
    float angle = getDirectionAngle();
    
    // 将角度归一化到 -1.0 到 1.0
    float bias = angle / ANGLE_MAX;
    
    // 限制范围
    if (bias > 1.0) bias = 1.0;
    if (bias < -1.0) bias = -1.0;
    
    // 应用死区
    if (fabs(bias) < 0.05) bias = 0.0;
    
    return bias;
}

// ================= 检查有效信号 =================
bool ultrasonic_controller::hasValidSignal() {
    if (!measurementReady) return false;
    
    // 检查时间差是否合理
    int32_t absTimeDiff = abs(filteredTimeDiff);
    return (absTimeDiff < MAX_TIME_DIFFERENCE) && (absTimeDiff > 0);
}

// ================= 校准 =================
void ultrasonic_controller::calibrate() 
{
    // 清空历史缓冲区
    for (int i = 0; i < HISTORY_SIZE; i++) timeDiffHistory[i] = 0;
    
    filteredTimeDiff = 0;
    timeOffset = 0;
    
    // 等待几次测量来校准
    delay(1000);
    
    // 假设现在信号源在正前方，计算偏移
    timeOffset = filteredTimeDiff;  // 正前方的理论时间差应为0
    
    isCalibrated = true;
}

// ================= 处理测量数据 =================
void ultrasonic_controller::processMeasurement() 
{
    if (!leftDetected || !rightDetected) return;
    
    // 计算时间差（考虑硬件延迟差）
    int32_t rawTimeDiff = (int32_t)(leftRiseTime - rightRiseTime) - timeOffset;
    
    // 滤波处理
    filteredTimeDiff = applyFilter(rawTimeDiff);
    
    // 标记测量完成
    measurementReady = true;
    
    // 重置检测标志（准备下一次测量）
    resetMeasurement();
}

// ================= 重置测量 =================
void ultrasonic_controller::resetMeasurement() 
{
    leftDetected = false;
    rightDetected = false;
    measurementReady = false;
}

// ================= 应用滤波器 =================
int32_t ultrasonic_controller::applyFilter(int32_t newValue) 
{
    // 更新历史缓冲区
    timeDiffHistory[historyIndex] = newValue;
    historyIndex = (historyIndex + 1) % HISTORY_SIZE;
    
    // 计算移动平均值（排除异常值）
    int32_t sum = 0;
    int count = 0;
    
    // 先找出中值
    int32_t values[HISTORY_SIZE];
    for (int i = 0; i < HISTORY_SIZE; i++) 
    {
        values[i] = timeDiffHistory[i];
    }
    
    // 简单排序找中值
    for (int i = 0; i < HISTORY_SIZE - 1; i++) 
    {
        for (int j = i + 1; j < HISTORY_SIZE; j++) 
        {
            if (values[i] > values[j]) {
                int32_t temp = values[i];
                values[i] = values[j];
                values[j] = temp;
            }
        }
    }
    int32_t median = values[HISTORY_SIZE / 2];
    
    // 使用中值附近的值计算平均
    for (int i = 0; i < HISTORY_SIZE; i++) 
    {
        if (abs(timeDiffHistory[i] - median) < 100) 
        { // 100μs范围内
            sum += timeDiffHistory[i];
            count++;
        }
    }
    
    return (count > 0) ? (sum / count) : newValue;
}

// ================= 从时间差计算角度 =================
float ultrasonic_controller::calculateAngleFromTimeDiff(int32_t timeDiff) 
{
    // 几何关系：sin(θ) = (v * Δt) / d
    // 其中：v = 声速，Δt = 时间差，d = 接收器间距
    
    float dt_us = (float)timeDiff;  // 时间差（μs）
    float d = RECEIVER_SPACING;     // 接收器间距（cm）
    float v = SPEED_OF_SOUND;       // 声速（cm/μs）
    
    // 计算sin(θ)
    float sin_theta = (v * dt_us) / d;
    
    // 限制范围（防止计算错误）
    if (sin_theta > 1.0) sin_theta = 1.0;
    if (sin_theta < -1.0) sin_theta = -1.0;
    
    // 计算角度（弧度转度）
    float angle_rad = asin(sin_theta);
    float angle_deg = angle_rad * 180.0 / M_PI;
    
    return angle_deg;
}