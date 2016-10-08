/*
 * AD7195.h
 *
 * Created: 2016-10-01 00:06:44
 *  Author: baretk
 */ 


#ifndef AD7195_H_
#define AD7195_H_

#include "SPI.h"
void AD7195_CS(uint8_t chipNo, bool state);
void AD7195_regwrite(uint8_t chipNo, uint8_t address, uint32_t value);
void AD7195_Init(uint8_t chipNo);
uint8_t AD7195_WhoIam(uint8_t ChipNo);
void AD7195_Sync(void);
void AD7195_Reset(uint8_t chipNo);
void AD7195_ContConvRead(uint8_t * channel1, uint8_t * channel2, uint32_t * value1, uint32_t * value2);
void AD7195_ContRead(uint8_t chipNo, bool enable);
bool AD7195_RDY(uint8_t chipNo);
void AD7195_ReadStore(allData_t * allData);
void AD7195_PressureCalc(AD7195_t * AD7195);

/* Registers */
#define AD7192_REG_COMM		0 /* Communications Register (WO, 8-bit) */
#define AD7192_REG_STAT		0 /* Status Register	     (RO, 8-bit) */
#define AD7192_REG_MODE		1 /* Mode Register	     (RW, 24-bit */
#define AD7192_REG_CONF		2 /* Configuration Register  (RW, 24-bit) */
#define AD7192_REG_DATA		3 /* Data Register	     (RO, 24/32-bit) */
#define AD7192_REG_ID		4 /* ID Register	     (RO, 8-bit) */
#define AD7192_REG_GPOCON	5 /* GPOCON Register	     (RO, 8-bit) */
#define AD7192_REG_OFFSET	6 /* Offset Register	     (RW, 16-bit
				   * (AD7792)/24-bit (AD7192)) */
#define AD7192_REG_FULLSALE	7 /* Full-Scale Register
				   * (RW, 16-bit (AD7792)/24-bit (AD7192)) */

/* Communications Register Bit Designations (AD7192_REG_COMM) */
#define AD7192_COMM_WEN		BIT(7) /* Write Enable */
#define AD7192_COMM_WRITE	0 /* Write Operation */
#define AD7192_COMM_READ	BIT(6) /* Read Operation */
#define AD7192_COMM_ADDR(x)	(((x) & 0x7) << 3) /* Register Address */
#define AD7192_COMM_CREAD	BIT(2) /* Continuous Read of Data Register */

/* Status Register Bit Designations (AD7192_REG_STAT) */
#define AD7192_STAT_RDY		BIT(7) /* Ready */
#define AD7192_STAT_ERR		BIT(6) /* Error (Overrange, Underrange) */
#define AD7192_STAT_NOREF	BIT(5) /* Error no external reference */
#define AD7192_STAT_PARITY	BIT(4) /* Parity */
#define AD7192_STAT_CH3		BIT(2) /* Channel 3 */
#define AD7192_STAT_CH2		BIT(1) /* Channel 2 */
#define AD7192_STAT_CH1		BIT(0) /* Channel 1 */

/* Mode Register Bit Designations (AD7192_REG_MODE) */
#define AD7192_MODE_SEL(x)	(((x) & 0x7) << 21) /* Operation Mode Select */
#define AD7192_MODE_SEL_MASK	(0x7 << 21) /* Operation Mode Select Mask */
#define AD7192_MODE_DAT_STA	BIT(20) /* Status Register transmission */
#define AD7192_MODE_CLKSRC(x)	(((x) & 0x3) << 18) /* Clock Source Select */
#define AD7192_MODE_SINC3	BIT(15) /* SINC3 Filter Select */
#define AD7192_MODE_ACX		BIT(14) /* AC excitation enable(AD7195 only)*/
#define AD7192_MODE_ENPAR	BIT(13) /* Parity Enable */
#define AD7192_MODE_CLKDIV	BIT(12) /* Clock divide by 2 (AD7190/2 only)*/
#define AD7192_MODE_SCYCLE	BIT(11) /* Single cycle conversion */
#define AD7192_MODE_REJ60	BIT(10) /* 50/60Hz notch filter */
#define AD7192_MODE_RATE(x)	((x) & 0x3FF) /* Filter Update Rate Select */

