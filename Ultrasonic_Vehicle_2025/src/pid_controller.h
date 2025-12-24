/*===================
* pid_controller.h
=====================*/

#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

typedef struct pid_data_ pid_data;
struct pid_data_
{
    float e[3]; // 误差：比例误差、积分误差、微分误差
    float k[3]; // 系数：比例系数、积分系数、微分系数
    float l[3]; // 限幅：积分限幅、输出最小、输出最大
};

class pid_controller
{
private:
    pid_data* obs; // PID数据集成
    float error;   // 误差值
    float data_;   // 原始PID输出
    float* data_o; // 输出数据
    float* data_d; // 预期数据
    
public:
    /// @brief 构造函数
    pid_controller();

    /// @brief 初始化PID控制器
    /// @param data_d 期望的数据（比如小车的位置）
    /// @param data_o 实际输出的数据指令（比如小车电机的扭矩）
    bool init(float* data_d, float* data_o);

    /// @brief PID计算函数，通过当前获取的数据计算应该输出的数据，并存储在指针中
    /// @param data_t 当前传感器获取的数据，对应data_d，PID需要让data_t不断接近data_d
    bool Calc(float data_t);

    /// @brief 设置PID控制的系数
    /// @param kp 比例系数
    /// @param ki 积分系数
    /// @param kd 微分系数
    bool SetK(float kp, float ki, float kd);

    /// @brief 设置限幅
    /// @param l_int 积分限幅
    /// @param l_min 输出最小限幅
    /// @param l_max 输出最大限幅
    bool SetL(float l_int, float l_min, float l_max);
};

#endif
