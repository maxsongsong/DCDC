#ifdef BSP_TASK_ENUM
#undef BSP_TASK_INIT_START
#undef BSP_TASK_REG_FUNC
#undef BSP_TASK_INIT_END
#define BSP_TASK_INIT_START enum \
{
#define BSP_TASK_REG_FUNC(func, period) bsp_task_##func##_e,
#define BSP_TASK_INIT_END \
    TASK_MAX,             \
    }                     \
    ;                     \
    bsp_task_t bsp_task_table[TASK_MAX];
#endif

#ifdef BSP_TASK_EXTERN
#undef BSP_TASK_INIT_START
#undef BSP_TASK_REG_FUNC
#undef BSP_TASK_INIT_END
#define BSP_TASK_INIT_START ;
#define BSP_TASK_REG_FUNC(func, period) extern void func(void);
#define BSP_TASK_INIT_END ;
#endif

#ifdef BSP_TASK_FUNC
#undef BSP_TASK_INIT_START
#undef BSP_TASK_REG_FUNC
#undef BSP_TASK_INIT_END
#define BSP_TASK_INIT_START  \
    void bsp_task_init(void) \
    {
#define BSP_TASK_REG_FUNC(func, period)      \
    bsp_task_table[bsp_task_##func##_e].bsp_task_func = func;     \
    bsp_task_table[bsp_task_##func##_e].bsp_task_period = period; \
    bsp_task_table[bsp_task_##func##_e].cnt_last = 0;
#define BSP_TASK_INIT_END }
#endif
