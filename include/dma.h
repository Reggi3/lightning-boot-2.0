/*
 * LF1000 DMA API
 *
 * Brian Cavagnolo <brian@cozybit.com>
 *
 * linux/include/asm-arm/arch-lf1000/dma.h
 *
 * This is the custom DMA API for the LF1000.  Please read the comments in this
 * file and see the dmatest driver to understand how to use it.
 *
 * Based on  linux/include/asm-arm/arch-pxa/dma.h
 * Author:	Nicolas Pitre
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __ASM_ARCH_DMA_H
#define __ASM_ARCH_DMA_H

#include <linux/dma-mapping.h>
#include <asm/arch/platform.h>

/* DMA IDs for various LF1000 peripherals.  Use these in your lf1000_dma_desc 
 */
#define DMA_NOSUCHID	-1
#define DMA_UART0TX	0
#define DMA_UART0RX	1
#define DMA_UART1TX	2
#define DMA_UART1RX	3
#define DMA_UART2TX	4
#define DMA_UART2RX	5
#define DMA_UART3TX	6
#define DMA_UART3RX	7
#define DMA_UART4TX	8
#define DMA_UART4RX	9
#define DMA_UART5TX	10
#define DMA_UART5RX	11
#define DMA_USBEP1	12
#define DMA_USBEP2	13
#define DMA_USBEP3	14
#define DMA_USBEP4	15
#define DMA_SDRW	16
#define DMA_MPEGTS	17
#define DMA_SPI0TX	18
#define DMA_SPI0RX	19
#define DMA_SPI1TX	20
#define DMA_SPI1RX	21
#define DMA_SPI2TX	22
#define DMA_SPI2RX	23
#define DMA_PCMOUT	24
#define DMA_SPDIFOUT	25
#define DMA_PCMIN	26
#define DMA_MICIN	27
#define DMA_ADC1IN	28
#define DMA_ADC1OUT	29
#define DMA_MCUSTATIC0	31
#define DMA_MCUSTATIC1	32
#define DMA_I2C0	33
#define DMA_I2C1	34


/* Descriptor structure for LF1000's DMA engine. */
struct lf1000_dma_desc {
	volatile dma_addr_t src;
	volatile dma_addr_t dst;
	volatile unsigned long len; /* in bytes */
	volatile int id;
	volatile unsigned long flags;
};

/* flags for use with lf1000_dma_desc */
#define DMA_DST_NOINC		(1<<12)
#define DMA_DST_IO		(1<<10)
#define DMA_DST_MEM		(0<<10)
#define DMA_DST_8BIT		(0<<8)
#define DMA_DST_16BIT		(1<<8)
#define DMA_DST_32BIT		(2<<8)
#define DMA_SRC_NOINC		(1<<4)
#define DMA_SRC_IO		(1<<2)
#define DMA_SRC_MEM		(0<<2)
#define DMA_SRC_8BIT		(0<<0)
#define DMA_SRC_16BIT		(1<<0)
#define DMA_SRC_32BIT		(2<<0)

typedef enum {
	DMA_PRIO_HIGH = 0,
	DMA_PRIO_MEDIUM = 1,
	DMA_PRIO_LOW = 2,
	DMA_PRIO_LOWEST = 3
} lf1000_dma_prio;

#ifdef CPU_MF2530F
#define LF1000_DMA_CHANNELS 16
#elif defined CPU_LF1000
#define LF1000_DMA_CHANNELS 8
#else
#error "No CPU variant defined, can't determine number of DMA channels."
#endif

#ifdef CPU_MF2530F /* channel priority, MF2530: */
/* Channels 0-3 are HIGH priority, 4-7 are MED, and 8-15 are LOW
 */
#define lf1000_for_each_dma_prio(ch, prio)		\
	for (ch = prio * 4; ch != (4 << prio); ch++)
#elif defined CPU_LF1000 /* channel priority, LF1000 */
/* MagicEyes updated priority 9/13/2007
 * DMA 1, 5 High priority
 * DMA 2, 6
 * DMA 3, 7
 * DMA 0, 4 Low priority
 */

#define lf1000_for_each_dma_prio(ch, prio)		\
	for (ch = ((prio + 1) & 3); ch <= (((prio + 1) & 3) + 4); ch += 4)
#endif

/*
 * Request a dma channel.  Your irq_handler will be called if/when an interrupt
 * on your channel occurs.  data will be passed to your handler.  The return
 * value is either the channel number or -ENODEV if your request could not be
 * completed.
 */
int lf1000_dma_request(char *name,
		       lf1000_dma_prio prio,
		       void (*irq_handler)(int, void *, unsigned int),
		       void *data);

/* Free a dma channel requested with lf1000_dma_request.
 */
void lf1000_dma_free(int dma_ch);

/* initialize an lf1000_dma_desc.  You should call this to put your descriptor
 * in a proper default state.
 */
void lf1000_dma_initdesc(struct lf1000_dma_desc *desc);

/* launch the dma transaction.  If DMA cannot be scheduled, this function
 * returns -EAGAIN.  If the descriptor is bad, returns -EINVAL.
 */
int lf1000_dma_launch(int channel, struct lf1000_dma_desc *desc);

/* return 1 if the channel is busy, 0 otherwise.
 */
int lf1000_dma_busy(int channel);


void lf1000_dma_int_en(int channel);

void lf1000_dma_int_dis(int channel);

void lf1000_dma_int_clr(int channel);

/* This function is made available as a lazy shortcut hack!  Its purpose is to
 * allocate dma-capable buffers using dma_alloc_coherent.  That function
 * requires a struct device, which a simple dumb test driver like the dmatest
 * driver does not have.  In practice, you should use your own struct device.
 */
void *lf1000_dma_buf_alloc(size_t size, dma_addr_t *dma_handle);

/* Used to free buffers allocated with lf1000_dma_buf_alloc.
 */
void lf1000_dma_buf_free(size_t size, void *vaddr, dma_addr_t dma_handle);

#endif /* _ASM_ARCH_DMA_H */
