/*******************************************************************************
 This example contains a FREERTOS APP that comes with a modified GPL          **
 By using this examples you accept the license terms of the FREERTOS APP      **
 located in the help documentation of the FREERTOS APP.                       **
 FREERTOS APP can be downloaded from DAVE: -> Help -> Install DAVE APPs...    **
 then add the following link:                                                 **
 http://dave.infineon.com/Libraries/DAVE_Libs/Contributed/v4.0.0/             **
                                                                              **
 Copyright (c) 2015, Infineon Technologies AG                                 **
 All rights reserved.                                                         **
                                                                              **
 Redistribution and use in source and binary forms, with or without           **
 modification,are permitted provided that the following conditions are met:   **
                                                                              **
 *Redistributions of source code must retain the above copyright notice,      **
 this list of conditions and the following disclaimer.                        **
 *Redistributions in binary form must reproduce the above copyright notice,   **
 this list of conditions and the following disclaimer in the documentation    **
 and/or other materials provided with the distribution.                       **
 *Neither the name of the copyright holders nor the names of its contributors **
 may be used to endorse or promote products derived from this software without**
 specific prior written permission.                                           **
                                                                              **
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"  **
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE    **
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE   **
 ARE  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE   **
 LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR         **
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF         **
 SUBSTITUTE GOODS OR  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS    **
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN      **
 CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)       **
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE   **
 POSSIBILITY OF SUCH DAMAGE.                                                  **
                                                                              **
 To improve the quality of the software, users are encouraged to share        **
 modifications, enhancements or bug fixes with Infineon Technologies AG       **
 dave@infineon.com).                                                          **
                                                                              **
********************************************************************************
**                                                                            **
**                                                                            **
** PLATFORM : Infineon XMC4500 Series                                         **
**                                                                            **
** AUTHOR : Application Engineering Team                                      **
**                                                                            **
** version 4.0.0  Initial version                                             **
**         4.1.2  Modified for APP updates                                    **
**         4.1.4  Modified for APP updates									  **
**                                                                            **
** MODIFICATION DATE : OCT, 30, 2015                                          **
**                                                                            **
*******************************************************************************/


/*
 * This is a very simple demo that demonstrates task and queue usages only.
 * Details of other FreeRTOS features (API functions, tracing features,
 * diagnostic hook functions, memory management, etc.) can be found on the
 * FreeRTOS web site (http://www.FreeRTOS.org) and in the FreeRTOS book.
 * Details of this demo (what it does, how it should behave, etc.) can be found
 * in the accompanying PDF application note.
 * http://interactive.freertos.org/attachments/token/rpjspilbw7qbvca/?name=FR-191-AN-RB-003i2-_A_simple_FreeRTOS_demo_for_the_LPCXpresso_.pdf
 *
*/

#include <DAVE.h>

#include "Dave/Generated/FREERTOS/task.h"
#include "Dave/Generated/FREERTOS/queue.h"

//#include "source/ICM-42688-P/ICM-42688-P.h"
#include "ICM-42688-P.h"


static void prvBlinkyTask( void *pvParameters );
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

