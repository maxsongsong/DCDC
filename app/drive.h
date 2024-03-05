#ifndef __DRIVE_H
#define __DRIVE_H

#include "bsp_adc.h"
#include "bsp_pwm.h"

BSP_ADC_DEC(I_IN)
BSP_ADC_DEC(V_IN)
BSP_ADC_DEC(V_OUT)
BSP_ADC_DEC(I_OUT)
BSP_ADC_DEC(I_L)
BSP_ADC_DEC(I_L1)

BSP_PWM_DEC(BUCK)
BSP_PWM_DEC(BUCK1)
BSP_PWM_DEC(BOOST)
BSP_PWM_DEC(BOOST1)

#endif
