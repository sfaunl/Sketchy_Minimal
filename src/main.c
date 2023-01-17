/**
 * @file main.c
 * @author sefa unal
 * @brief 
 * @version 0.01
 * @date 2023-01-17
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "shal.h"
#include "scheduler.h"

#define LD1	PIN0	// Green LED
#define LD2	PIN7	// Blue LED
#define LD3	PIN14	// Red LED
/**
 * @brief 
 * 
 * @param taskInfo 
 * @param funcArg 
 * @return int 
 */
int TASK_LED1(Scheduler_Task *taskInfo, void *funcArg)
{
	(void)taskInfo;
	(void)funcArg;
	shal_gpio_toggle(GPIOB, LD1); // Toggle LED

	return 1; // return a non zero value to continue task
}
/**
 * @brief 
 * 
 * @param taskInfo 
 * @param funcArg 
 * @return int 
 */
int TASK_LED2(Scheduler_Task *taskInfo, void *funcArg)
{
	(void)taskInfo;
	(void)funcArg;
	shal_gpio_toggle(GPIOB, LD2); // Toggle LED

	return 1; // return a non zero value to continue task
}
/**
 * @brief 
 * 
 * @param userParam 
 */
void systick_callback(void *userParam)
{
	(void)userParam;
	shal_gpio_toggle(GPIOB, LD3); // Toggle LED
}

int main(void) {
	// Initialize time library
	shal_time_init();

	shal_systick_set_callback(SHAL_SYSTICK_CALLBACK_RELOAD, systick_callback, 0);

	// set gpio pins as output
	shal_gpio_set_as_output(GPIOB, LD1, SHAL_GPIO_OTYPE_PUSH_PULL, SHAL_GPIO_OSPEED_LOW, SHAL_GPIO_PULLUPDOWN_NONE);
	shal_gpio_set_as_output(GPIOB, LD2, SHAL_GPIO_OTYPE_PUSH_PULL, SHAL_GPIO_OSPEED_LOW, SHAL_GPIO_PULLUPDOWN_NONE);
	shal_gpio_set_as_output(GPIOB, LD3, SHAL_GPIO_OTYPE_PUSH_PULL, SHAL_GPIO_OSPEED_LOW, SHAL_GPIO_PULLUPDOWN_NONE);

	Scheduler *scheduler = scheduler_open(shal_time_get_ns);

	scheduler_task_new(scheduler, "LED1", SCHEDULER_PERIOD_MS(100), TASK_LED1, 0);
	scheduler_task_new(scheduler, "LED2", SCHEDULER_PERIOD_MS(1000), TASK_LED2, 0);

	// run tasks
	scheduler_run(scheduler);

	// we shouldn't get here!
	while (1);
}
