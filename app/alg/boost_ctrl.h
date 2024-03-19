#ifndef __BOOST_CTRL_H
#define __BOOST_CTRL_H
#include "pid.h"

#define TRUE 1
#define FALSE 0
typedef struct
{
    pid_param_t voltLoop;
    pid_param_t currLoop;
    pid_param_t volt_limt_up_loop;
    pid_param_t volt_limt_dn_loop;

    bool softStatus;
    float voltLoopResult;
    float currLoopResult;
    float vin;
    float vout;
    float rampCnt;
    float outVoltRef;
    float outVoltRampRef;
    float outVoltRealRef;
    float outVolt;
    float outVoltFiler;
    float vinVoltRef;
    float vinVolt;
    float vinVoltFiler;
    float currRef;
    float currFiler;
    float curr;
    float feedforwardVolt;
    float feedforwardCoefficient;
    float vpwm;

    float curr_trig_limt_up;
    float curr_trig_limt_dn;

    float duty;

} BoostCtrl_t;

void BoostCtrlInit();
void BoostController(void);
#endif