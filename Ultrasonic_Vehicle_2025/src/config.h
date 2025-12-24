/*===================
* config.h
=====================*/

#ifndef CONFIG_H
#define CONFIG_H

/************************************************************
 * 1. 调试配置
 ************************************************************/
#define SERIAL_BAUDRATE         115200

/************************************************************
 * 2. 超声波控制器参数
 ************************************************************/
#define ULTRASONIC_RX_LEFT_PIN   34
#define ULTRASONIC_RX_RIGHT_PIN  35

#define ULTRASONIC_MAX_TIME_DIFFERENCE     500  // us，异常时间差限幅
#define ULTRASONIC_TIMEDIFF_DEADZONE       10   // us，时间差死区
#define ULTRASONIC_SILENCE_TIME            18   // us，静默时间

/************************************************************
 * 3. 电机控制参数
 ************************************************************/
#define MOTOR_LEFT_PIN          4
#define MOTOR_RIGHT_PIN         17

#define MOTOR_MAX_SPEED         255     // PWM 最大值
#define MOTOR_BASE_SPEED        127     // 直行基础速度
#define MOTOR_MAX_TURN_SPEED    127     // 转弯速度

#define PWM_UPDATE_INTERVAL_MS  10      // 电机 PWM 更新间隔

/************************************************************
 * 4. 行为状态机参数
 ************************************************************/
#define SIGNAL_TIMEOUT_MS       100     // 判定信号丢失时间
#define SEARCH_TURN_SPEED       80      // 搜索时原地旋转速度

/************************************************************
 * 5. PID 控制参数（方向）
 ************************************************************/
#define PID_KP                  2.0
#define PID_KI                  0.01
#define PID_KD                  0.05

#define PID_MAX_INTEGRAL        1.0
#define PID_MIN_OUTPUT         -1.0
#define PID_MAX_OUTPUT          1.0

#define PID_DEAD_ZONE           0.002    // 输出死区

#define PID_DESIRED_DATA        0.0    // 目标时间差（正对声源）

/************************************************************
 * 6. 低通滤波参数
 ************************************************************/
#define LPF_ALPHA               0.6    // 低通滤波系数（0.0 - 1.0）

#endif // CONFIG_H
