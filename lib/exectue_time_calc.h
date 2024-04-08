#ifndef __EXECTUE_TIME_CALC_H
#define __EXECTUE_TIME_CALC_H

#include <stdio.h>
#include "stdint.h"



#define ARM_M4 1 

#ifdef ARM_M4 
#define SYSFERQ 120.0F
#define SYSCLK_COUNTER  
#endif


typedef struct
{
  uint32_t startCnt;
  uint32_t endCnt;
  float realTime;
  float minTime;
  float maxTime;
  float avgTime;
} TimeCalcData;

static inline void RecoderStartTimeCnt(TimeCalcData *pstTimeCalcData)
{
    pstTimeCalcData->startCnt = SYSCLK_COUNTER;
}

static inline void RecoderEndTimeCnt(TimeCalcData *pstTimeCalcData)
{
    pstTimeCalcData->endCnt = SYSCLK_COUNTER;
}

static inline void CalculateTime(TimeCalcData *pstTimeCalcData)
{
    pstTimeCalcData->realTime = (float)(pstTimeCalcData->endCnt - pstTimeCalcData->startCnt) / SYSFERQ;
    if (pstTimeCalcData->minTime > pstTimeCalcData->realTime) {
        pstTimeCalcData->minTime  = pstTimeCalcData->realTime;
    }
    if (pstTimeCalcData->maxTime < pstTimeCalcData->realTime) {
        pstTimeCalcData->maxTime  = pstTimeCalcData->realTime;
    }
    pstTimeCalcData->avgTime += (0.001f) * (pstTimeCalcData->realTime - pstTimeCalcData->avgTime);
}
#endif // __EXECTUE_TIME_CALC_H