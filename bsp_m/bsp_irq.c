#include "bsp_irq.h"
#include "misc.h"
#include "drive.h"
void bsp_irq_func(void)
{
    static uint8_t sw_car = 0;
    if(sw_car == 0)
    {
        BSP_PWM_SET_CAR(BUCK1,1099);
        BSP_PWM_SET_CAR(BUCK,1099);
        sw_car = 1;
    }
    else if(sw_car == 1)
    {
        BSP_PWM_SET_CAR(BUCK1,1199);
        BSP_PWM_SET_CAR(BUCK,1199);
        sw_car = 0;
    }
    ctrl_isr();
}
