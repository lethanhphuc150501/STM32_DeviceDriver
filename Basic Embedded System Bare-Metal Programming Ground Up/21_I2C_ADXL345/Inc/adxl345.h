
#ifndef ADXL345_H_
#define ADXL345_H_

#include "i2c.h"
#include <stdint.h>

#define MPU6050_ADDR 		0x68		//Device address

#define ACCEL_CONFIG_REG	0x1C		//Data format (acceleration)
#define ACCEL_XOUT_H_REG	0x3B		//Data start address
#define PWR_MGMT_1_REG		0x6B		//Power control
#define WHO_AM_I_REG		0x75		//Device ID

#define FOUR_G				0x08
#define RESET				0x00


void adxl_init(void);
void adxl_read_values(uint8_t reg);

#endif /* ADXL345_H_ */
