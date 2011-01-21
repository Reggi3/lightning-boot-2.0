#ifndef LF1000_MMC_HH
#define LF1000_MMC_HH

/* Registers (as offsets from LF1000_SDIOn_BASE) */
#define SDI_CTRL	0x000
#define SDI_CLKDIV	0x008
#define SDI_CLKENA	0x010
#define SDI_TMOUT	0x014
#define SDI_CTYPE	0x018
#define SDI_BLKSIZ	0x01C
#define SDI_BYTCNT	0x020
#define SDI_INTMASK	0x024
#define SDI_CMDARG	0x028
#define SDI_CMD		0x02C
#define SDI_RESP0	0x030
#define SDI_RESP1	0x034
#define SDI_RESP2	0x038
#define SDI_RESP3	0x03C
#define SDI_MINTSTS	0x040
#define SDI_RINTSTS	0x044
#define SDI_STATUS	0x048
#define SDI_FIFOTH	0x04C
#define SDI_TCBCNT	0x05C
#define SDI_TBBCNT	0x060
#define SDI_DAT		0x100
#define SDI_SYSCLKENB	0x7C0
#define SDI_CLKGEN	0x7C4

/* SDI Control Register (CTRL) */
#define ABORT_DATA	8
#define SEND_IRQ_RESP	7
#define READ_WAIT	6
#define DMA_ENA		5
#define INT_ENA		4
#define DMARST		2
#define FIFORST		1
#define CTRLRST		0

/* SDI Clock Enable Register (CLKENA) */
#define LOWPWR		16
#define CLKENA		0

/* SDI Timeout Register (TMOUT) */
#define DTMOUT		8
#define RSPTMOUT	0

/* SDI Card Type Register (CTYPE) */
#define WIDTH		1	/* 0: 1-bit, 1: 4-bit */

/* SDI Interrupt Mask Register (INTMASK) */
#define MSKSDIOINT	16
#define MSKEBE		15
#define MSKACD		14
#define MSKSBE		13
#define MSKHLE		12
#define MSKFRUN		11
#define MSKHTO		10
#define MSKDRTO		9
#define MSKRTO		8
#define MSKDCRC		7
#define MSKRCRC		6
#define MSKRXDR		5
#define MSKTXDR		4
#define MSKDTO		3
#define MSKCD		2
#define MSKRE		1

/* SDI Command Register (CMD) */
#define STARTCMD	31	/* start command (don't set until arg ready) */
#define UPDATECLKONLY	21	/* update CLKDIV, CLKENA, don't send command */
#define CARDNUM		16	/* must be 0, bits 16:20 */
#define SENDINIT	15	/* send init sequence before sending command */
#define STOPABORT	14	/* stop or abort current transfer, if any */
#define WAITPRVDAT	13	/* wait for previous transfer completion */
#define SENDAUTOSTOP	12	/* send stop at end of transfer */
#define TRMODE		11	/* data transfer mode (0: block, 1: stream) */
#define RW		10	/* data transfer (read and write) expected */
#define DATEXP		9	/* data transfer (at least read) expected */
#define CHKRSPCRC	8	/* need to check CRC for response */
#define RSPLEN		7	/* expected response (0: short, 1: long) */
#define RSPEXP		6	/* expected response (0: no, 1: yes) */
#define CMDINDEX	0	/* command opcode */

/* SDI Masked Interrupt Status Register */
#define SDIOINT		16	/* SDIO interrupt */
#define EBEINT		15	/* end bit error (read) or no CRC (write) */
#define ACDINT		14	/* auto command done */
#define SBEINT		13	/* start bit error */
#define HLEINT		12	/* hardware locked error */
#define FRUNINT		11	/* FIFO underrun or overrun error */
#define HTOINT		10	/* data starvation by host timeout */
#define DRTOINT		9	/* data read timeout */
#define RTOINT		8	/* response timeout */
#define DCRCINT		7	/* data CRC error */
#define RCRCINT		6	/* response CRC error */
#define RXDRINT		5	/* RX FIFO data request */
#define TXDRINT		4	/* TX FIFO data request */
#define DTOINT		3	/* data transfer over */
#define CDINT		2	/* command done */
#define REINT		1	/* response error */

/* SDI Status Register (STATUS) */
#define DMAREQ		31
#define DMAACK		30
#define FIFOCOUNT	17
#define RSPINDEX	11
#define FSMBUSY		10
#define DATBUSY		9
#define CPRESENT	8
#define CMDFSM		4
#define FIFOFULL	3
#define FIFOEMPTY	2
#define TXWMARK		1
#define RXWMARK		0

/* SDI FIFO Threshold Watermark Register (FIFOTH) */
#define RXTH		16
#define TXTH		0

/* SDI Card Interface Control Register (CICTRL) */
#define CDETECT		2
#define WRTPRT		0

/* SDI System Clock Enable Register (SYSCLKENB) */
#define PCLKMODE	3
#define CLKGENENB	2

/* SDI Clock Generator Register (CLKGEN) */
#define CLKDIV0		4	/* bits 6:4 */
#define CLKSRCSEL0	1	/* bits 3:1, 0:PCLK, 1:PLL0, 2:PLL1 */

#endif