/*-----------------------------------------------------------*/
void imu_init(SPI_MASTER_t *SPI_MASTER_sel)
{
	uint8_t SendData[2];

	// Switch Register Bank
	SendData[0] = 0x76;
	SendData[1]	= Bit1; //Register 2
	if(SPI_MASTER_Transmit(SPI_MASTER_sel, SendData, 2) == SPI_MASTER_STATUS_SUCCESS)
		while(SPI_MASTER_IsTxBusy(SPI_MASTER_sel));

	// Accel AAF off
	SendData[0] = 0x03;
	SendData[1]	= Bit0;
	if(SPI_MASTER_Transmit(SPI_MASTER_sel, SendData, 2) == SPI_MASTER_STATUS_SUCCESS)
		while(SPI_MASTER_IsTxBusy(SPI_MASTER_sel));

	// Switch Register Bank
	SendData[0] = 0x76;
	SendData[1]	= Bit0; //Register 1
	if(SPI_MASTER_Transmit(SPI_MASTER_sel, SendData, 2) == SPI_MASTER_STATUS_SUCCESS)
		while(SPI_MASTER_IsTxBusy(SPI_MASTER_sel));

	// Gyro Notch off; AAF off
	SendData[0] = 0x0B;
	SendData[1]	= Bit1 | Bit0;
	if(SPI_MASTER_Transmit(SPI_MASTER_sel, SendData, 2) == SPI_MASTER_STATUS_SUCCESS)
		while(SPI_MASTER_IsTxBusy(SPI_MASTER_sel));

	// Switch Register Bank
	SendData[0] = 0x76;
	SendData[1]	= 0x00; //Register 0
	if(SPI_MASTER_Transmit(SPI_MASTER_sel, SendData, 2) == SPI_MASTER_STATUS_SUCCESS)
		while(SPI_MASTER_IsTxBusy(SPI_MASTER_sel));

	/*/ Set data ready reset mode //has to wait now sometimes
	SendData[0] = 0x63;
	SendData[1]	= 0b10100000; //Register 0
	if(SPI_MASTER_Transmit(SPI_MASTER_sel, SendData, 2) == SPI_MASTER_STATUS_SUCCESS)
		while(SPI_MASTER_IsTxBusy(SPI_MASTER_sel));
		*/

	/*/ Set interrupt times to 8us //No effect on spikes
	SendData[0] = 0x64;
	SendData[1]	= 0b11100000; //Register 0
	if(SPI_MASTER_Transmit(SPI_MASTER_sel, SendData, 2) == SPI_MASTER_STATUS_SUCCESS)
		while(SPI_MASTER_IsTxBusy(SPI_MASTER_sel));
		*/


	//// 7:6 Reserved | 5 TEMP_DIS | 4 IDLE | 3:2 GYRO_MODE | 1:0 ACCEL_MODE
	uint8_t LN_Mode = Bit1 | Bit0;
	uint8_t pwr_config = (LN_Mode <<2) | (LN_Mode);
	SendData[0] = PWR_MGMT0;
	SendData[1] = pwr_config;
	if(SPI_MASTER_Transmit(SPI_MASTER_sel, SendData, 2) == SPI_MASTER_STATUS_SUCCESS)
		while(SPI_MASTER_IsTxBusy(SPI_MASTER_sel));

	int counter = 0;
	while(counter<3000000)
	{
		counter++;
	}


	// 7:5 accelerometer output range | 3:0 output data rate
	SendData[0] = ACCEL_CONFIG0;
	SendData[1]	= _16_G | _32_kHz;
	if(SPI_MASTER_Transmit(SPI_MASTER_sel, SendData, 2) == SPI_MASTER_STATUS_SUCCESS)
		while(SPI_MASTER_IsTxBusy(SPI_MASTER_sel));

	counter = 0;
	while(counter<3000000)
	{
		counter++;
	}

	// 7:5 gyroscope output range | 3:0 output data rate
	SendData[0] = GYRO_CONFIG0;
	SendData[1] = _125_DPS | _32_kHz;
	if(SPI_MASTER_Transmit(SPI_MASTER_sel, SendData, 2) == SPI_MASTER_STATUS_SUCCESS)
			while(SPI_MASTER_IsTxBusy(SPI_MASTER_sel));

	counter = 0;
	while(counter<3000000)
	{
		counter++;
	}

}



