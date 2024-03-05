#ifndef __CAL_EFF_H
#define __CAL_EFF_H

#include "stdint.h"
/*
    (1-boost_duty)*vout
    -------------------
       buck_duty*vin

    boost_duty---升压下管占空比
    buck_duty----降压上管占空比
*/
typedef struct
{
    float buck_duty;
    float boost_duty;
    float vin;
    float vout;
} cal_eff_input_t;

typedef struct
{
    float eff;
} cal_eff_output_t;

typedef struct
{
    cal_eff_input_t input;
    cal_eff_output_t output;
} cal_eff_param_t;

void cal_eff_func(cal_eff_param_t *str);

#endif
