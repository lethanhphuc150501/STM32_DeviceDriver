#include "adxl345.h"

char data;
uint8_t data_rec[6];

void adxl_read_address(uint8_t reg)
{
	I2C1_byteRead(MPU6050_ADDR, reg, &data);
}

void adxl_write(uint8_t reg, char value)
{
	char data[1];
	data[0] = value;
	I2C1_burstWrite(MPU6050_ADDR, reg, 1, data);
}

void adxl_read_values(uint8_t reg)
{
	I2C1_burstRead(MPU6050_ADDR, reg, 6, (char *) data_rec);
}

void adxl_init(void)
{
	/* Enable I2C */
	I2C1_init();

	/* Read the DEVID, this should return 0xE5 */
	adxl_read_address(WHO_AM_I_REG);

	/* Reset all bits */
	adxl_write(PWR_MGMT_1_REG, RESET);

	/* Set data format range to +-4g */
	adxl_write(ACCEL_CONFIG_REG, FOUR_G);
}
