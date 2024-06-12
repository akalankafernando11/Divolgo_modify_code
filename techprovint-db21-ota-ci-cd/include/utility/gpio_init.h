#ifndef GPIO_INIT_H
#define GPIO_INIT_H

#define SPI_CLK			18
#define SPI_MOSI		23
#define SPI_CS			12
#define SPI_MISO		19

#define I2C_SCL			22
#define I2C_SDA			21

#define EPD_PWR_EN		2
#define EPD_RST			4
#define EPD_BUSY_N		13

#define ON_BTN_SIG	 	32
#define ON_BTN_TRIG     GPIO_NUM_32
#define	BC_CHG_SIG		34 
#define BC_CHG_TRIG     GPIO_NUM_34
#define BC_PGOOD		35
#define USB_DETECT_SIG	14
#define USB_DETECT_TRIG     GPIO_NUM_14

#define LED_EN			15
#define LED_INT			25

#define SOFT_PWR_OFF 	27

#define RTC_INT			33
#define RTC_INT_TRIG    GPIO_NUM_33

#define BM_ALARM		26

#define TEMP_ALERT_1	37
#define TEMP_ALERT_2	38




#endif  /* GPIO_INIT_H */
