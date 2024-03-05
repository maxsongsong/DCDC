#include "buck.h"
#include "string.h"
#include "pid.h"
#include "math.h"
void buck_reset(buck_param_t *str)
{
    memset(str, 0, sizeof(buck_param_t));
}

void buck_set_volt_pid(buck_param_t *str, pid_cfg_t *pid_cfg)
{
    memcpy(&str->inter.volt_pid.cfg, pid_cfg, sizeof(pid_cfg_t));
}

void buck_set_volt_lmt_pid(buck_param_t *str, pid_cfg_t *pid_cfg)
{
    memcpy(&str->inter.volt_lmt_pid.cfg, pid_cfg, sizeof(pid_cfg_t));
}

void buck_set_curr_pid(buck_param_t *str, pid_cfg_t *pid_cfg)
{
    memcpy(&str->inter.curr_pid.cfg, pid_cfg, sizeof(pid_cfg_t));
}

void buck_set_curr_pid1(buck_param_t *str, pid_cfg_t *pid_cfg)
{
    memcpy(&str->inter.curr_pid1.cfg, pid_cfg, sizeof(pid_cfg_t));
}

void buck_set_cfg(buck_param_t *str, buck_cfg_t *cfg)
{
    memcpy(&str->inter.cfg, cfg, sizeof(buck_cfg_t));
}

static float volt_loop(pid_param_t *str, float vout, float vref)
{
    float out_tmp = 0.0f;
    str->input.act = vout;
    str->input.ref = vref;
    out_tmp = pid_cal(str);
    return out_tmp;
}

static float curr_loop(pid_param_t *str, float i_l, float iref)
{
    float out_tmp = 0.0f;
    str->input.act = i_l;
    str->input.ref = iref;
    out_tmp = pid_cal(str);
    return out_tmp;
}

static float buck_dcm_cal_duty(buck_param_t *str, float i_ref)
{
    float duty_tmp = 0.0f;
    duty_tmp = sqrt(2 * str->inter.cfg.l_val * i_ref * str->input.vout / ((str->input.vin - str->input.vout) * str->input.vin) / (str->inter.cfg.ctrl_ts));
    if (isnan(duty_tmp))
    {
        duty_tmp = 0.0f;
    }
    return duty_tmp;
}

static void buck_dcm_judge(buck_param_t *str, float i_ref)
{
    float dcm_curr;
    float duty_tmp;
    duty_tmp = str->input.vout / str->input.vin;
    if (duty_tmp > 1.0f)
    {
        duty_tmp = 1.0f;
    }
    if (duty_tmp < 0.0f)
    {
        duty_tmp = 0.0f;
    }
    float ton = 0.0f;
    ton = str->inter.cfg.ctrl_ts * duty_tmp;
    dcm_curr = (str->input.vin - str->input.vout) * ton / str->inter.cfg.l_val / 2.0f;
    str->inter.dcm_curr = dcm_curr;
    float dcm_curr_hys = 0.0f;
    if (str->output.dcm_mode == 1)
    {
        if ((i_ref > dcm_curr) ||
            (str->input.vout < (str->inter.cfg.vref - 1.0f)))
        {
            str->output.dcm_mode = 0;
        }
    }
    else
    {
        if ((i_ref < (dcm_curr - 1.0f)) &&
            (str->input.vout < (str->inter.cfg.vref + 0.1f)))
        {
            str->output.dcm_mode = 1;
        }
    }
}

void buck_alg(buck_param_t *str)
{
    float volt_loop_out = 0.0f;
    float curr_loop_out = 0.0f;
    float curr_loop_out1 = 0.0f;
    volt_loop_out = volt_loop(&str->inter.volt_pid, str->input.vout, str->inter.cfg.vref + str->inter.volt_lmt_pid.output.output);
    volt_loop_out += str->input.iout;
    buck_dcm_judge(str, volt_loop_out / 2);
    float l_volt_fw = 0.0f;
    l_volt_fw = (volt_loop_out / 2 - str->inter.iref_last) * str->inter.cfg.l_val / str->inter.cfg.ctrl_ts;
    str->inter.l_volt_fw = l_volt_fw;
    str->inter.iref_last = volt_loop_out / 2;
    str->inter.volt_lmt_pid.input.act = volt_loop_out;
    str->inter.volt_lmt_pid.input.ref = 100.0f;
    pid_cal(&str->inter.volt_lmt_pid);
    if (str->output.dcm_mode == 0)
    {
        if (volt_loop_out > 100.0f)
        {
            volt_loop_out = 100.0f;
        }
        curr_loop_out = curr_loop(&str->inter.curr_pid, str->input.i_l, volt_loop_out / 2);
        str->inter.curr_loop_out = curr_loop_out;
        curr_loop_out1 = curr_loop(&str->inter.curr_pid1, str->input.i_l1, volt_loop_out / 2);
        curr_loop_out += l_volt_fw;
        curr_loop_out1 += l_volt_fw;
        str->output.duty = (curr_loop_out + str->input.vout) / str->input.vin;
        str->output.duty1 = (curr_loop_out1 + str->input.vout) / str->input.vin;
    }
    else
    {
        str->output.duty = buck_dcm_cal_duty(str, volt_loop_out / 2);
        str->output.duty1 = buck_dcm_cal_duty(str, volt_loop_out / 2);
        str->inter.volt_pid.inter.i_err = 0.0f;
        str->inter.curr_pid.inter.i_err = volt_loop_out / 2;
        str->inter.curr_pid1.inter.i_err = volt_loop_out / 2;
    }
    str->output.duty = (str->output.duty > 1.0f)
                           ? 1.0f
                       : (str->output.duty < 0.0f)
                           ? 0.0f
                           : str->output.duty;

    str->output.duty1 = (str->output.duty1 > 1.0f)
                            ? 1.0f
                        : (str->output.duty1 < 0.0f)
                            ? 0.0f
                            : str->output.duty1;
    str->output.duty1 = 1.0f - str->output.duty1;
}
