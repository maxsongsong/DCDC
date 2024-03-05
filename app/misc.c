#include "buck.h"
#include "misc.h"
#include "cal_eff.h"
#include "buck_curr.h"
#include "boost.h"
#include "cal_curr.h"
#include "drive.h"

cal_eff_param_t buck_eff;
boost_param_t boost_param;
cal_curr_t cal_curr_buck;
cal_curr_t cal_curr_boost;
buck_param_t buck_param;

void ctrl_init(void)
{
    pid_cfg_t pid_cfg;
    boost_cfg_t boost_cfg;
    buck_cfg_t buck_cfg;

    buck_reset(&buck_param);

    buck_cfg.ctrl_ts = 20.0e-6f;
    buck_cfg.l_val = 10.0e-6f;
    buck_cfg.vref = 16.8f;
    buck_set_cfg(&buck_param, &buck_cfg);

    pid_cfg.kp = 8;
    pid_cfg.ki = 0.1f;
    pid_cfg.kd = 0.0f;
    pid_cfg.i_err_lmt_max = 115.0f;
    pid_cfg.i_err_lmt_min = -115.0f;
    pid_cfg.output_lmt_max = 115.0f;
    pid_cfg.output_lmt_min = -115.0f;
    buck_set_volt_pid(&buck_param, &pid_cfg);

    pid_cfg.kp = 0.01f;
    pid_cfg.ki = 0.001f;
    pid_cfg.kd = 0.0f;
    pid_cfg.i_err_lmt_max = 0.0f;
    pid_cfg.i_err_lmt_min = -16.8f;
    pid_cfg.output_lmt_max = 0.0f;
    pid_cfg.output_lmt_min = -16.8f;
    buck_set_volt_lmt_pid(&buck_param, &pid_cfg);

    pid_cfg.kp = 0.12f;
    pid_cfg.ki = 0.005f;
    pid_cfg.kd = 0.0f;
    pid_cfg.i_err_lmt_max = 50.0f;
    pid_cfg.i_err_lmt_min = -50.0f;
    pid_cfg.output_lmt_max = 50.0f;
    pid_cfg.output_lmt_min = -50.0f;
    buck_set_curr_pid(&buck_param, &pid_cfg);
    buck_set_curr_pid1(&buck_param, &pid_cfg);
}

static void ctrl_update_param(void)
{
    buck_param.input.i_l = BSP_ADC_GET(I_L);
    buck_param.input.i_l1 = BSP_ADC_GET(I_L1);
    buck_param.input.iin = BSP_ADC_GET(I_IN);
    buck_param.input.iout = BSP_ADC_GET(I_OUT);
    buck_param.input.vin = BSP_ADC_GET(V_IN);
    buck_param.input.vout = BSP_ADC_GET(V_OUT);
}

void ctrl_isr(void)
{
    ctrl_update_param();

    buck_alg(&buck_param);

    BSP_PWM_SET_INPUT(BUCK, buck_param.output.duty, 1, buck_param.output.dcm_mode ? 0 : 1, 0);
    BSP_PWM_SET_INPUT(BUCK1, buck_param.output.duty1, 1, buck_param.output.dcm_mode ? 0 : 1, 0);
    BSP_PWM_SET_INPUT(BOOST, 0.2, 1, 1, 0);
    BSP_PWM_SET_INPUT(BOOST1, 0.8, 1, 1, 0);
}
