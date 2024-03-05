/* Copyright 2003-2014 The MathWorks, Inc. */

// *******************************************************************
// **** To build this mex function use: mex sfun_cppcount_cpp.cpp ****
// *******************************************************************

#include "bsp_m/bsp_irq.h"
#include "bsp_m/bsp_init.h"
#include "bsp_m/bsp_task.h"
#include "CtrlAPP.h"
#include "misc.h"
#include "drive.h"

#define S_FUNCTION_LEVEL 2
#define S_FUNCTION_NAME CtrlAPP

// Need to include simstruc.h for the definition of the SimStruct and
// its associated macro definitions.
#include "simstruc.h"
#include "CtrlAPP_port.h"

#define IS_PARAM_DOUBLE(pVal) (mxIsNumeric(pVal) && !mxIsLogical(pVal) && \
                               !mxIsEmpty(pVal) && !mxIsSparse(pVal) && !mxIsComplex(pVal) && mxIsDouble(pVal))

// Function: mdlInitializeSizes ===============================================
// Abstract:
//    The sizes information is used by Simulink to determine the S-function
//    block's characteristics (number of inputs, outputs, states, etc.).
static void mdlInitializeSizes(SimStruct *S)
{
    // No expected parameters
    ssSetNumSFcnParams(S, 0);

    // Parameter mismatch will be reported by Simulink
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S))
    {
        return;
    }

    // Specify I/O
    if (!ssSetNumInputPorts(S, INPUT_PORT_NUM))
        return;

    ssSetInputPortWidth(S, 0, INPUT_PORT_WIDTH);

    ssSetInputPortDirectFeedThrough(S, 0, 1);
    if (!ssSetNumOutputPorts(S, OUTPUT_PORT_NUM))
        return;

    ssSetOutputPortWidth(S, 0, OUTPUT_PORT_WIDTH);
    ssSetOutputPortWidth(S, 1, OUTPUT_PORT_WIDTH);
    ssSetOutputPortWidth(S, 2, OUTPUT_PORT_WIDTH);
    ssSetOutputPortWidth(S, 3, OUTPUT_PORT_WIDTH);
    ssSetOutputPortWidth(S, 4, OUTPUT_PORT_WIDTH);
    ssSetOutputPortWidth(S, 5, OUTPUT_PORT_WIDTH);
    ssSetOutputPortWidth(S, 6, OUTPUT_PORT_WIDTH);
    ssSetOutputPortWidth(S, 7, OUTPUT_PORT_WIDTH);
    ssSetOutputPortWidth(S, 8, OUTPUT_PORT_WIDTH);
    ssSetOutputPortWidth(S, 9, OUTPUT_PORT_WIDTH);
    ssSetOutputPortWidth(S, 10, OUTPUT_PORT_WIDTH);

    ssSetNumSampleTimes(S, 1);

    // Reserve place for C++ object
    ssSetNumPWork(S, 1);

    ssSetOperatingPointCompliance(S, USE_CUSTOM_OPERATING_POINT);

    /* Set this S-function as runtime thread-safe for multicore execution */
    ssSetRuntimeThreadSafetyCompliance(S, RUNTIME_THREAD_SAFETY_COMPLIANCE_TRUE);

    ssSetOptions(S, SS_OPTION_WORKS_WITH_CODE_REUSE |
                        SS_OPTION_EXCEPTION_FREE_CODE |
                        SS_OPTION_DISALLOW_CONSTANT_SAMPLE_TIME);
}

// Function: mdlInitializeSampleTimes =========================================
// Abstract:
//   This function is used to specify the sample time(s) for your
//   S-function. You must register the same number of sample times as
//   specified in ssSetNumSampleTimes.
static void mdlInitializeSampleTimes(SimStruct *S)
{
    ssSetSampleTime(S, 0, 1 / 120e6);
    ssSetOffsetTime(S, 0, 0.0);
    ssSetModelReferenceSampleTimeDefaultInheritance(S);
}

// Function: mdlStart =======================================================
// Abstract:
//   This function is called once at start of model execution. If you
//   have states that should be initialized once, this is the place
//   to do it.
#define MDL_START
static void mdlStart(SimStruct *S)
{
}

