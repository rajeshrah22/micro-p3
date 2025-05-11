#ifndef MPU6050_H__
#define MPU6050_H__

#include <stdint.h>

struct i2c {
	volatile unsigned IC_CON;	
	volatile unsigned IC_TAR;
	volatile unsigned IC_SAR;
	volatile unsigned padding1;
	volatile unsigned IC_DATA_CMD;	
	volatile unsigned IC_SS_SCL_HCNT;
	volatile unsigned IC_SS_SCL_LCNT;
	volatile unsigned IC_FS_SCL_HCNT;
	volatile unsigned IC_FS_SCL_LCNT;
	volatile unsigned padding2[2];
	volatile unsigned IC_INTR_STAT;
	volatile unsigned IC_INTR_MASK;
	volatile unsigned IC_RAW_INTR_STAT;
	volatile unsigned IC_RX_TL;
	volatile unsigned IC_TX_TL;
	volatile unsigned IC_CLR_INTR;
	volatile unsigned IC_CLR_RX_UNDER;
	volatile unsigned IC_CLR_RX_OVER;
	volatile unsigned IC_CLR_TX_OVER;
	volatile unsigned IC_CLR_RD_REQ;
	volatile unsigned IC_CLR_TX_ABRT;
	volatile unsigned IC_CLR_RX_DONE;
	volatile unsigned IC_CLR_ACTIVITY;
	volatile unsigned IC_CLR_STOP_DET;
	volatile unsigned IC_CLR_START_DET;
	volatile unsigned IC_CLR_GEN_CALL;
	volatile unsigned IC_ENABLE;
	volatile unsigned IC_STATUS;
	volatile unsigned IC_TXFLR;
	volatile unsigned IC_RXFLR;
	volatile unsigned IC_SDA_HOLD;
	volatile unsigned IC_TX_ABRT_SOURCE;
	volatile unsigned IC_SLV_DATA_NACK_ONLY;
	volatile unsigned IC_DMA_CR;
	volatile unsigned IC_DMA_TDLR;
	volatile unsigned IC_DMA_RDLR;
	volatile unsigned IC_SDA_SETUP;
	volatile unsigned IC_ACK_GENERAL_CALL;
	volatile unsigned IC_ENABLE_STATUS;
	volatile unsigned IC_FS_SPKLEN;
	volatile unsigned padding3;
	volatile unsigned IC_CLR_RESTART_DET;
	volatile unsigned padding4[19];
	volatile unsigned IC_COMP_PARAM_1;
	volatile unsigned IC_COMP_VERSION;
	volatile unsigned IC_COMP_TYPE;	
};

/* I2C1 CON */
#define IC_1_CON_MASTER_MODE_MASK 		(0)
#define IC_1_CON_SPEED_STANDARD 		((1) << (1))
#define IC_1_CON_SPEED_FAST_MODE		((2) << (1))
#define IC_1_CON_SPEED_HIGH_SPEED		((3) << (1))
#define IC_1_CON_IC_RESTART_EN_MASK		(5)
#define IC_1_CON_IC_SLAVE_DISABLE		(6)



/* I2C_TAR */

#define IC_MPU6050_AD0_VDD_ADDRESS 		(0b1101001)
#define IC_MPU6050_AD0_GND_ADDRESS 		(0b1101000)

/* I2C DATA CMD*/

#define IC_DATA_READ_CMD				((1) << (8))
#define IC_DATA_WRITE_CMD				((0) << (8))
#define IC_DATA_RESTART_CMD				(10)
#define IC_DATA_STOP_CMD				((1) << (9))

/* I2C Enable Status */

#define IC_1_ENABLE_STATUS_IC_EN		(0)

/* I2C STATUS */

#define IC_1_IC_STATUS_ACTIVITY			(0)
#define IC_1_IC_STATUS_TFNF				(1)
#define IC_1_IC_STATUS_TFE				(2)
#define IC_1_IC_STATUS_RXNE				(3)

#define MPU_DATA_BUF_SIZE	2

void i2c1_read_MPU6050(uint8_t dev_addr, uint8_t dev_reg_addr, uint8_t *buffer, int len);

void i2c1_write_MPU6050(uint8_t dev_addr, uint8_t dev_reg_addr, uint8_t data);

#endif
