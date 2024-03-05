#include "boost.h"
#include "string.h"
#include "pid.h"
#include "math.h"
void boost_reset(boost_param_t *str)
{
    memset(str, 0, sizeof(boost_param_t));
}

void boost_set_volt_pid(boost_param_t *str, pid_cfg_t *pid_cfg)
{
    memcpy(&str->inter.volt_pid.cfg, pid_cfg, sizeof(pid_cfg_t));
}

void boost_set_curr_pid(boost_param_t *str, pid_cfg_t *pid_cfg)
{
    memcpy(&str->inter.curr_pid.cfg, pid_cfg, sizeof(pid_cfg_t));
}

void boost_set_cfg(boost_param_t *str, boost_cfg_t *cfg)
{
    memcpy(&str->inter.cfg, cfg, sizeof(boost_cfg_t));
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

static float boost_dcm_cal_duty(boost_param_t *str, float i_ref)
{

    return sqrt(2 * str->inter.cfg.l_val * i_ref * str->input.vout / ((str->input.vin - str->input.vout) * str->input.vin) / (str->inter.cfg.ctrl_ts));
}

static void boost_dcm_judge(boost_param_t *str, float i_ref)
{
    float dcm_curr;
    float duty_tmp;
    duty_tmp = str->input.vout / str->input.vin;
    float ton = 0.0f;
    ton = str->inter.cfg.ctrl_ts * duty_tmp;
    dcm_curr = (str->input.vin - str->input.vout) * ton / str->inter.cfg.l_val / 2.0f;
    str->output.dcm_mode = ((i_ref < dcm_curr) && (str->input.vout < (str->inter.cfg.vref + 0.1f))) ? 1 : 0;
}

void boost_alg(boost_param_t *str)
{
    float volt_loop_out = 0.0f;
    float curr_loop_out = 0.0f;
    volt_loop_out = volt_loop(&str->inter.volt_pid, str->input.vout, str->inter.cfg.vref);
    curr_loop_out = curr_loop(&str->inter.curr_pid, str->input.i_l, volt_loop_out);
    str->output.duty = 1.0f - ((str->input.vin - curr_loop_out) / str->input.vout);
    str->output.duty = (str->output.duty > 0.95f) ? 0.95f : str->output.duty;
    str->output.duty = (str->output.duty < 0.0f) ? 0.0f : str->output.duty;
    str->output.dcm_mode = 0;
}
