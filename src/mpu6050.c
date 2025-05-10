#include "i2c.h"
#include "helpers.h"
#include "mpu6050.h"
#include "rp2040-map.h"

#define IC_CON_SLV_DIS_BITS		BIT(6)
#define IC_CON_MSTR_MODE_EN_BITS	(0x1)
#define IC_CON_RESTART_EN_SHIFT		(5)
#define IC_CON_SPEED_SHIFT		(1)

#define IC_CON_SPEED_STD_BITS		(0x1)
#define IC_STATUS_TFNF_SHIFT		(0x1)
#define IC_ENABLE_ENABLE_BITS		(0x1)
#define IC_DATA_CMD_CMD_SHIFT		(8)
#define IC_DATA_CMD_STOP_SHIFT		(9)
#define IC_RAW_INTR_STAT_TX_ABRT_BITS 	BIT(6)

#define I2C_IC_CON		(0x00)
#define I2C_IC_TAR		(0x01)
#define I2C_IC_DATA_CMD		(0x4)
#define I2C_IC_SS_SCL_HCNT	(0x5)
#define I2C_IC_SS_SCL_LCNT	(0x6)
#define I2C_IC_RAW_INTR_STAT	(0xD)
#define I2C_IC_ENABLE		(0x1B)
#define I2C_IC_STATUS		(0x1C)
#define I2C_IC_RXFLR		(0x1E)


/* Adapted from Joe Abi Samara */

/* This does not work by the way. Not tested. */

void i2c1_write_MPU6050(uint8_t dev_addr, uint8_t dev_reg_addr, uint8_t data) {

    // check if I2C is idle and if TX FIFO is empty
    loop_until_bit_is_clear(I2C1_BASE[28], IC_1_IC_STATUS_ACTIVITY);
    loop_until_bit_is_set(I2C1_BASE[28], IC_1_IC_STATUS_TFE);

    // disable I2C and wait until it is disabled
    I2C1_BASE[27] = 0;
    loop_until_bit_is_clear(I2C1_BASE[39], IC_1_ENABLE_STATUS_IC_EN);

    // set target device address
    I2C1_BASE[1] = dev_addr;

    // enable I2C and wait until it is enabled
    I2C1_BASE[27] = 1;
    loop_until_bit_is_set(I2C1_BASE[39], IC_1_ENABLE_STATUS_IC_EN);

    // write register address (check to make sure TX FIFO is not full before writing)
    loop_until_bit_is_set(I2C1_BASE[28], IC_1_IC_STATUS_TFNF); 
    I2C1_BASE[4] = dev_reg_addr;

    // write data byte (check to make sure TX FIFO is not full before writing)
    // set the stop bit with the data_cmd to stop I2C transaction after successful write.
    loop_until_bit_is_set(I2C1_BASE[28], IC_1_IC_STATUS_TFNF); 
    I2C1_BASE[4] = data | IC_DATA_STOP_CMD;

    // wait until TX FIFO is empty (write complete)
    loop_until_bit_is_set(I2C1_BASE[28], IC_1_IC_STATUS_TFE);
    
}

void i2c1_read_MPU6050(uint8_t dev_addr, uint8_t dev_reg_addr, uint8_t *buffer, int len) {

    
    // check if I2C is idle and if TX FIFO is empty
    loop_until_bit_is_clear(I2C1_BASE[28], IC_1_IC_STATUS_ACTIVITY);
    loop_until_bit_is_set(I2C1_BASE[28], IC_1_IC_STATUS_TFE);

    // disable I2C and wait until it is disabled
    I2C1_BASE[27] = 0;
    loop_until_bit_is_clear(I2C1_BASE[39], IC_1_ENABLE_STATUS_IC_EN);
    
    // set the target device address
    I2C1_BASE[1] = dev_addr;

    // enable I2C and loop until it is enabled
    I2C1_BASE[27] = 1;
    loop_until_bit_is_set(I2C1_BASE[39], IC_1_ENABLE_STATUS_IC_EN);

    // write register address (check to make sure TX FIFO is not full before writing,
    // and check that it is empty before moving to reading)
    loop_until_bit_is_set(I2C1_BASE[28], IC_1_IC_STATUS_TFNF); 
    I2C1_BASE[4] = dev_reg_addr;
    loop_until_bit_is_set(I2C1_BASE[28], IC_1_IC_STATUS_TFE);

    for (int i = 0; i < len; ++i) {

        // Read command with read bit. if its the last read in the transaction, send the stop command
        if(i == (len - 1)) {
            I2C1_BASE[4] = (IC_DATA_READ_CMD) | (IC_DATA_RESTART_CMD) | (IC_DATA_STOP_CMD);
        }
        else {
            I2C1_BASE[4] = (IC_DATA_READ_CMD) | (IC_DATA_RESTART_CMD);
        }
        // loop until the RX FIFO is not empty (meaning we have received data)
        loop_until_bit_is_set(I2C1_BASE[28], IC_1_IC_STATUS_RXNE);

        // store the data
        buffer[i] = (I2C1_BASE[4] & 0xFF);
    }
    
}