/* Mode Register: AD7192_MODE_SEL options */ 
#define AD7192_MODE_CONT		0 /* Continuous Conversion Mode */
#define AD7192_MODE_SINGLE		1 /* Single Conversion Mode */
#define AD7192_MODE_IDLE		2 /* Idle Mode */
#define AD7192_MODE_PWRDN		3 /* Power-Down Mode */
#define AD7192_MODE_CAL_INT_ZERO	4 /* Internal Zero-Scale Calibration */
#define AD7192_MODE_CAL_INT_FULL	5 /* Internal Full-Scale Calibration */
#define AD7192_MODE_CAL_SYS_ZERO	6 /* System Zero-Scale Calibration */
#define AD7192_MODE_CAL_SYS_FULL	7 /* System Full-Scale Calibration */

/* Mode Register: AD7192_MODE_CLKSRC options */
#define AD7192_CLK_EXT_MCLK1_2		0 /* External 4.92 MHz Clock connected
					   * from MCLK1 to MCLK2 */
#define AD7192_CLK_EXT_MCLK2		1 /* External Clock applied to MCLK2 */
#define AD7192_CLK_INT			2 /* Internal 4.92 MHz Clock not
					   * available at the MCLK2 pin */
#define AD7192_CLK_INT_CO		3 /* Internal 4.92 MHz Clock available
					   * at the MCLK2 pin */


/* Configuration Register Bit Designations (AD7192_REG_CONF) */

#define AD7192_CONF_CHOP	BIT(23) /* CHOP enable */
#define AD7192_CONF_REFSEL	BIT(20) /* REFIN1/REFIN2 Reference Select */
#define AD7192_CONF_CHAN(x)	(((1 << (x)) & 0xFF) << 8) /* Channel select */
#define AD7192_CONF_CHAN_MASK	(0xFF << 8) /* Channel select mask */
#define AD7192_CONF_BURN	BIT(7) /* Burnout current enable */
#define AD7192_CONF_REFDET	BIT(6) /* Reference detect enable */
#define AD7192_CONF_BUF		BIT(4) /* Buffered Mode Enable */
#define AD7192_CONF_UNIPOLAR	BIT(3) /* Unipolar/Bipolar Enable */
#define AD7192_CONF_GAIN(x)	((x) & 0x7) /* Gain Select */

#define AD7192_CH_AIN1P_AIN2M	0 /* AIN1(+) - AIN2(-) */
#define AD7192_CH_AIN3P_AIN4M	1 /* AIN3(+) - AIN4(-) */
#define AD7192_CH_TEMP		2 /* Temp Sensor */
#define AD7192_CH_AIN2P_AIN2M	3 /* AIN2(+) - AIN2(-) */
#define AD7192_CH_AIN1		4 /* AIN1 - AINCOM */
#define AD7192_CH_AIN2		5 /* AIN2 - AINCOM */
#define AD7192_CH_AIN3		6 /* AIN3 - AINCOM */
#define AD7192_CH_AIN4		7 /* AIN4 - AINCOM */

/* ID Register Bit Designations (AD7192_REG_ID) */
#define ID_AD7190		0x4
#define ID_AD7192		0x0
#define ID_AD7195		0x6
#define AD7192_ID_MASK		0x0F

/* GPOCON Register Bit Designations (AD7192_REG_GPOCON) */
#define AD7192_GPOCON_BPDSW	BIT(6) /* Bridge power-down switch enable */
#define AD7192_GPOCON_GP32EN	BIT(5) /* Digital Output P3 and P2 enable */
#define AD7192_GPOCON_GP10EN	BIT(4) /* Digital Output P1 and P0 enable */
#define AD7192_GPOCON_P3DAT	BIT(3) /* P3 state */
#define AD7192_GPOCON_P2DAT	BIT(2) /* P2 state */
#define AD7192_GPOCON_P1DAT	BIT(1) /* P1 state */
#define AD7192_GPOCON_P0DAT	BIT(0) /* P0 state */

#define AD7192_INT_FREQ_MHz	4915200

#endif /* AD7195_H_ */