// Function: mdlOutputs =======================================================
// Abstract:
//   In this function, you compute the outputs of your S-function
//   block.
static void mdlOutputs(SimStruct *S, int_T tid)
{
    static uint32_t task_timer_cnt = 0;
    static uint32_t sys_cnt = 0;
    static uint8_t init_flg = 0;
    if (init_flg == 0)
    {
        init_flg = 1;
        bsp_task_init();
        bsp_init_func();
    }

    /*输入接口开始*/
    INPUT_PTR(S);
    /*输入接口结束*/

    /*中断源条件开始*/
    if (BSP_PWM_GET_TOP_UPDATE(BUCK)) // 希望在峰值进行采样和PWM计算。
    /*中断源条件开始*/
    {

        /*获取ADC开始*/
        BSP_ADC_SET(I_IN, *u[1]);
        BSP_ADC_SET(V_IN, *u[2]);
        BSP_ADC_SET(V_OUT, *u[3]);
        BSP_ADC_SET(I_OUT, *u[4]);
        BSP_ADC_SET(I_L, *u[5]);
        BSP_ADC_SET(I_L1, *u[6]);
        /*获取ADC结束*/

        /*中断函数开始*/
        bsp_irq_func();
        /*中断函数结束*/

        /*DEBUG开始*/
        ssPrintf("buck_param.input.i_l:%f\n", buck_param.input.i_l);
        ssPrintf("buck_param.input.iin:%f\n", buck_param.input.iin);
        ssPrintf("buck_param.input.iout:%f\n", buck_param.input.iout);
        ssPrintf("buck_param.input.vin:%f\n", buck_param.input.vin);
        ssPrintf("buck_param.input.vout:%f\n", buck_param.input.vout);
        ssPrintf("buck_param.output.duty:%f\n", buck_param.output.duty);
        ssPrintf("***********************************\n");
        /*DEBUG结束*/
    }

    /*PWM输出开始*/
    BSP_PWM_RUN(BUCK);
    BSP_PWM_RUN(BUCK1);

    BSP_PWM_RUN(BOOST);
    BSP_PWM_RUN(BOOST1);
    /*PWM输出结束*/

    /*输出接口开始*/
    OUTPUT_PTR(S);

    pwm[0] = BSP_PWM_GET_CH(BUCK);
    pwm[1] = BSP_PWM_GET_CH_N(BUCK);
    pwm[2] = BSP_PWM_GET_CH(BUCK1);
    pwm[3] = BSP_PWM_GET_CH_N(BUCK1);

    y3[0] = BSP_PWM_GET_CH(BOOST);
    y3[1] = BSP_PWM_GET_CH_N(BOOST);
    y3[2] = BSP_PWM_GET_CH(BOOST1);
    y3[3] = BSP_PWM_GET_CH_N(BOOST1);

    y1[0] = buck_param.inter.volt_pid.input.act;
    y1[1] = buck_param.inter.volt_pid.input.ref;
    y1[2] = buck_param.inter.volt_pid.output.output;
    y1[3] = buck_param.inter.volt_pid.inter.i_err;

    y2[0] = buck_param.inter.curr_pid.input.act;
    y2[1] = buck_param.inter.curr_pid.input.ref;
    y2[2] = buck_param.inter.curr_pid.output.output;
    y2[3] = buck_param.inter.curr_pid.inter.i_err;

    y4[0] = buck_param.output.dcm_mode;
    y4[1] = buck_param.inter.dcm_curr;
    y4[2] = buck_param.output.duty;
    y4[3] = buck_param.output.duty1;

    y5[0] = buck_param.inter.curr_loop_out;
    y5[1] = buck_param.inter.l_volt_fw;

    y6[0] = buck_param.inter.volt_lmt_pid.input.act;
    y6[1] = buck_param.inter.volt_lmt_pid.input.ref;
    y6[2] = buck_param.inter.volt_lmt_pid.output.output;
    y6[3] = buck_param.inter.volt_lmt_pid.inter.i_err;
    /*输出接口结束*/

    /*任务函数开始*/
    sys_cnt++;
    if (sys_cnt >= (0.0001f /*秒*/ / ssGetSampleTime(S, 0))) // 100us执行一次
    {
        sys_cnt = 0;
        task_timer_cnt++;
    }
    bsp_task_run(task_timer_cnt);
    /*任务函数结束*/
}

// Function: mdlTerminate =====================================================
// Abstract:
//   In this function, you should perform any actions that are necessary
//   at the termination of a simulation.  For example, if memory was
//   allocated in mdlStart, this is the place to free it.
static void mdlTerminate(SimStruct *S)
{
}

// Required S-function trailer
#ifdef MATLAB_MEX_FILE /* Is this file being compiled as a MEX-file? */
#include "simulink.c"  /* MEX-file interface mechanism */
#else
#include "cg_sfun.h" /* Code generation registration function */
#endif
