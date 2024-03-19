#ifndef __FILTER_ALG_H
#define __FILTER_ALG_H

#define UP_LMT(val, up_val) {val = val > up_val ? up_val : val;}
#define DN_LMT(val, dn_val) {val = val < dn_val ? dn_val : val;}
#define UP_DN_LMT(val, up_val, dn_val) \
    {                                  \
        UP_LMT(val, up_val);           \
        DN_LMT(val, dn_val);           \
    }

float Filter1stOrder(float coefficient, float currentValue, float filterValue);

#endif