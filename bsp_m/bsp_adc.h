#ifndef __BSP_ADC_H
#define __BSP ADC_H

#define BSP_ADC_FUNC(name)               \
    static float name;                   \
    void bsp_adc_set_##name(float value) \
    {                                    \
        name = value;                    \
    }                                    \
                                         \
    float bsp_adc_get_##name(void)       \
    {                                    \
        return name;                     \
    }

#define BSP_ADC_DEC(name)                 \
    void bsp_adc_set_##name(float value); \
    float bsp_adc_get_##name(void);

#define BSP_ADC_SET(name, value) bsp_adc_set_##name(value);
#define BSP_ADC_GET(name) bsp_adc_get_##name();

#endif
