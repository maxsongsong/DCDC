#ifndef __ADC_H
#define __ADC_H

typedef struct
{
    float buck_i_in;
    float buck_i_out;
    float buck_i_l;
    float buck_v_in;
    float buck_v_out;
    float v_bat;
    float i_bat;
    float boost_v_in;
    float boost_i_in;
    float boost_v_out;
    float boost_i_out;
    float boost_i_l;
}adc_param_t;

void adc_set_buck_i_in(float val);
void adc_set_buck_i_out(float val);
void adc_set_buck_i_l(float val);
void adc_set_buck_v_in(float val);
void adc_set_buck_v_out(float val);
void adc_set_v_bat(float val);
void adc_set_i_bat(float val);
void adc_set_boost_v_in(float val);
void adc_set_boost_i_in(float val);
void adc_set_boost_v_out(float val);
void adc_set_boost_i_out(float val);
void adc_set_boost_i_l(float val);

float adc_get_buck_i_in(void);
float adc_get_buck_i_out(void);
float adc_get_buck_i_l(void);
float adc_get_buck_v_in(void);
float adc_get_buck_v_out(void);
float adc_get_v_bat(void);
float adc_get_i_bat(void);
float adc_get_boost_v_in(void);
float adc_get_boost_i_in(void);
float adc_get_boost_v_out(void);
float adc_get_boost_i_out(void);
float adc_get_boost_i_l(void);

#endif
