#include "boost_logic.h"
#include "stdio.h"
#include "simstruc.h"
#include "stdint.h"
#include "fsm_run.h"
#include "list.h"

FsmObj g_boostFsm;  // 创建状态机对象
FsmStateObj g_boostIdie, g_boostRamp, g_boostRun,g_boostStop; // 给状态机创建状态对象

enum g_boostFsm_state
{
	boost_idie = 0,
	boost_ramp ,
	boost_run ,
	boost_stop 
};

/***相应的状态处理函数实现***/
void BoostIdieInit(void)
{
	
}

void BoostRampInit(void)
{
	
}

void BoostRunInit(void)
{

}

void BoostStopInit(void)
{
	
}

uint8_t OnBoostIdie()
{
    static uint8_t idieInitFlag = 0U;
	if (idieInitFlag == 0U) {
	    idieInitFlag = 1U;
        ssPrintf("on_idieINIT!!!\n");
        BoostIdieInit();
	}
    fsm_change_state(&g_boostFsm, boost_ramp);
	return 0U;
}

uint8_t OnBoostRamp()
{
    static uint8_t rampInitFlag = 0U;
	if (rampInitFlag == 0U) {
	    rampInitFlag = 1U;
        ssPrintf("on_idieINIT!!!\n");
        BoostRampInit();
	}
    ssPrintf("on_Boost_ramp!!!\n");
    fsm_change_state(&g_boostFsm, boost_run); 
	return 0U;
}


uint8_t OnBoostRun()
{
    static uint8_t runInitFlag = 0U;
	if (runInitFlag == 0U) {
	    runInitFlag = 1U;
        ssPrintf("on_idieINIT!!!\n");
        BoostRunInit();
	}
    ssPrintf("on_Boost_run!!!\n");
 

    // fsm_change_state(&g_boostFsm, boost_stop); 
	return 0U;
}

uint8_t OnBoostStop()
{
    static uint8_t stopInitFlag = 0U;
	if (stopInitFlag == 0U) {
	    stopInitFlag = 1U;
        BoostStopInit();  
	}
    fsm_change_state(&g_boostFsm, boost_idie);
	return 0U;
}





void BoostControllerFsmInit(void)
{    
	fsm_init(&g_boostFsm, "BoostController", boost_idie);   // 初始化状态机
     /*初始化状态对象*/
	fsm_state_init(&g_boostIdie, boost_idie, OnBoostIdie);
	fsm_state_init(&g_boostRamp, boost_ramp, OnBoostRamp);
	fsm_state_init(&g_boostRun,  boost_run,  OnBoostRun);
	fsm_state_init(&g_boostStop, boost_stop, OnBoostStop);

    /******** 将状态对象添加到相应的状态机中 **********/
	fsm_state_add(&g_boostFsm, &g_boostIdie);
	fsm_state_add(&g_boostFsm, &g_boostRamp);
	fsm_state_add(&g_boostFsm, &g_boostRun);
	fsm_state_add(&g_boostFsm, &g_boostStop);
}


uint8_t  g_boostFsmState;
void BoostControllerFsmRun(void)
{
	fsm_exec(&g_boostFsm);  // 运行状态机，
    g_boostFsmState = g_boostFsm.curr_state;
    ssPrintf("boostFsmState:%d\n", g_boostFsmState);
}


