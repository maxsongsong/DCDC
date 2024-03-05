#ifndef __BSP_TASK_H
#define __BSP_TASK_H

#include "stdint.h"

typedef struct
{
    uint32_t bsp_task_period;
    void (*bsp_task_func)(void);
    uint32_t cnt_last;
} bsp_task_t;

void bsp_task_init(void);
void bsp_task_run(uint32_t cnt);

#endif