static void prvSendSPI( void *pvParameters )
{

	// A list of the 12 registers that we need to read, OR'd with the Read bit
//	uint8_t SPI_SendData[12] = {ACCEL_DATA_X1 | Bit7, ACCEL_DATA_X0 | Bit7, ACCEL_DATA_Y1 | Bit7, ACCEL_DATA_Y0 | Bit7, ACCEL_DATA_Z1 | Bit7, ACCEL_DATA_Z0 | Bit7, GYRO_DATA_X1 | Bit7, GYRO_DATA_X0 | Bit7, GYRO_DATA_Y1 | Bit7, GYRO_DATA_Y0 | Bit7, GYRO_DATA_Z1 | Bit7, GYRO_DATA_Z0 | Bit7};
//	uint8_t SPI_ReadData[12];

//	uint8_t SPI_WriteBuffer[24]  = { ACCEL_DATA_X1 | Bit7, 0,  ACCEL_DATA_X0 | Bit7, 0,  ACCEL_DATA_Y1 | Bit7, 0,  ACCEL_DATA_Y0 | Bit7, 0,  ACCEL_DATA_Z0 | Bit7, 0,  GYRO_DATA_X1 | Bit7, 0,
//									GYRO_DATA_X0 | Bit7, 0,  GYRO_DATA_Y1 | Bit7, 0,  GYRO_DATA_Y0 | Bit7, 0,  GYRO_DATA_Z1 | Bit7, 0,  GYRO_DATA_Z0 | Bit7, 0,  0x75 | Bit7, 0};
//	uint8_t SPI_ReadBuffer[24] = {0};

	uint8_t SPI_WriteBuffer[2]  = { ACCEL_DATA_X1 | Bit7, 0};
	uint8_t SPI_ReadBuffer[2] = {0};


	DIGITAL_IO_SetOutputLow(&TESTPIN);
	imu_init(&SPI_MASTER_0);
	DIGITAL_IO_SetOutputHigh(&TESTPIN);


	for( ;; )
	{


 		DIGITAL_IO_SetOutputLow(&TESTPIN);

 		SPI_MASTER_Transfer(&SPI_MASTER_0, SPI_WriteBuffer, SPI_ReadBuffer, 2);

 		 		/* Wait until the data transmission is finished */
 		 		while(SPI_MASTER_0.runtime->tx_busy)
 		 		{
 		 		}

 		 		while(SPI_MASTER_0.runtime->rx_busy)
 		 		{
 		 		}

				/* Ensure the last byte is shifted out from the buffer at lower baud rates, when frame end mode is disabled. */
				while (SPI_MASTER_GetFlagStatus(&SPI_MASTER_0, (uint32_t)XMC_SPI_CH_STATUS_FLAG_MSLS) != 0U)
				{

				}

 		 		DIGITAL_IO_SetOutputHigh(&TESTPIN);



// 		 SPI_MASTER_Transmit(&SPI_MASTER_0, SPI_SendData, 12);
// 		 SPI_MASTER_Receive(&SPI_MASTER_0, SPI_ReadData, 12);
// //		SPI_MASTER_Transfer(&SPI_MASTER_0, SPI_SendData, SPI_ReadData, 12);

// 		/* Wait until the data transmission is finished */
// 		while(SPI_MASTER_0.runtime->tx_busy)
// 		{
// 		}

// 		while(SPI_MASTER_0.runtime->rx_busy)
// 		{
// 		}

// 		/* Ensure the last byte is shifted out from the buffer at lower baud rates, when frame end mode is disabled. */
// 		while (SPI_MASTER_GetFlagStatus(&SPI_MASTER_0, (uint32_t)XMC_SPI_CH_STATUS_FLAG_MSLS) != 0U)
// 		{

// 		}

		// DIGITAL_IO_SetOutputHigh(&TESTPIN);

		// Collect data from IMU
//		for(int i = 0; i<12; i++)
//		 {
//			DIGITAL_IO_SetOutputLow(&TESTPIN);
//			 // Transmits the specified number of data words
//			 if(SPI_MASTER_Transmit(&SPI_MASTER_0, &SPI_SendData[i], 1) == SPI_MASTER_STATUS_SUCCESS)
//				 while(SPI_MASTER_IsTxBusy(&SPI_MASTER_0));
//
//			 // Receives the specified number of data words
//			 if(SPI_MASTER_Receive(&SPI_MASTER_0, &SPI_ReadData[i], 1) == SPI_MASTER_STATUS_SUCCESS);
//				 while(SPI_MASTER_IsRxBusy(&SPI_MASTER_0));
//			DIGITAL_IO_SetOutputHigh(&TESTPIN);
//
//		 }




	  vTaskDelay(2000);
	}
}
/*-----------------------------------------------------------*/


static void prvBlinkyTask( void *pvParameters )
{
  for( ;; )
  {
	  DIGITAL_IO_ToggleOutput(&TESTPIN);
	  vTaskDelay(1500);
  }
}
/*-----------------------------------------------------------*/


