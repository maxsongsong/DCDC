#ifndef __PID_H
#define __PID_H

#include "string.h"

typedef struct
{
    float kp;
    float ki;
    float kd;
    float i_err_lmt_max;
    float i_err_lmt_min;
    float output_lmt_max;
    float output_lmt_min;
} pid_cfg_t;

typedef struct
{
    float ref;
    float act;
} pid_input_t;

typedef struct
{
    float output;
} pid_output_t;

typedef struct
{
    float err;
    float err_last;
    float err_diff;
    float i_err;
} pid_inter_t;

typedef struct
{
    pid_cfg_t   cfg;
    pid_inter_t inter;
    pid_input_t input;
    pid_output_t output;
} pid_param_t;

void pid_reset(pid_param_t *pid_param);
void pid_set_ref(pid_param_t *pid_param, float ref);
void pid_set_act(pid_param_t *pid_param, float act);
float pid_cal(pid_param_t *pid_param);
void PidComputationalClear(pid_param_t *pid_param);

#endif
