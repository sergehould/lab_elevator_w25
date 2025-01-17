 /* ***************************************************************************
  * Elevator project 
  * Description: 
  * 
  * You must provide an history of your code every week.
  *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  * Author  Date            Comments on this revision
  *~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  * 
  * 
  *        
 *****************************************************************************/

/* Kernel includes. */
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h" 
#include "semphr.h"
#include "croutine.h"
#include <stdio.h>
#include "event_groups.h"
#include "include/public.h"
#include "include/configuration.h"
#include "include/motors.h"
#include "include/ssd.h"
#include "include/initBoard.h"
#include "include/console.h"
#include "include/Tick_core.h"
#include "include/util.h"

/* Prototypes for the standard FreeRTOS callback/hook functions implemented within this file. */
void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName );

int main( void ){
	/* Enable multiVectoredInt.   */
	INTCONbits.MVEC=1; 
    
    /* Initializes resources */
    ios_init();
    heartbeat_init();
    //ssd_init(); // seven segment display
    encoders_init(); // motor encoder initialization
    motors_init(); // motor initialization
    
    /* create tasks here */
    
    vTaskStartScheduler(); // starts scheduler
	return 0;
}

void vApplicationIdleHook( void ){
    int cnt=0;
    while(1){
        // to complete
    }
}

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
    pxTask= pxTask;
    pcTaskName = pcTaskName;
		for( ;; );
}

