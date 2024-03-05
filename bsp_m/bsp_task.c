#include "bsp_task.h"
#include "stdint.h"
#define BSP_TASK_ENUM
#include "bsp_task_reg.h"
#undef BSP_TASK_ENUM

#define BSP_TASK_EXTERN
#include "bsp_task_reg.h"
#undef BSP_TASK_EXTERN

#define BSP_TASK_FUNC
#include "bsp_task_reg.h"
#undef BSP_TASK_FUNC

void bsp_task_run(uint32_t cnt)
{
    uint32_t i = 0;
    for (i = 0; i < TASK_MAX; i++)
    {
        if ((cnt - bsp_task_table[i].cnt_last) >= bsp_task_table[i].bsp_task_period)
        {
            bsp_task_table[i].cnt_last = cnt;
            bsp_task_table[i].bsp_task_func();
        }
    }
}
