#ifndef __BUCK_CURR_H
#define __BUCK_CURR_H

#include "pid.h"
#include "stdint.h"

typedef struct
{
    float vin;
    float iin;
    float vout;
    float iout;
    float i_l;
    float iref;
} buck_curr_input_t;

typedef struct
{
    float l_val;
    float ctrl_ts;
} buck_curr_cfg_t;

typedef struct
{
    pid_param_t curr_pid;
    pid_param_t volt_pid;
    buck_curr_cfg_t cfg;
} buck_curr_inter_t;

typedef struct
{
    float duty;
    uint8_t dcm_mode;
    float dcm_curr;
} buck_curr_output_t;

typedef struct
{
    buck_curr_input_t input;
    buck_curr_inter_t inter;
    buck_curr_output_t output;
} buck_curr_param_t;

void buck_curr_reset(buck_curr_param_t *str);
void buck_curr_set_curr_pid(buck_curr_param_t *str, pid_cfg_t *pid_cfg);
void buck_curr_set_cfg(buck_curr_param_t *str, buck_curr_cfg_t *cfg);
void buck_curr_alg(buck_curr_param_t *str);

#endif
