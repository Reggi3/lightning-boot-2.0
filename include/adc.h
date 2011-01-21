#ifndef LIGHTNING_BOOT_ADC_H
#define LIGHTNING_BOOT_ADC_H

/*
 * model ADC as a line:
 * milliVolts = ((ADC_SLOPE * 256) * READING) / 256 + ADC_CONSTANT
 */

#if defined (CONFIG_MACH_ME_MP2530F) || defined (CONFIG_MACH_LF_MP2530F)
#define ADC_SLOPE_256           1943    /* ADC slope * 256              */
#define ADC_CONSTANT            1125    /* ADC constant                 */
#elif defined (CONFIG_MACH_ME_LF1000)
#define ADC_SLOPE_256           2322    /* ADC slope * 256              */
#define ADC_CONSTANT            1267    /* ADC constant                 */
#elif defined (CONFIG_MACH_LF_LF1000)
#define ADC_SLOPE_256           1957    /* ADC slope * 256              */
#define ADC_CONSTANT            190     /* ADC constant                 */
#else
#error board type undefined
#endif

/* ADC-to-millivolts, as per above */
#define ADC_TO_MV(r)	(((ADC_SLOPE_256*r)/256)+ADC_CONSTANT)

void adc_init(void);
u16 adc_get_reading(u8 channel);

#endif /* LIGHTNING_BOOT_ADC_H */
