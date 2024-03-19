#include "filter_alg.h"

float Filter1stOrder(float coefficient, float currentValue, float filterValue)
{
   return filterValue += (coefficient) * (currentValue - filterValue);
}
