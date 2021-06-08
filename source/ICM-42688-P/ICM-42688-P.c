/******************************************************
 * This file contains the driver for ICM-42688-P 
 * 
 * 
 * 
 * 
 * 
 ******************************************************/


#include "ICM-42688-P.h"


static void registerWrite(icm_42688_p_t * imu, uint8_t registerAddress, uint8_t data)
{
    uint8_t Buffer[2] = {registerAddress, data};

    DIGITAL_IO_SetOutputLow(imu->CS_Pin);

    if(SPI_MASTER_Transmit(imu->SPI_Master, Buffer, 2) == SPI_MASTER_STATUS_SUCCESS)
    {
        while(SPI_MASTER_IsTxBusy(imu->SPI_Master));
    }

    DIGITAL_IO_SetOutputHigh(imu->CS_Pin);

}


static uint8_t registerRead(icm_42688_p_t * imu, uint8_t registerAddress)
{
    uint8_t Buffer[2] = {registerAddress | READ_BIT, 0};

    DIGITAL_IO_SetOutputLow(imu->CS_Pin);

    if(SPI_MASTER_Transmit(imu->SPI_Master, Buffer, 1) == SPI_MASTER_STATUS_SUCCESS)
    {
        while(SPI_MASTER_IsTxBusy(imu->SPI_Master));
    }

    uint8_t registerContent;

    if(SPI_MASTER_Receive(imu->SPI_Master, &registerContent, 1) == SPI_MASTER_STATUS_SUCCESS)
    {
        while(SPI_MASTER_IsRxBusy(imu->SPI_Master));
    }

    DIGITAL_IO_SetOutputHigh(imu->CS_Pin);

    return registerContent;

}

static void init(icm_42688_p_t * imu)
{
    // Switch Register Bank to register 2
    imu->registerWrite(imu, 0x76, BIT1);

    //Accel AAF off
    imu->registerWrite(imu, 0x03, BIT0);

    //Switch Register Bank to register 1
    imu->registerWrite(imu, 0x76, BIT0);

    // Gyro Notch off; AAF off
    imu->registerWrite(imu, 0x0B, BIT1 | BIT0);

    // Switch Register Bank to bank 0
    imu->registerWrite(imu, 0x76, BIT0);

    //// 7:6 Reserved | 5 TEMP_DIS | 4 IDLE | 3:2 GYRO_MODE | 1:0 ACCEL_MODE
    uint8_t LN_Mode = BIT1 | BIT0;
	uint8_t pwr_config = (LN_Mode <<2) | (LN_Mode);
    imu->registerWrite(imu, PWR_MGMT0, pwr_config);

    vTaskDelay(pdMS_TO_TICKS(10)); //Wait a few ms according to datasheet

    // 7:5 accelerometer output range | 3:0 output data rate
    imu->registerWrite(imu, ACCEL_CONFIG0, _16_G | _32_kHz);
    
    vTaskDelay(pdMS_TO_TICKS(10)); //Wait a few ms according to datasheet

    imu->registerWrite(imu, GYRO_CONFIG0, _125_DPS | _32_kHz);

    vTaskDelay(pdMS_TO_TICKS(10)); //Wait a few ms according to datasheet

}

static void refreshReadings(icm_42688_p_t * imu)
{

    imu->readings.Accelerometer_XAxis = imu->registerRead(imu, ACCEL_DATA_X1) << 8 | imu->registerRead(imu, ACCEL_DATA_X0);
    imu->readings.Accelerometer_YAxis = imu->registerRead(imu, ACCEL_DATA_Y1) << 8 | imu->registerRead(imu, ACCEL_DATA_Y0);
    imu->readings.Accelerometer_ZAxis = imu->registerRead(imu, ACCEL_DATA_Z1) << 8 | imu->registerRead(imu, ACCEL_DATA_Z0);
    
    imu->readings.Gyroscope_XAxis = imu->registerRead(imu, GYRO_DATA_X1) << 8 | imu->registerRead(imu, GYRO_DATA_X0);
    imu->readings.Gyroscope_YAxis = imu->registerRead(imu, GYRO_DATA_Y1) << 8 | imu->registerRead(imu, GYRO_DATA_Y0);
    imu->readings.Gyroscope_ZAxis = imu->registerRead(imu, GYRO_DATA_Z1) << 8 | imu->registerRead(imu, GYRO_DATA_Z0);

}

void ICM_42688_P(icm_42688_p_t * imu, SPI_MASTER_t *SPI_MASTER, DIGITAL_IO_t * CS_Pin)
{
    imu->SPI_Master = SPI_MASTER;
    imu->CS_Pin = CS_Pin;
    
    imu->registerWrite = registerWrite;
    imu->registerRead = registerRead;
    
    imu->init = init;
    imu->collectData = collectData;



}
