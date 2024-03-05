#include "bsp_init.h"
#include "drive.h"
#include "../app/misc.h"

void bsp_init_func(void)
{
    ctrl_init();
    BSP_PWM_INIT(BUCK, 1199.0f, 10, BSP_PWM_MODE0);
    BSP_PWM_INIT(BUCK1, 1199.0f, 10, BSP_PWM_MODE1);
    BSP_PWM_INIT(BOOST, 1199.0f, 10, BSP_PWM_MODE0);
    BSP_PWM_INIT(BOOST1, 1199.0f, 10, BSP_PWM_MODE1);
}
