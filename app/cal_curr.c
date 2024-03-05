#include "cal_curr.h"

void cal_curr_func(cal_curr_t *str)
{
    str->output.i_in = str->input.i_l * str->input.buck_duty;
    str->output.i_out = str->input.i_l * (1.0f - str->input.boost_duty);
}
