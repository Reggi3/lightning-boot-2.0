/* LF1000 I2C registers and hardware definitions */

#ifndef LF1000_I2C_H
#define LF1000_I2C_H

/* registers, as offsets from I2C_BASE */
#define ICCR			0x000
#define ICSR			0x004
#define IAR			0x008
#define IDSR			0x00C
#define QCNT_MAX		0x010
#define BURST_CTRL		0x014
#define BYTE_COUNT		0x018
#define BURST_STATUS		0x01C
#define BURST_INTERVAL		0x020
#define IRQ_PEND		0x024
#define I2C_CLKENB		0x100

/* I2C Control Register (ICCR) */
#define ACK_GEN				7
#define CLK_SRC				6
#define IRQ_ENB				5
#define EXT_IRQ_ENB			4
#define CLK_SCALER			0

/* I2C Status Register (ICSR) */
#define ST_ENB				12
#define BURST_FIN			11
#define SLAVE_MATCH_OCCUR		10
#define GENERAL_CALL_OCCUR		9
#define SLV_RX_STOP			8
#define MASTER_SLV			7
#define TX_RX				6
#define ST_BUSY				5
#define TXRX_ENB			4
#define ARBIT_FAIL			3
#define ACK_STATUS			0

/* QCNT_MAX */
#define CNT_MAX				0

/* IRQ Pending (IRQ_PEND) */
#define OP_HOLD				1
#define PEND				0

#endif

