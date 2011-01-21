#ifndef _LF1000_POWER_H_
#define _LF1000_POWER_H_

#define ADC_MAX_BATTERY_MV	8000	/* max expected battery value	*/
#define ADC_LOW_BATTERY_MV	4100	/* low battery			*/
#define ADC_CRITICAL_BATTERY_MV 4000	/* critical low battery		*/

/* Hysteresis low to normal Battery */
#define ADC_NORMAL_BATTERY_MV   (ADC_LOW_BATTERY_MV + 400)

enum lf1000_power_status {
	UNKNOWN 		= 0,
	EXTERNAL		= 1,	/* on external power */
	BATTERY			= 2,	/* on battery power */
	LOW_BATTERY		= 3,
	CRITICAL_BATTERY	= 4,
};
#endif
