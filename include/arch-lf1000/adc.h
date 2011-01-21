/* LF1000 Analog to Digital Converter (ADC) Driver
 *
 * include/asm/arch/adc.h -- ADC settings and hardware abstrcation.
 *
 * Andrey Yurovsky <andrey@cozybit.com> */

#ifndef LF1000_ADC_H
#define LF1000_ADC_H

/*
 * Multiplexor Settings
 */
#ifdef CONFIG_MACH_LF_MP2530F
	#define LF1000_ADC_VBATSENSE	0
	#define LF1000_ADC_LEDVSENSE	1
	#define LF1000_ADC_VOLUMESENSE	2
	#define LF1000_ADC_UNUSED	3
	#define LF1000_ADC_MAX_CHANNEL	3
#elif defined CONFIG_MACH_ME_LF1000 || defined CONFIG_MACH_LF_LF1000
	#define LF1000_ADC_VBATSENSE	2
	#define LF1000_ADC_LEDVSENSE	0
	#define LF1000_ADC_VOLUMESENSE	1
	#define LF1000_ADC_UNUSED	3
	#define LF1000_ADC_MAX_CHANNEL	7
#else
#warning "Unknown platform variant: picking default ADC channels!"
	#define LF1000_ADC_VBATSENSE	0
	#define LF1000_ADC_LEDVSENSE	1
	#define LF1000_ADC_VOLUMESENSE	2
	#define LF1000_ADC_UNUSED	3
	#define LF1000_ADC_MAX_CHANNEL	3
#endif

/*
 * ADC Registers (offsets from LF1000_ADC_BASE) 
 */
#define ADCCON			0x00
#define ADCDAT			0x04
#define ADCINTENB		0x08
#define ADCINTCLR		0x0C
#define ADCCLKENB		0x40

/* ADC control register (ADCCON) */
#define APEN			14
#define APSV			6
#define ASEL			3
#define STBY			2
#define ADEN			0

/*
 * Driver API
 */
int adc_GetReading(u8 channel);

#endif
