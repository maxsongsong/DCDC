#include "boost_ctrl.h"
#include "boost_paramete_define.h"
#include "pid.h"
#include "filter_alg.h"

BoostCtrl_t g_boostCtrl = {
    .voltLoop {
        .cfg {
            .kp = 0.0f,
            .ki = 0.0f,
            .i_err_lmt_max =0.0f,
            .i_err_lmt_min =0.0f,
            .output_lmt_max = 0.0f,
            .output_lmt_min = 0.0f,
        }
    },
    .currLoop {
        .cfg {
            .kp = 0.0f,
            .ki = 0.0f,
            .i_err_lmt_max =0.0f,
            .i_err_lmt_min =0.0f,
            .output_lmt_max = 0.0f,
            .output_lmt_min = 0.0f,
        }
    },
    .outVoltRef = 400.0f,
    .feedforwardCoefficient = 1.0f,
};

static void BoostVoltCtrlLoop(BoostCtrl_t *pstBoostCtrl)
{
#ifdef USE_PI_CONTROLLER_BASE_BOOST
    pstBoostCtrl->voltLoop.input.act = pstBoostCtrl->outVoltFiler; 
    pstBoostCtrl->voltLoop.input.ref = pstBoostCtrl->outVoltRealRef;
    pstBoostCtrl->voltLoopResult = pid_cal(&pstBoostCtrl->voltLoop);
    UP_DN_LMT(pstBoostCtrl->voltLoopResult,100.0f,0.0f)
#else
    //TODO: 需要增加一个NPNZ控制
#endif
}

static void BoostCurrCtrlLoop(BoostCtrl_t *pstBoostCtrl)
{
    pstBoostCtrl->feedforwardCoefficient = pstBoostCtrl->outVoltFiler - pstBoostCtrl->vinVoltFiler;
#ifdef USE_PI_CONTROLLER_BASE_BOOST
    pstBoostCtrl->currLoop.input.act = pstBoostCtrl->currFiler;
    pstBoostCtrl->currLoop.input.ref = pstBoostCtrl->voltLoopResult;
    pstBoostCtrl->currLoopResult = pid_cal(&pstBoostCtrl->currLoop);
#else
   //TODO: 需要增加一个NPNZ控制
#endif
}

static float BoostPwmGain(BoostCtrl_t *pstBoostCtrl)
{
    float duty;
    pstBoostCtrl->vpwm = pstBoostCtrl->currLoopResult + pstBoostCtrl->feedforwardVolt * pstBoostCtrl->feedforwardCoefficient;
    UP_DN_LMT(pstBoostCtrl->vpwm, pstBoostCtrl->outVoltRef, pstBoostCtrl->vinVoltFiler);
    pstBoostCtrl->vinVoltFiler = (pstBoostCtrl->vinVoltFiler < 1.0f) ? 1.0f : pstBoostCtrl->vinVoltFiler; //除法做非零判断
    duty = pstBoostCtrl->vpwm / pstBoostCtrl->vinVoltFiler;
    UP_DN_LMT(duty, BOOST_MAX_DUTY, BOOST_MIN_DUTY); //限幅
    return duty;
}

static void BoostPwmSet(BoostCtrl_t *pstBoostCtrl)
{
    pstBoostCtrl->duty = BoostPwmGain(pstBoostCtrl);
}

static void BoostOutVoltRamp(BoostCtrl_t *pstBoostCtrl)
{
    float refRampStep;
    if (pstBoostCtrl->softStatus == FALSE) {
        pstBoostCtrl->rampCnt++;  
    }
    if ((pstBoostCtrl->outVoltRampRef >= pstBoostCtrl->outVoltRef) && (pstBoostCtrl->softStatus == FALSE)) {
        pstBoostCtrl->outVoltRampRef = pstBoostCtrl->outVoltRef;
        pstBoostCtrl->outVoltRealRef = pstBoostCtrl->outVoltRef;
        pstBoostCtrl->softStatus = TRUE;
        pstBoostCtrl->rampCnt = 0U;
    }
    else if ((pstBoostCtrl->softStatus == FALSE) && (pstBoostCtrl->rampCnt > CNT_1MS_BASE_40KHZ)) {
        refRampStep = (pstBoostCtrl->outVoltRef - pstBoostCtrl->vinVoltFiler) / BOOST_OUT_VREF_STEP;
        pstBoostCtrl->outVoltRampRef = pstBoostCtrl->vinVoltFiler + refRampStep;
        pstBoostCtrl->rampCnt = 0U;
    } else {
        return;
    }
}

static void BoostCtrlLoop(BoostCtrl_t *pstBoostCtrl)
{
    BoostVoltCtrlLoop(pstBoostCtrl);
    BoostCurrCtrlLoop(pstBoostCtrl);
}

static void BoostAdcGain(BoostCtrl_t *pstBoostCtrl)
{
    pstBoostCtrl->outVoltFiler = Filter1stOrder(FILTER_0P05HZ_BASE_5HZ, pstBoostCtrl->outVoltRef, pstBoostCtrl->outVoltFiler);
    pstBoostCtrl->currFiler = Filter1stOrder(FILTER_0P05HZ_BASE_5HZ, pstBoostCtrl->currRef, pstBoostCtrl->currFiler);
    pstBoostCtrl->vinVoltFiler = Filter1stOrder(FILTER_0P05HZ_BASE_5HZ, pstBoostCtrl->vinVolt, pstBoostCtrl->vinVoltFiler);
}
void BoostCtrlInit(void)
{
    PidComputationalClear(&g_boostCtrl.currLoop);
    PidComputationalClear(&g_boostCtrl.voltLoop);
    g_boostCtrl.outVoltFiler = 0.0f;
    g_boostCtrl.currFiler = 0.0f;
    g_boostCtrl.vinVoltFiler = 0.0f;
}
void BoostController(void)
{
    BoostAdcGain(&g_boostCtrl);
    BoostOutVoltRamp(&g_boostCtrl);
    BoostCtrlLoop(&g_boostCtrl);
    BoostPwmSet(&g_boostCtrl);
}

bool GetBoostCtrlSoftStatus()
{
    return g_boostCtrl.softStatus;
}