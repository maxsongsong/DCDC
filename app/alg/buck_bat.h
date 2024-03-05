#ifndef __BUCK_BAT_H
#define __BUCK_BAT_H

#include "pid.h"
#include "stdint.h"

#define BUCK_BAT_VIN_FLT_ECO 0.01f

typedef struct
{
    float vin;
    float iin;
    float vout;
    float iout;
    float i_l;
    float iref;
} buck_bat_input_t;

typedef struct
{
    float vref_up;
    float vref_dn;
    float l_val;
    float ctrl_ts;
} buck_bat_cfg_t;

typedef struct
{
    pid_param_t curr_pid;
    pid_param_t volt_up_pid;
    pid_param_t volt_dn_pid;
    pid_param_t vin_lmt_pid;
    buck_bat_cfg_t cfg;
    float vin_lmt_ref;
    float vin_flt;
} buck_bat_inter_t;

typedef struct
{
    float duty;
    uint8_t dcm_mode;
    float dcm_curr;
} buck_bat_output_t;

typedef struct
{
    float curr_loop_out;
    float volt_loop_up_out;
    float volt_loop_dn_out;
    float vin_lmt_loop_out;
    float iref;
} back_bat_obs_t;

typedef struct
{
    buck_bat_input_t input;
    buck_bat_inter_t inter;
    buck_bat_output_t output;
    back_bat_obs_t obs;
} buck_bat_param_t;

void buck_bat_reset(buck_bat_param_t *str);
void buck_bat_set_volt_up_pid(buck_bat_param_t *str, pid_cfg_t *pid_cfg);
void buck_bat_set_volt_dn_pid(buck_bat_param_t *str, pid_cfg_t *pid_cfg);
void buck_bat_set_vin_lmt_pid(buck_bat_param_t *str, pid_cfg_t *pid_cfg);
void buck_bat_set_curr_pid(buck_bat_param_t *str, pid_cfg_t *pid_cfg);
void buck_bat_set_cfg(buck_bat_param_t *str, buck_bat_cfg_t *cfg);
void buck_bat_alg(buck_bat_param_t *str);
float buck_bat_get_duty(void);

#endif
