#ifndef _ICM_42688_P_
#define _ICM_42688_P_

#pragma once

#include "Dave.h"
#include "Dave/Generated/FREERTOS/task.h"
#include <stdint.h>


#define BIT0 0x01
#define BIT1 0x02
#define BIT2 0x04
#define BIT3 0x08
#define BIT4 0x10
#define BIT5 0x20
#define BIT6 0x40
#define BIT7 0x80

#define READ_BIT BIT7       //The 7th bit in an 8 bit-register address indicates a read or write operation


const uint8_t PWR_MGMT0 = 		0x4E;
const uint8_t DEVICE_CONFIG = 	0x11;
const uint8_t SENSOR_CONFIG0 = 	0x03;
const uint8_t WHO_AM_I = 		0x75;

const uint8_t TEMP_DATA1 = 		0x1D;
const uint8_t TEMP_DATA	= 		0x1E;

// accelerometer with programmable full-scale range of ±2g, ±4g, ±8g and ±16g(default)
const uint8_t ACCEL_CONFIG0 =   0x50; // 7:5 accelerometer output range, 3:0 output data rate

const uint8_t ACCEL_DATA_X1	=	0x1F; // upper byte
const uint8_t ACCEL_DATA_X0	=	0x20; // lower byte
const uint8_t ACCEL_DATA_Y1	= 	0x21;
const uint8_t ACCEL_DATA_Y0 =	0x22;
const uint8_t ACCEL_DATA_Z1 =	0x23;
const uint8_t ACCEL_DATA_Z0 =	0x24;

const uint8_t _16_G = 0x00;						// ±16g (default) = 0000
const uint8_t _8_G = 0x20;						// ±8g = 0010
const uint8_t _4_G = 0x40;						// ±4g = 0100
const uint8_t _2_G = 0x60;						// ±2g = 0110

// gyroscope with programmable full-scale range of ±15.625, ±31.25, ±62.5, ±125, ±250, ±500, ±1000 and ±2000(default) degrees/sec
const uint8_t GYRO_CONFIG0 =	0x4F; // 7:5 gyroscope output range, 3:0 output data rate

const uint8_t GYRO_DATA_X1 = 	0x25; // upper byte
const uint8_t GYRO_DATA_X0 = 	0x26; // lower byte
const uint8_t GYRO_DATA_Y1 =	0x27;
const uint8_t GYRO_DATA_Y0 =	0x28;
const uint8_t GYRO_DATA_Z1 = 	0x29;
const uint8_t GYRO_DATA_Z0 =	0x2A;

const uint8_t _2000_DPS = 0x00;					// ±2000	degrees/sec	(default)
const uint8_t _1000_DPS = 0x20;					// ±1000 	degrees/sec
const uint8_t _500_DPS = 0x40;					// ±500 	degrees/sec
const uint8_t _250_DPS = 0x60;					// ±250 	degrees/sec
const uint8_t _125_DPS = 0x80;					// ±125 	degrees/sec
const uint8_t _65_5_DPS = 0xA0;					// ±500 	degrees/sec
const uint8_t _31_25_DPS = 0xC0;				// ±250 	degrees/sec
const uint8_t _15_625_DPS = 0xE0;				// ±125 	degrees/sec

const uint8_t _32_kHz = 0x01;					// 32kHz
const uint8_t _16_kHz = 0x02;					// 16kHz
const uint8_t _8_kHz = 0x03;					// 8kHz
const uint8_t _4_kHz = 0x04;					// 4kHz
const uint8_t _2_kHz = 0x05;					// 2kHz
const uint8_t _1_kHz = 0x06;					// 1kHz (default)


// UI Filter Block can be programmed to select filter order and bandwidth independently for gyroscope and accelerometer
const uint8_t ACCEL_CONFIG1 =	0x53;			//	4:3 ACCEL_UI_FILT_ORD (Filter Order), 2:1 ACCEL_DEC2_M2_ORD
const uint8_t GYRO_CONFIG1 =	0x51;			//	7:5 TEMP_FILT_BW, 3:2 GYRO_UI_FILT_ORD, 1:0 GYRO_DEC2_M2_ORD
const uint8_t GYRO_ACCEL_CONFIG0 = 0x52	;	//	7:4 ACCEL_UI_FILT_BW, 3:0 GYRO_UI_FILT_BW

// Filter Bandwidth for Low Noise Mode

const uint8_t bw0 = 0;			// BW = ODR / 2
const uint8_t bw1 = 0x11;		// BW = max(400Hz, ODR) / 4 (default)
const uint8_t bw2 = 0x22;		// BW = max(400Hz, ODR) / 5
const uint8_t bw3 = 0x33;		// BW = max(400Hz, ODR) / 8
const uint8_t bw4 = 0x44;		// BW = max(400Hz, ODR) / 10
const uint8_t bw5 = 0x55;		// BW = max(400Hz, ODR) / 16
const uint8_t bw6 = 0x66;		// BW = max(400Hz, ODR) / 20
const uint8_t bw7 = 0x77; 		// BW = max(400Hz, ODR) / 40

typedef struct imu_data
{
    uint16_t Accelerometer_XAxis;
    uint16_t Accelerometer_YAxis;
    uint16_t Accelerometer_ZAxis;

    uint16_t Gyroscope_XAxis;
    uint16_t Gyroscope_YAxis;
    uint16_t Gyroscope_ZAxis;

} imu_data_t;

typedef struct icm_42688_p
{
    //Methods
    void        (*registerWrite)        (icm_42688_p_t * imu, uint8_t registerAddress, uint8_t data);
    uint8_t     (*registerRead)         (icm_42688_p_t * imu, uint8_t registerAddress);
    void        (*init)                 (icm_42688_p_t * imu);
    void        (*refreshReadings)      (icm_42688_p_t * imu);
    


    //Members
    SPI_MASTER_t * SPI_Master;  //SPI Master Block that the IMU is connected to
    DIGITAL_IO_t * CS_Pin;      //CS Pin that the IMU is connected to
    imu_data_t     readings;


} icm_42688_p_t;


//Constructor
void ICM_42688_P(icm_42688_p_t * imu, SPI_MASTER_t *SPI_MASTER, DIGITAL_IO_t * CS_Pin);

#endif
