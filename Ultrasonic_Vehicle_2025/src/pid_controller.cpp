/*===================
* pid_controller.cpp
=====================*/

#include "pid_controller.h"

pid_controller::pid_controller()
{
    this->obs = new pid_data();
}

bool pid_controller::init(float* data_d, float* data_o)
{
    this->data_d = data_d;
    this->data_o = data_o;

    // 初始化误差和系数
    obs->e[0] = 0.0f;
    obs->e[1] = 0.0f;
    obs->e[2] = 0.0f;

    obs->k[0] = 0.0f; // kp
    obs->k[1] = 0.0f; // ki
    obs->k[2] = 0.0f; // kd

    obs->l[0] = 0.0f; // 积分限幅
    obs->l[1] = 0.0f; // 输出最小
    obs->l[2] = 0.0f; // 输出最大

    return true;
}

bool pid_controller::Calc(float data_t)
{
    error = *data_d - data_t;      // 计算误差
    obs->e[2] = error - obs->e[0]; // 离散微分
    obs->e[1] += error;            // 积分
    obs->e[0] = error;             // 更新误差

    // 积分限幅
    if (obs->e[1] > obs->l[0])
        obs->e[1] = obs->l[0];
    else if (obs->e[1] < -obs->l[0])
        obs->e[1] = -obs->l[0];
    
    // 临时数据容器，计算PID原始值
    data_ = obs->k[0] * obs->e[0] + 
            obs->k[1] * obs->e[1] + 
            obs->k[2] * obs->e[2];
    
    // 输出限幅
    if (data_ > obs->l[2])
        *data_o = obs->l[2];
    else if (data_ < obs->l[1])
        *data_o = obs->l[1];
    else
        *data_o = data_;

    return true;
}

bool pid_controller::SetK(float kp, float ki, float kd)
{
    this->obs->k[0] = kp;
    this->obs->k[1] = ki;
    this->obs->k[2] = kd;

    return true;
}

bool pid_controller::SetL(float l_int, float l_min, float l_max)
{
    this->obs->l[0] = l_int;
    this->obs->l[1] = l_min;
    this->obs->l[2] = l_max;

    return true;
}