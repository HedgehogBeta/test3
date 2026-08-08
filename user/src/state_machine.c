#include "state_machine.h"
#include "led.h"

/*状态枚举*/
typedef enum {
    STATE_IDLE,
    STATE_RUNNING,
    STATE_ALARM,
    STATE_COUNT,//哨兵
} state_t;

typedef void (*state_func)(void);

static void state_idle_run(void);
static void state_running_run(void);
static void state_alarm_run(void);

static state_t  current_state = STATE_IDLE;
static uint32_t state_start_tick = 0U;   /* 进入当前状态的时刻 */

#define ALL_LEDS (LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN)
#define STATE_DURATION_MS 3000U // 状态持续时间

static const state_func state_table[STATE_COUNT]={
    [STATE_IDLE] = state_idle_run,
    [STATE_RUNNING] = state_running_run,
    [STATE_ALARM] = state_alarm_run
};


static void state_idle_run(void){
    led_off(ALL_LEDS);
    if(HAL_GetTick() - state_start_tick >= STATE_DURATION_MS){
        current_state = STATE_RUNNING;
        state_start_tick = HAL_GetTick();
    }
}

static void state_running_run(void){
    
    static uint8_t step = 0U;
    led_off(ALL_LEDS);

    const uint8_t leds[4] = { LED1_PIN, LED2_PIN, LED3_PIN, LED4_PIN };
    led_on(leds[step]);
    step = (step+1U) % 4U;

    HAL_Delay(120U);
    if(HAL_GetTick() - state_start_tick >= STATE_DURATION_MS){
        current_state = STATE_ALARM;
        state_start_tick = HAL_GetTick();
    }

    
}

static void state_alarm_run(void){
    static uint8_t switch_alarm = 0U;
    if (switch_alarm == 0U){
        led_on (ALL_LEDS);
    }
    else {
        led_off(ALL_LEDS);
        
    }
    
    switch_alarm ^= 1U;

    HAL_Delay(60U);
    if(HAL_GetTick() - state_start_tick >= STATE_DURATION_MS){
        current_state = STATE_IDLE;
        state_start_tick = HAL_GetTick();
    }

}

void state_machine_run (void){
    if (current_state < STATE_COUNT && state_table[current_state] != NULL)
    {
        state_table[current_state]();
    }//边界检查
}

