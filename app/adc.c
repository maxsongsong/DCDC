#include "adc.h"
#include "bsp_adc.h"

adc_param_t adc_param;

void adc_set_buck_i_in(float val)
{
    adc_param.buck_i_in = val;
}
void adc_set_buck_i_out(float val)
{
    adc_param.buck_i_out = val;
}
void adc_set_buck_i_l(float val)
{
    adc_param.buck_i_l = val;
}
void adc_set_buck_v_in(float val)
{
    adc_param.buck_v_in = val;
}
void adc_set_buck_v_out(float val)
{
    adc_param.buck_v_out = val;
}
void adc_set_v_bat(float val)
{
    adc_param.v_bat = val;
}
void adc_set_i_bat(float val)
{
    adc_param.i_bat = val;
}
void adc_set_boost_v_in(float val)
{
    adc_param.boost_v_in = val;
}
void adc_set_boost_i_in(float val)
{
    adc_param.boost_i_in = val;
}
void adc_set_boost_v_out(float val)
{
    adc_param.boost_v_out = val;
}
void adc_set_boost_i_out(float val)
{
    adc_param.boost_i_out = val;
}
void adc_set_boost_i_l(float val)
{
    adc_param.boost_i_l = val;
}

float adc_get_buck_i_in(void)
{
    return adc_param.buck_i_in;
}
float adc_get_buck_i_out(void)
{
    return adc_param.buck_i_out;
}
float adc_get_buck_i_l(void)
{
    return adc_param.buck_i_l;
}
float adc_get_buck_v_in(void)
{
    return adc_param.buck_v_in;
}
float adc_get_buck_v_out(void)
{
    return adc_param.buck_v_out;
}
float adc_get_v_bat(void)
{
    return adc_param.v_bat;
}
float adc_get_i_bat(void)
{
    return adc_param.i_bat;
}
float adc_get_boost_v_in(void)
{
    return adc_param.boost_v_in;
}
float adc_get_boost_i_in(void)
{
    return adc_param.boost_i_in;
}
float adc_get_boost_v_out(void)
{
    return adc_param.boost_v_out;
}
float adc_get_boost_i_out(void)
{
    return adc_param.boost_i_out;
}
float adc_get_boost_i_l(void)
{
    return adc_param.boost_i_l;
}
