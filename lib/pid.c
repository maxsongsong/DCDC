#include "pid.h"

void pid_reset(pid_param_t *pid_param)
{
    memset(pid_param, 0, sizeof(pid_param_t));
}

void pid_set_ref(pid_param_t *pid_param, float ref)
{
    pid_param->input.ref = ref;
}

void pid_set_act(pid_param_t *pid_param, float act)
{
    pid_param->input.act = act;
}

float pid_cal(pid_param_t *pid_param)
{
    pid_param->inter.err = pid_param->input.ref - pid_param->input.act;
    pid_param->inter.i_err += pid_param->inter.err * pid_param->cfg.ki;
    pid_param->inter.err_diff = pid_param->inter.err - pid_param->inter.err_last;
    pid_param->output.output = pid_param->cfg.kp * pid_param->inter.err +
                               pid_param->inter.i_err;

    if (pid_param->output.output > pid_param->cfg.output_lmt_max)
    {
        if (pid_param->cfg.kp * pid_param->inter.err > pid_param->cfg.output_lmt_max)
        {
            pid_param->inter.i_err = 0;
        }
        else
        {
            pid_param->inter.i_err = pid_param->cfg.output_lmt_max - pid_param->cfg.kp * pid_param->inter.err;
        }
        pid_param->output.output = pid_param->cfg.output_lmt_max;
    }
    else if (pid_param->output.output < pid_param->cfg.output_lmt_min)
    {
        if (pid_param->cfg.kp * pid_param->inter.err < pid_param->cfg.output_lmt_min)
        {
            pid_param->inter.i_err = 0;
        }
        else
        {
            pid_param->inter.i_err = pid_param->cfg.output_lmt_min - pid_param->cfg.kp * pid_param->inter.err;
        }
        pid_param->output.output = pid_param->cfg.output_lmt_min;
    }
    pid_param->inter.err_last = pid_param->inter.err;

    return pid_param->output.output;
}
