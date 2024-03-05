#include "cal_eff.h"

void cal_eff_func(cal_eff_param_t *str)
{
    str->output.eff = ((1.0f - str->input.boost_duty) *
                       str->input.vout) /
                      (str->input.buck_duty *
                       str->input.vin);
}
