#ifndef _CTRLAPP_PORT_CPP_
#define _CTRLAPP_PORT_CPP_

#include "stdint.h"
#define S_FUNCTION_LEVEL 2

#include "simstruc.h"
#include "CtrlAPP.h"

#define INPUT_PORT_NUM 1
#define INPUT_PORT_WIDTH 10

#define OUTPUT_PORT_NUM 11
#define OUTPUT_PORT_WIDTH 4
#define ADC_INPUT_NUM 6

#define OUTPUT_PTR(S)                             \
   real_T *pwm = ssGetOutputPortRealSignal(S, 0); \
   real_T *y1 = ssGetOutputPortRealSignal(S, 1);  \
   real_T *y2 = ssGetOutputPortRealSignal(S, 2);  \
   real_T *y3 = ssGetOutputPortRealSignal(S, 3);  \
   real_T *y4 = ssGetOutputPortRealSignal(S, 4);  \
   real_T *y5 = ssGetOutputPortRealSignal(S, 5);  \
   real_T *y6 = ssGetOutputPortRealSignal(S, 6);  \
   real_T *y7 = ssGetOutputPortRealSignal(S, 7);  \
   real_T *y8 = ssGetOutputPortRealSignal(S, 8);  \
   real_T *y9 = ssGetOutputPortRealSignal(S, 9);  \
   real_T *y10 = ssGetOutputPortRealSignal(S, 10);

#define INPUT_PTR(S)                                         \
   InputRealPtrsType u = ssGetInputPortRealSignalPtrs(S, 0); 

typedef struct
{
   float PWM_carrier_act;
   float PWM_carrier_bark;
   float PWM_carrier_bark2;

} Ctrlapp_t;

void mdl_ctrl_out(SimStruct *S);
void mdl_ctl_sampling_input(SimStruct *S);

#endif
