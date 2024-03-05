#include "buck_curr.h"
#include "string.h"
#include "pid.h"
#include "math.h"
void buck_curr_reset(buck_curr_param_t *str)
{
    memset(str, 0, sizeof(buck_curr_param_t));
}

void buck_curr_set_volt_pid(buck_curr_param_t *str, pid_cfg_t *pid_cfg)
{
    memcpy(&str->inter.volt_pid.cfg, pid_cfg, sizeof(pid_cfg_t));
}

void buck_curr_set_curr_pid(buck_curr_param_t *str, pid_cfg_t *pid_cfg)
{
    memcpy(&str->inter.curr_pid.cfg, pid_cfg, sizeof(pid_cfg_t));
}

void buck_curr_set_cfg(buck_curr_param_t *str, buck_curr_cfg_t *cfg)
{
    memcpy(&str->inter.cfg, cfg, sizeof(buck_curr_cfg_t));
}

static float curr_loop(pid_param_t *str, float i_l, float iref)
{
    float out_tmp = 0.0f;
    str->input.act = i_l;
    str->input.ref = iref;
    out_tmp = pid_cal(str);
    return out_tmp;
}

static float buck_curr_dcm_cal_duty(buck_curr_param_t *str, float i_ref)
{

    return sqrt(2 * str->inter.cfg.l_val * i_ref * str->input.vout /
                ((str->input.vin - str->input.vout) * str->input.vin) /
                (str->inter.cfg.ctrl_ts));
}

static void buck_curr_dcm_judge(buck_curr_param_t *str, float i_ref)
{
    float dcm_curr;
    float duty_tmp;
    duty_tmp = str->input.vout / str->input.vin;
    float ton = 0.0f;
    ton = str->inter.cfg.ctrl_ts * duty_tmp;
    dcm_curr = (str->input.vin - str->input.vout) * ton / str->inter.cfg.l_val / 2.0f;
    str->output.dcm_curr = dcm_curr;
    str->output.dcm_mode = (i_ref < dcm_curr) ? 1 : 0;
}

void buck_curr_alg(buck_curr_param_t *str)
{
    float curr_loop_out = 0.0f;
    buck_curr_dcm_judge(str, str->input.iref);
    if (str->output.dcm_mode == 0)
    {
        curr_loop_out = curr_loop(&str->inter.curr_pid, str->input.i_l, str->input.iref);
        str->output.duty = (curr_loop_out + str->input.vout) / str->input.vin;
    }
    else
    {
        str->output.duty = buck_curr_dcm_cal_duty(str, str->input.iref);
    }
}
