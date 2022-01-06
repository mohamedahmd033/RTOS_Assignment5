

#include "tm4c123gh6pm.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"


#define RED_LED (1U<<1)
#define BLUE_LED (1U<<2)
#define GREEN_LED (1U<<3)
#define LED_OFF (0U)
#define PORTF (1U<<5)

#define NUMBER_OF_ITERATIONS_PER_ONE_MILI_SECOND (762*2)


void Three_LEDs_INIT(void);
void delay_ms(unsigned long n);

void task1(void*ptr);
void task2(void*ptr);


xSemaphoreHandle redMutex;
xSemaphoreHandle greenMutex;
int main()
{

	redMutex = xSemaphoreCreateMutex();
	greenMutex = xSemaphoreCreateMutex();

	
	Three_LEDs_INIT();
	
	if( redMutex != NULL && greenMutex != NULL  )
	{
	xTaskCreate(task1,"task1",240,NULL,2,NULL);
	xTaskCreate(task2,"task2",240,NULL,2,NULL);
	//xTaskCreate(gateKeeper1,"LowPriorityTask",240,NULL,1,NULL);
	//xTaskCreate(gateKeeper2,"LowPriorityTask",240,NULL,1,NULL);
	vTaskStartScheduler();
	}
	else
	{
	}
	
	for(;;);
}




void task1(void*ptr)
{	
	vTaskDelay(1000/portTICK_RATE_MS);
	int counter=0;
	
	for(;;)
	{
			xSemaphoreTake( greenMutex, portMAX_DELAY );
			{
						GPIO_PORTF_DATA_R =0x0U;
						while(counter<2)
						{
							GPIO_PORTF_DATA_BITS_R[GREEN_LED] = GREEN_LED;
							delay_ms(1000);
							
							GPIO_PORTF_DATA_R =LED_OFF;
							delay_ms(500);
							
							counter++;
						}
						counter = 0;
						
						vTaskDelay(100/portTICK_RATE_MS);
						
						xSemaphoreTake( redMutex, portMAX_DELAY );
						{
							GPIO_PORTF_DATA_R =0x0U;
							while(counter<2)
							{
								GPIO_PORTF_DATA_BITS_R[RED_LED] = RED_LED;
								delay_ms(1000);
							
								GPIO_PORTF_DATA_R =LED_OFF;
								delay_ms(500);
							
								counter++;
							}
						counter = 0;
						}
						
						xSemaphoreGive( redMutex );
			}
			xSemaphoreGive( greenMutex );
			
			vTaskDelay(100/portTICK_RATE_MS);
			


	}
	
}





void task2(void*ptr)
{	
	vTaskDelay(1000/portTICK_RATE_MS);
	int counter=0;
	
	for(;;)
	{
			xSemaphoreTake( redMutex, portMAX_DELAY );
			{
						GPIO_PORTF_DATA_R =0x0U;
						while(counter<2)
						{
							GPIO_PORTF_DATA_BITS_R[RED_LED] = RED_LED;
							delay_ms(1000);
							
							GPIO_PORTF_DATA_R =LED_OFF;
							delay_ms(500);
							
							counter++;
						}
						counter = 0;
						
						vTaskDelay(100/portTICK_RATE_MS);
						
						xSemaphoreTake( greenMutex, portMAX_DELAY );
						{
							GPIO_PORTF_DATA_R =0x0U;
							while(counter<2)
							{
								GPIO_PORTF_DATA_BITS_R[GREEN_LED] = GREEN_LED;
								delay_ms(1000);
							
								GPIO_PORTF_DATA_R =LED_OFF;
								delay_ms(500);
							
								counter++;
							}
						counter = 0;
						}
						
						xSemaphoreGive( greenMutex );
			}
			xSemaphoreGive( redMutex );
			
			vTaskDelay(100/portTICK_RATE_MS);
			


	}
	
}




void vApplicationIdleHook()
{
	int idleCounter = 0;
		while(1)
		{
			idleCounter++;
		}
}






void Three_LEDs_INIT(void)
{
	SYSCTL_RCGCGPIO_R|= PORTF;
	while((SYSCTL_PRGPIO_R&0x00000020)==0){}
	GPIO_PORTF_LOCK_R= 0x4C4F434B;
	GPIO_PORTF_CR_R=0x1F;
	GPIO_PORTF_DIR_R|= RED_LED | GREEN_LED | BLUE_LED;
	GPIO_PORTF_DEN_R|= RED_LED | GREEN_LED | BLUE_LED;
	GPIO_PORTF_DATA_R =LED_OFF;
	
}



void delay_ms(unsigned long n)
{
    volatile unsigned long count = 0;
    while(count++ < (NUMBER_OF_ITERATIONS_PER_ONE_MILI_SECOND * n) );
}



	