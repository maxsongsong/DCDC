#ifndef __CAL_CURR_H
#define __CAL_CURR_H

#include "stdint.h"

typedef struct
{
    float i_l;
    float buck_duty;
    float boost_duty;
} cal_curr_input_t;

typedef struct
{
    float i_in;
    float i_out;
} cal_curr_output_t;

typedef struct
{
    cal_curr_input_t input;
    cal_curr_output_t output;
} cal_curr_t;

void cal_curr_func(cal_curr_t *str);

#endif
