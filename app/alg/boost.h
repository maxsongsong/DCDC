#ifndef __BOOST_H
#define __BOOST_H

#include "pid.h"
#include "stdint.h"

typedef struct
{
    float vin;
    float iin;
    float vout;
    float iout;
    float i_l;
} boost_input_t;

typedef struct
{
    float vref;
    float l_val;
    float ctrl_ts;
} boost_cfg_t;

typedef struct
{
    pid_param_t curr_pid;
    pid_param_t volt_pid;
    boost_cfg_t cfg;
} boost_inter_t;

typedef struct
{
    float duty;
    uint8_t dcm_mode;
} boost_output_t;

typedef struct
{
    boost_input_t input;
    boost_inter_t inter;
    boost_output_t output;
} boost_param_t;

void boost_reset(boost_param_t *str);
void boost_set_volt_pid(boost_param_t *str, pid_cfg_t *pid_cfg);
void boost_set_curr_pid(boost_param_t *str, pid_cfg_t *pid_cfg);
void boost_set_cfg(boost_param_t *str, boost_cfg_t *cfg);
void boost_alg(boost_param_t *str);

#endif
