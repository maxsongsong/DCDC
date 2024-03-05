#ifndef __MISC_H
#define __MISC_H

#include "buck.h"
#include "cal_eff.h"
#include "buck_curr.h"
#include "boost.h"
#include "buck_bat.h"

void ctrl_init(void);
void ctrl_isr(void);

extern buck_param_t buck_param;

#endif
