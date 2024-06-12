#ifndef DEBUG_H
#define DEBUG_H

// debug
// bool serial_debug_en          = 0;
bool debug_epd_en             = 1;
bool debug_mem_en             = 1;
bool debug_eeprom_en          = 1;

const bool DEBUG_ERROR_EN           = 1;
const bool DEBUG_WIFI_EN            = 1;
const bool DEBUG_MSG_EN             = 1;
const bool DEBUG_TIMER_EN           = 1;
const bool DEBUG_SUMMARY_EN         = 1;


#define DEBUG_EPD       2
#define DEBUG_ERROR     3
#define DEBUG_WIFI      4
#define DEBUG_MSG       5
#define DEBUG_TIMER     6
#define DEBUG_EEPROM    7
#define DEBUG_STEP      8
#define DEBUG_MEM       9
#define DEBUG_SUMMARY   10
#define DEBUG_OTA       11

void log_message(uint8_t debug_type, String str = "", bool printSameLine=false) {
    if (DEBUG_ENABLE) {
        String prefix ((char *)0);
        switch(debug_type){
            case DEBUG_EPD:
                prefix += "EPD";
                break;
            case DEBUG_ERROR:
                prefix += "ERROR";
                break;
            case DEBUG_WIFI:
                prefix += "WIFI";
                break;
            case DEBUG_MSG:
                prefix += "MSG";
                break;
            case DEBUG_TIMER:
                prefix += "TIMER";
                break;
            case DEBUG_EEPROM:
                prefix += "EEPROM";
                break;
            case DEBUG_MEM:
                prefix += "MEM";
                break;  
            case DEBUG_SUMMARY:
                prefix += "SUMMARY";
                break;  
            case DEBUG_OTA:
                prefix += "OTA";
                break;
        }
        if(printSameLine) {
            Serial.print(str);
        } else {
            Serial.print(prefix);
            Serial.print("\t:\t");
            Serial.println(str);
        }
        prefix.remove(0);
    } else {
        str = "";
    }
}

time_t timer_start() {
    unsigned int timer = millis();
    return timer; 
}

time_t timer_reset() {
    unsigned int timer = 0;
    return timer; 
}

time_t timer_stop(unsigned int timer) 
{
    return (millis() - timer);
}

#endif /* DEBUG_H */
