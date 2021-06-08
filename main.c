/*
* Main Application. Initializes 7 IMUS and collects data from them every 2 Sec
*/

#include <DAVE.h>

#include "Dave/Generated/FREERTOS/task.h"
#include "Dave/Generated/FREERTOS/queue.h"

#include "source/ICM-42688-P/ICM-42688-P.h"


static void prvSendSPI( void *pvParameters );



/*-----------------------------------------------------------*/

/**
 * @brief main() - Application entry point
 *
 * <b>Details of function</b><br>
 * This routine is the application entry point. It is invoked by the device startup code. It is responsible for
 * invoking the App initialization dispatcher routine - DAVE_Init() and hosting the place-holder for user application
 * code.
 */
int main(void)
{

  if(DAVE_Init() == DAVE_STATUS_FAILURE)
  {
    /* Placeholder for error handler code. The while loop below can be replaced with an user error handler */
    XMC_DEBUG(("DAVE APPs initialization failed with status\n"));
    while(1U)
    {
    }
  }


	/* Start the two tasks as described in the accompanying application
	   note. */
	//xTaskCreate (prvBlinkyTask, ( const char * const ) "Blinky", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL );
	xTaskCreate (prvSendSPI, ( const char * const ) "SPI", 1000, NULL, tskIDLE_PRIORITY + 2, NULL );


	/* Start the tasks running. */
	vTaskStartScheduler();

  while(1U)
  {
  }
}


static void prvSendSPI( void *pvParameters )
{

	icm_42688_p_t imu0 = ICM_42688_P(&imu0, &SPI_MASTER_0, &TESTPIN);
	icm_42688_p_t imu1 = ICM_42688_P(&imu1, &SPI_MASTER_0, &TESTPIN);
	icm_42688_p_t imu2 = ICM_42688_P(&imu2, &SPI_MASTER_0, &TESTPIN);
	icm_42688_p_t imu3 = ICM_42688_P(&imu3, &SPI_MASTER_0, &TESTPIN);
	icm_42688_p_t imu4 = ICM_42688_P(&imu4, &SPI_MASTER_0, &TESTPIN);
	icm_42688_p_t imu5 = ICM_42688_P(&imu5, &SPI_MASTER_0, &TESTPIN);
	icm_42688_p_t imu6 = ICM_42688_P(&imu6, &SPI_MASTER_0, &TESTPIN);

	imu0.init(&imu0);
	imu1.init(&imu1);
	imu2.init(&imu2);
	imu3.init(&imu3);
	imu4.init(&imu4);
	imu5.init(&imu5);
	imu6.init(&imu6);

	for( ;; )
	{

		imu0.refreshReadings(&imu0);
		imu1.refreshReadings(&imu1);
		imu2.refreshReadings(&imu2);
		imu3.refreshReadings(&imu3);
		imu4.refreshReadings(&imu4);
		imu5.refreshReadings(&imu5);
		imu6.refreshReadings(&imu6);

		printf("Hello World\n!");


	  	vTaskDelay(2000);
	}
}
/*-----------------------------------------------------------*/

