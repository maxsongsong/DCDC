#ifndef __BSP_PWM_H
#define __BSP_PWM_H

#include "stdint.h"

#define BSP_PWM_REG(name) bsp_pwm_t bsp_pwm_##name;

#define BSP_PWM_DEC(name) extern bsp_pwm_t bsp_pwm_##name;

#define BSP_PWM_SET_CNT(name, cnt) \
    bsp_pwm_set_cnt(&bsp_pwm_##name, cnt)

#define BSP_PWM_SET_CAR(name, car) \
    bsp_pwm_set_car(&bsp_pwm_##name, car)

#define BSP_PWM_SET_INPUT(name, duty, ch_en, chn_en, brake) \
    bsp_pwm_set_duty(&bsp_pwm_##name, duty);                \
    bsp_pwm_set_ch_en(&bsp_pwm_##name, ch_en);              \
    bsp_pwm_set_chn_en(&bsp_pwm_##name, chn_en);            \
    bsp_pwm_set_brake(&bsp_pwm_##name, brake)

#define BSP_PWM_INIT(name, car, dead_zone, pwm_mode) bsp_pwm_init(&bsp_pwm_##name, pwm_mode, dead_zone, car)
#define BSP_PWM_RUN(name) bsp_pwm_run(&bsp_pwm_##name)

#define BSP_PWM_GET_CH(name) bsp_pwm_get_ch(&bsp_pwm_##name)
#define BSP_PWM_GET_CH_N(name) bsp_pwm_get_ch_n(&bsp_pwm_##name)

#define BSP_PWM_GET_TOP_UPDATE(name) bsp_pwm_get_top_update(&bsp_pwm_##name)
#define BSP_PWM_GET_DOWN_UPDATE(name) bsp_pwm_get_down_update(&bsp_pwm_##name)

typedef enum
{
    BSP_PWM_MODE0,
    BSP_PWM_MODE1,
} BSP_PWM_MODE_E;

typedef struct
{
    float duty;     // 输入占空比 0～1之间
    uint8_t ch_en;  // 输出使能
    uint8_t chn_en; // 互补输出使能
    uint8_t brake;  // 高电平有效
    uint32_t car;
} bsp_pwm_input_t;

typedef struct
{
    BSP_PWM_MODE_E pwm_mode; // PWM模式
    uint32_t dead_zone;      // 死区时间
    uint32_t car;            // 满值
} bsp_pwm_cfg_t;

typedef struct
{
    uint32_t cnt; // 内部计数器值
    uint32_t cmp_shadow;
    uint32_t cmp_act;
    uint32_t cnt_last;
    uint32_t cnt_last_last;
    uint8_t pwm_out_last;
    uint8_t pwm_out_n_last;
    uint32_t dead_time;
    uint8_t ch_en_last;
    uint8_t chn_en_last;
    uint8_t brake_lock;
    uint8_t dir;
} bsp_pwm_inter_t;

typedef struct
{
    uint8_t pwm_out;   // PWM输出
    uint8_t pwm_out_n; // PWM互补输出
} bsp_pwm_output_t;

typedef struct
{
    float test;
    bsp_pwm_input_t input;
    bsp_pwm_cfg_t cfg;
    bsp_pwm_inter_t inter;
    bsp_pwm_output_t output;
} bsp_pwm_t;

void bsp_pwm_init(bsp_pwm_t *str, BSP_PWM_MODE_E pwm_mode, int32_t dead_zone, float car);
void bsp_pwm_run(bsp_pwm_t *str);

float bsp_pwm_get_ch(bsp_pwm_t *str);
float bsp_pwm_get_ch_n(bsp_pwm_t *str);
void bsp_pwm_set_duty(bsp_pwm_t *str, float val);
void bsp_pwm_set_cnt(bsp_pwm_t *str, float val);
void bsp_pwm_set_ch_en(bsp_pwm_t *str, float val);
void bsp_pwm_set_chn_en(bsp_pwm_t *str, float val);
void bsp_pwm_set_brake(bsp_pwm_t *str, float val);
void bsp_pwm_set_car(bsp_pwm_t *str, uint32_t val);
uint8_t bsp_pwm_get_top_update(bsp_pwm_t *str);
uint8_t bsp_pwm_get_down_update(bsp_pwm_t *str);

#endif
