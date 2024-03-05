#ifndef __BUCK_H
#define __BUCK_H

#include "pid.h"
#include "stdint.h"

typedef struct
{
    float vin;
    float iin;
    float vout;
    float iout;
    float i_l;
    float i_l1;
} buck_input_t;

typedef struct
{
    float vref;
    float l_val;
    float ctrl_ts;
} buck_cfg_t;

typedef struct
{
    pid_param_t curr_pid;
    pid_param_t curr_pid1;
    pid_param_t volt_pid;
    pid_param_t volt_lmt_pid;
    buck_cfg_t cfg;
    float iref_last;
    float dcm_curr;
    float l_volt_fw;
    float curr_loop_out;
} buck_inter_t;

typedef struct
{
    float duty;
    float duty1;
    uint8_t dcm_mode;
} buck_output_t;

typedef struct
{
    buck_input_t input;
    buck_inter_t inter;
    buck_output_t output;
} buck_param_t;

void buck_reset(buck_param_t *str);
void buck_set_volt_pid(buck_param_t *str, pid_cfg_t *pid_cfg);
void buck_set_volt_lmt_pid(buck_param_t *str, pid_cfg_t *pid_cfg);
void buck_set_curr_pid(buck_param_t *str, pid_cfg_t *pid_cfg);
void buck_set_curr_pid1(buck_param_t *str, pid_cfg_t *pid_cfg);
void buck_set_cfg(buck_param_t *str, buck_cfg_t *cfg);
void buck_alg(buck_param_t *str);

#endif
