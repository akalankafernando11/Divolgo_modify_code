#ifndef UTILS_H
#define UTILS_H

#include "config.h"
#include "debug.h"



void gpio_init() {
	digitalWrite(EPD_RST,   HIGH);
    pinMode(EPD_RST,    INPUT);
    digitalWrite(EPD_PWR_EN, LOW);
    pinMode(EPD_PWR_EN, 	OUTPUT);
    
    digitalWrite(LED_EN, 	HIGH);
	digitalWrite(SOFT_PWR_OFF, HIGH);

	pinMode(ON_BTN_SIG, 	INPUT);
	pinMode(BC_CHG_SIG, 	INPUT);
	pinMode(BC_PGOOD, 		INPUT);
	pinMode(USB_DETECT_SIG, 	INPUT);

	pinMode(LED_EN, 		OUTPUT);
    digitalWrite(LED_EN, 	HIGH);
	pinMode(LED_INT, 		INPUT);
	pinMode(SOFT_PWR_OFF, 	OUTPUT);

	pinMode(RTC_INT, 		INPUT);
	pinMode(BM_ALARM, 		INPUT);
    
	pinMode(TEMP_ALERT_1, 	INPUT);
	pinMode(TEMP_ALERT_2, 	INPUT);

    pinMode(SPI_CS, 		OUTPUT);
	digitalWrite(SPI_CS, 	LOW);

	delay_ms(20);
}

void power_off() {
    digitalWrite(LED_EN, HIGH);
    Wire.begin(I2C_SDA, I2C_SCL);
    
    led_blinking_power_off();
    
    // log_message(DEBUG_MSG, "Powering off");
    delay_ms(200);
    pinMode(SOFT_PWR_OFF, OUTPUT);
    digitalWrite(SOFT_PWR_OFF, LOW);
    delay_ms(100);
}

void power_off_check() {
    
    if (interruptCounter) {
        interruptCounter = 0;
        log_message(DEBUG_MSG, "Power Button press detected.!");
        if(!digitalRead(USB_DETECT_SIG)) {
        
            timerWrite(timer_w, 0); 
            battery_capacity = getCapacity();
            power_off_display_update();
            delay_ms(100);
            power_off();
        } else{
            write_value_to_eeprom(EEPROM_MEM_ADDR_DEVELOPER_MODE, "0");
            delay_ms(100);
            ESP.restart();
        }  
    } else if (powered_on_by_usb) {
        if(!digitalRead(USB_DETECT_SIG)) {
            power_off_display_update();
            delay_ms(100);
            power_off();
        }
    }
}
  
#endif /* UTILS_H */
