/******************************************************
 * This file contains the driver for ICM-42688-P 
 * 
 * 
 * 
 * 
 * 
 ******************************************************/


#include "ICM-42688-P.h"

void icm_42688_p(icm_42688_p_t * imu, SPI_MASTER_t *SPI_MASTER_sel)
{
    imu->SPI_Master = SPI_MASTER_sel;


}
