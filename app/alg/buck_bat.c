#include "buck_bat.h"
#include "string.h"
#include "pid.h"
#include "math.h"
void buck_bat_reset(buck_bat_param_t *str)
{
    memset(str, 0, sizeof(buck_bat_param_t));
}

void buck_bat_set_volt_up_pid(buck_bat_param_t *str, pid_cfg_t *pid_cfg)
{
    memcpy(&str->inter.volt_up_pid.cfg, pid_cfg, sizeof(pid_cfg_t));
}

void buck_bat_set_volt_dn_pid(buck_bat_param_t *str, pid_cfg_t *pid_cfg)
{
    memcpy(&str->inter.volt_dn_pid.cfg, pid_cfg, sizeof(pid_cfg_t));
}

void buck_bat_set_vin_lmt_pid(buck_bat_param_t *str, pid_cfg_t *pid_cfg)
{
    memcpy(&str->inter.vin_lmt_pid.cfg, pid_cfg, sizeof(pid_cfg_t));
}

void buck_bat_set_curr_pid(buck_bat_param_t *str, pid_cfg_t *pid_cfg)
{
    memcpy(&str->inter.curr_pid.cfg, pid_cfg, sizeof(pid_cfg_t));
}

void buck_bat_set_cfg(buck_bat_param_t *str, buck_bat_cfg_t *cfg)
{
    memcpy(&str->inter.cfg, cfg, sizeof(buck_bat_cfg_t));
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

static float buck_bat_dcm_cal_duty(buck_bat_param_t *str, float i_ref)
{

    return sqrt(2 * str->inter.cfg.l_val * i_ref * str->input.vout /
                ((str->input.vin - str->input.vout) * str->input.vin) /
                (str->inter.cfg.ctrl_ts));
}

static void buck_bat_dcm_judge(buck_bat_param_t *str, float i_ref)
{
    float dcm_curr;
    float duty_tmp;
    duty_tmp = str->input.vout / str->input.vin;
    float ton = 0.0f;
    ton = str->inter.cfg.ctrl_ts * duty_tmp;
    dcm_curr = (str->input.vin - str->input.vout) * ton / str->inter.cfg.l_val / 2.0f;
    str->output.dcm_curr = dcm_curr;
    str->output.dcm_mode = ((i_ref < dcm_curr) && (i_ref > 0.0f)) ? 1 : 0;
}

static void buck_bat_set_vin_lmt(buck_bat_param_t *str)
{
    // str->inter.vin_lmt_ref = (str->input.vin > str->inter.vin_lmt_ref)
    //                              ? str->input.vin
    //                              : (BUCK_BAT_VIN_FLT_ECO * str->input.vin +
    //                                 (1.0f - BUCK_BAT_VIN_FLT_ECO) * str->inter.vin_lmt_ref);

    str->inter.vin_lmt_ref = (BUCK_BAT_VIN_FLT_ECO * str->input.vin +
                              (1.0f - BUCK_BAT_VIN_FLT_ECO) * str->inter.vin_lmt_ref);
}

static void buck_bat_clr_dn_pid(pid_param_t *str)
{
    if (str->input.ref < str->input.act)
    {
        str->inter.err = 0.0f;
        str->inter.i_err = 0.0f;
        str->inter.err_diff = 0.0f;
        str->inter.err_last = 0.0f;
    }
}

static float buck_bat_vin_vout_dn_comp(pid_param_t *vin, pid_param_t *vout_dn)
{
    float ret;
    if (vin->output.output > vout_dn->output.output)
    {
        vout_dn->inter.i_err = vin->inter.i_err;
        ret = -vin->output.output;
    }
    else
    {
        vin->inter.i_err = vout_dn->inter.i_err;
        ret = vout_dn->output.output;
    }
    return ret;
}

void buck_bat_alg(buck_bat_param_t *str)
{
    float curr_loop_out = 0.0f;
    float volt_loop_up_out = 0.0f;
    float volt_loop_dn_out = 0.0f;
    float vin_lmt_loop_out = 0.0f;
    float iref = 0.0f;

    buck_bat_set_vin_lmt(str);

    vin_lmt_loop_out = volt_loop(&str->inter.vin_lmt_pid, str->input.vin, 25.0f);
    volt_loop_up_out = volt_loop(&str->inter.volt_up_pid, str->input.vout, str->inter.cfg.vref_up);
    volt_loop_dn_out = volt_loop(&str->inter.volt_dn_pid, str->input.vout, str->inter.cfg.vref_dn);

    // buck_bat_clr_dn_pid(&str->inter.vin_lmt_pid);
    // buck_bat_clr_dn_pid(&str->inter.volt_dn_pid);

    // str->inter.volt_dn_pid.inter.i_err -= str->inter.vin_lmt_pid.inter.i_err;
    // str->inter.vin_lmt_pid.inter.i_err = 0;

    iref = volt_loop_up_out + volt_loop_dn_out + str->input.iref - vin_lmt_loop_out;
    // str->inter.vin_flt = str->inter.vin_flt * 0.8f + str->input.vin * 0.2f;
    buck_bat_dcm_judge(str, iref);
    if (str->output.dcm_mode == 0)
    {
        curr_loop_out = curr_loop(&str->inter.curr_pid, str->input.i_l, iref);
        str->output.duty = (curr_loop_out + str->input.vout) / str->input.vin;
    }
    else
    {
        str->output.duty = buck_bat_dcm_cal_duty(str, iref);
    }

    str->obs.curr_loop_out = curr_loop_out;
    str->obs.volt_loop_up_out = volt_loop_up_out;
    str->obs.volt_loop_dn_out = volt_loop_dn_out;
    str->obs.vin_lmt_loop_out = vin_lmt_loop_out;
    str->obs.iref = iref;
}
