#include "bsp_pwm.h"

void bsp_pwm_init(bsp_pwm_t *str, BSP_PWM_MODE_E pwm_mode, int32_t dead_zone, float car)
{
    str->cfg.car = car;
    str->input.car = car;
    str->cfg.dead_zone = dead_zone;
    str->cfg.pwm_mode = pwm_mode;
    str->inter.cnt_last = 0;
    str->inter.cnt_last_last = 0;
    str->inter.cmp_act = 0;
    str->inter.cmp_shadow = 0;
    str->inter.pwm_out_last = 0;
    str->inter.pwm_out_n_last = 0;
    str->inter.dir = 0;
    str->output.pwm_out = 0;
    str->output.pwm_out_n = 0;
}

void bsp_pwm_run(bsp_pwm_t *str)
{
    if (str->inter.cnt != str->inter.cnt_last)
    {
        if ((str->inter.cnt_last > str->inter.cnt) &&
            (str->inter.cnt == 0)) // 在计数器的谷值更新比较值
        {
            str->input.duty = (str->input.duty > 1.0f)
                                  ? 1.0f
                              : (str->input.duty < 0.0f)
                                  ? 0.0f
                                  : str->input.duty;
            str->inter.cmp_act = str->input.duty * (str->cfg.car + 1);
            str->inter.cmp_shadow = str->inter.cmp_act;
            str->inter.ch_en_last = str->input.ch_en;
            str->inter.chn_en_last = str->input.chn_en;
            str->inter.brake_lock = 0;
            str->cfg.car = str->input.car;
        }
        if (str->input.brake == 1)
        {
            str->inter.brake_lock = 1;
        }
        if (str->cfg.pwm_mode == BSP_PWM_MODE0)
        {
            if (str->inter.cnt < str->inter.cmp_shadow)
            {
                str->output.pwm_out = 1;
            }
            else
            {
                str->output.pwm_out = 0;
            }
        }
        else if (str->cfg.pwm_mode == BSP_PWM_MODE1)
        {
            if (str->inter.cnt < str->inter.cmp_shadow)
            {
                str->output.pwm_out = 0;
            }
            else
            {
                str->output.pwm_out = 1;
            }
        }
        else
        {
            str->output.pwm_out = 0;
        }

        str->output.pwm_out_n = str->output.pwm_out ? 0 : 1;

        if (((str->inter.pwm_out_last == 1) &&
             (str->output.pwm_out == 0)) ||
            ((str->inter.pwm_out_n_last == 1) &&
             (str->output.pwm_out_n == 0)))
        {
            str->inter.dead_time = str->cfg.dead_zone;
        }

        if (str->inter.brake_lock == 1)
        {
            str->output.pwm_out = 0;
            str->output.pwm_out_n = 0;
        }

        if (str->inter.dead_time)
        {
            str->inter.dead_time--;
            str->output.pwm_out = 0;
            str->output.pwm_out_n = 0;
        }

        if (str->inter.ch_en_last == 0)
        {
            str->output.pwm_out = 0;
        }

        if (str->inter.chn_en_last == 0)
        {
            str->output.pwm_out_n = 0;
        }

        str->inter.cnt_last_last = str->inter.cnt_last;
        str->inter.cnt_last = str->inter.cnt;
        str->inter.pwm_out_last = str->output.pwm_out;
        str->inter.pwm_out_n_last = str->output.pwm_out_n;
    }
    if (str->inter.dir == 0)
    {
        str->inter.cnt++;
        if (str->inter.cnt >= str->cfg.car)
        {
            str->inter.dir = 1;
        }
    }
    else if (str->inter.dir == 1)
    {
        if (str->inter.cnt)
        {
            str->inter.cnt--;
        }
        if (str->inter.cnt == 0)
        {
            str->inter.dir = 0;
        }
    }
}

float bsp_pwm_get_ch(bsp_pwm_t *str)
{
    return str->output.pwm_out;
}
float bsp_pwm_get_ch_n(bsp_pwm_t *str)
{
    return str->output.pwm_out_n;
}
void bsp_pwm_set_duty(bsp_pwm_t *str, float val)
{
    str->input.duty = val;
}
void bsp_pwm_set_cnt(bsp_pwm_t *str, float val)
{
    str->inter.cnt = val;
}
void bsp_pwm_set_ch_en(bsp_pwm_t *str, float val)
{
    str->input.ch_en = val;
}
void bsp_pwm_set_chn_en(bsp_pwm_t *str, float val)
{
    str->input.chn_en = val;
}
void bsp_pwm_set_brake(bsp_pwm_t *str, float val)
{
    str->input.brake = val;
}
void bsp_pwm_set_car(bsp_pwm_t *str, uint32_t val)
{
    str->input.car = val;
}
uint8_t bsp_pwm_get_top_update(bsp_pwm_t *str)
{
    if (str->inter.cnt == str->cfg.car)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
uint8_t bsp_pwm_get_down_update(bsp_pwm_t *str)
{
    if (str->inter.cnt == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
