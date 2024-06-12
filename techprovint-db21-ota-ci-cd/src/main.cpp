#include <Arduino.h>
#include <DNSServer.h>

#include <WebServer.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <Update.h>

#include <SPI.h>
#include <Wire.h>
#include <WiFi.h>
#include <rom/rtc.h>
#include <Preferences.h>

// #include <ArduinoJson.h>

#include "EEPROM.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
// #include "esp_system.h"
#include "soc/rtc.h"
#include "esp_bt.h"
#include "driver/adc.h"

#include <TimeLib.h>
WiFiClient client;
// WebServer server(80);
#define VARIANT "esp32"
#define CURRENT_VERSION VERSION

#include "config.h"

uint8_t spiffs_read_buffer[IMAGE_FILE_SIZE];
uint8_t image_buffer[8760];
uint8_t image_buffer1[8760];
String spiffs_read_string((char *)0);

time_t time_stamp;
uint8_t sleep_time_v;

unsigned int battery_voltage;
uint8_t battery_capacity;
bool usb_detect = false;
bool developer_mode = false;
bool new_connection_time = false;

bool usb_status = 0;

uint8_t conn_fail_attempts = 0;

esp_sleep_wakeup_cause_t wakeup_reason;
bool powered_on_by_usb = 0;
uint8_t pre_status = 0;

bool fresh_start_detection = 0;
RTC_DATA_ATTR unsigned int running_cycle = 0;
RTC_DATA_ATTR unsigned int rotation_number = 0;

uint16_t status_code = 0;

time_t active_time = 0;

Preferences preferences;

int wdtTimeout = 60000; //time in ms to trigger the watchdog
hw_timer_t *timer_w = NULL;

DRAM_ATTR int interruptCounter = 0;
DRAM_ATTR bool rtc_alarm_trig = 0;

String img_last((char *)0);
String img_curr((char *)0);
String last_event((char *)0);
String last_rotate_event((char *)0);
String last_action((char *)0);
String last_status((char *)0);
String rtn_status((char *)0);
String running_event((char *)0);

String localIP((char *)0);
String battery_status((char *)0);

TwoWire I2Cone = TwoWire(0);
TwoWire I2Ctwo = TwoWire(1);

#include "config.h"
//  general libraries
#include "utility/general_functions.h"

#include "utility/i2c_lib.h"

// Interface_LED_driver
#include "chip_libraries/led_driver.h"

// Real time clock  PCF85063
#include "chip_libraries/real_time_clock.h"

//  Battery monitor library
#include "chip_libraries/battery_monitor.h"

#include "utility/display_information.h"

#include "strings.h"

#include "utility/gpio_init.h"
#include "utility/debug.h"
#include "utility/memory.h"

//  Temperature sensor library TMP101NA/3K
// #include "chip_libraries/temp_sense.h"

//  plastic logic epaper libraries
#include "epd/epd_actions.h"

#include "utility/utils.h"

// Display options
#include "utility/display_options.h"


#include "utility/debug.h"

//  wifi and control libraries
#include "communication/ssl_certificate.h"
#include "communication/wifi_actions.h"
#include "utility/timer_sleep.h"

#include "ota/ota.h"

void run() { 
    // action_event_image_rotation(1);

    // display_image("/i/621/004308af1d574c9.jpg");
    // add_white_background_data();
    // add_icon_data(Batt_CH, 300, 20);
    // display_image("imformation_display");
    // add_icon_data(Batt_charging, 300, 20);


    // add_icon_data(Update_check, 300, 70);
    // add_icon_data(Update_inprogress, 300, 120);
    // add_icon_data(Update_undone, 300, 170);
    // add_icon_data(Update_done, 200, 20);

    // display_image("imformation_display");

    // add_icon_data(WiFi_fail, 20, 80);
    // add_icon_data(WiFi_OK, 110, 10);
    // add_icon_data(WiFi_sleep, 110, 80);

    // add_icon_data(Batt_20, 300, 70);
    // add_icon_data(Batt_40, 300, 120);
    // add_icon_data(Batt_60, 300, 170);
    // add_icon_data(Batt_80, 200, 20);
    // add_icon_data(Batt_100, 200, 70);
    // add_icon_data(Batt_charging, 200, 120);
    // add_icon_data(Batt_full, 200, 170);



    // delete_dir(SPIFFS, "/log");
    // delete_dir(SPIFFS, "/images");
    // delete_dir(SPIFFS, "/stats");

    // write_value_to_eeprom(EEPROM_MEM_ADDR_IMG_LAST, "none");
    // write_value_to_eeprom(EEPROM_MEM_ADDR_IMG_CURR, "none");
    // write_value_to_eeprom(EEPROM_MEM_ADDR_LST_ACTION, "none");
    // write_value_to_eeprom(EEPROM_MEM_ADDR_LST_EVENT, "none");
    // write_value_to_eeprom(EEPROM_MEM_ADDR_LST_ROTATE_EVENT, "none");
    // write_value_to_eeprom(EEPROM_MEM_ADDR_LST_STATUS, "none");
    // write_value_to_eeprom(EEPROM_MEM_IMG_ROTATION, "none");
    // write_value_to_eeprom(EEPROM_MEM_IMG_NUMBER, "none");
    // write_value_to_eeprom(EEPROM_MEM_ADDR_LST_ROTATE_EVENT_TMP, "none");

    // rotation_duration_str  += read_value_from_eeprom(EEPROM_MEM_ADDR_ROTATION_DURATION);

    img_last += read_value_from_eeprom(EEPROM_MEM_ADDR_IMG_LAST);
    // log_message(DEBUG_MSG, "Last Downloaded Image : " + img_last);

    img_curr += read_value_from_eeprom(EEPROM_MEM_ADDR_IMG_CURR);
    // log_message(DEBUG_MSG, "Current image : " + img_curr);

    last_event += read_value_from_eeprom(EEPROM_MEM_ADDR_LST_EVENT);
    log_message(DEBUG_MSG, "Last Event : " + last_event);

    last_rotate_event += read_value_from_eeprom(EEPROM_MEM_ADDR_LST_ROTATE_EVENT);
    log_message(DEBUG_MSG, "Last Rotate Event : " + last_rotate_event);

    last_action += read_value_from_eeprom(EEPROM_MEM_ADDR_LST_ACTION);
    log_message(DEBUG_MSG, "Last Action : " + last_action);

    last_status += read_value_from_eeprom(EEPROM_MEM_ADDR_LST_STATUS);
    log_message(DEBUG_MSG, "Last Status : " + last_status);

    running_event += read_value_from_eeprom(EEPROM_MEM_ADDR_RUNNUNG_EVENT);
    log_message(DEBUG_MSG, "Running event : " + running_event);

    // log_message(DEBUG_MSG, "fresh_start_detection : " + String(fresh_start_detection));
    power_off_check();

    conn_fail_attempts = preferences.getInt("key2");

    bool connect_wifi = 0;

    if(rtc_alarm_trig){
        connect_wifi = 1;
    } else {
        led_blinking_conection_sleep();
    }

    //////////////////////////////////////////////////////////////////////// Rotation ///////////////////////////////
    bool rotation = true;

    while(rotation) {
        if(running_event != "0") {
            if(fresh_start_detection) {
                if(read_value_from_eeprom(EEPROM_MEM_ROTATION_EN) == "1") {
                    rotation_number = 1;
                }
                add_white_background_data();
                add_icon_data_battery_status(180, 20);
                add_icon_data(Task_Running, 140, 18);
                add_icon_data(WiFi_Connecting, 20, 20);
                display_image("imformation_display");
                break;
            }
        } else {
            add_white_background_data();
            add_icon_data_battery_status(180, 20);
            add_icon_data(No_Task, 140, 18);
            if(connect_wifi) {
                add_icon_data(WiFi_Connecting, 20, 20);
            } else {
                add_icon_data(WiFi_Sleep, 20, 20);
            }
            display_image("imformation_display");
            break;
        }
        String current_image_number_str((char *)0);
        String last_rotate_event_tmp((char *)0);
        // current_image_number_str += read_value_from_eeprom(EEPROM_MEM_IMG_ROTATION);
        last_rotate_event_tmp += read_value_from_eeprom(EEPROM_MEM_ADDR_LST_ROTATE_EVENT_TMP);

        // rotation_image_number = current_image_number_str.toInt();
        current_image_number_str.remove(0);

        power_off_check();

        if(AUTO_ROTATION_STOP){ // If this enabled rotation will be stopped after Sunday midnight
            if(rtc_get_day_of_week() == 2) {
                String rotation_start_date_string = read_value_from_eeprom(EEPROM_MEM_ROTATION_ON_DATE);
                
                uint16_t rotation_start_date = rotation_start_date_string.toInt();
                uint16_t day_of_year_today = rtc_day_of_year();
                
                if((day_of_year_today-rotation_start_date) > 2) {
                    log_message(DEBUG_MSG, "Rotation clearing");
                    rotation_number = 1;
                    action_event_image_rotation(last_rotate_event, rotation_number);
                    rotation_number = 0;                              // Stop rotation
                    write_value_to_eeprom(EEPROM_MEM_ADDR_RUNNUNG_EVENT, "0");
                }
            }
        }

        if (last_rotate_event_tmp != last_rotate_event) {
            rotation_number = 1;
            write_value_to_eeprom(EEPROM_MEM_ADDR_LST_ROTATE_EVENT_TMP, last_rotate_event);
        }
        last_rotate_event_tmp.remove(0);

        if (rotation_number) {
            action_event_image_rotation(last_rotate_event, rotation_number);
        }
        break;
    }

    //////////////////////////////////////////////////////////////////////// end Rotation ///////////////////////////////

    update_status_code(1); // None

    // Check forpower butten press counts
    power_off_check();

    if (connect_wifi) {
        led_blinking_trying_to_connect();
        if (wifi_connect()) {
            led_blinking_wifi_success();

            power_off_check(); // Check forpower butten press counts

            push_pull();

            if(fresh_start_detection == 1) {
                // clear_icon_data(100, 24, 30, 24);
                add_icon_data(WiFi_OK, 20, 20);
                if(status_code == 1200) {
                    add_icon_data(Server_error, 60, 20);
                }
                display_image("imformation_display");
            } else if(running_event == "0") {
              // clear_icon_data(100, 24, 30, 24);
              add_icon_data(WiFi_OK, 20, 20);
              if(status_code == 1200) {
                  add_icon_data(Server_error, 60, 20);
              }
              display_image("imformation_display");
            }

            timerWrite(timer_w, 0);  // Reset watchdog timer

            last_action.remove(0);
            last_action += read_value_from_eeprom(EEPROM_MEM_ADDR_LST_ACTION);

            // if (AUTO_FORMAT)
            // {
            //   if (status_code == 3318 || status_code == 3320)
            //   {
            //     // delete_dir(SPIFFS, "/log");
            //     // delete_dir(SPIFFS, "/log_batt");
            //     delete_dir(SPIFFS, IMG_PATH);
            //   }
            // }

            // if (AUTO_IMAGE_CHANGE)
            // {

            //   String tmp_image_name = IMG_PATH + img_last;

            //   if (status_code == 0)
            //   {
            //     if (!display_image(tmp_image_name))
            //     {
            //       display_status = "FL";
            //     }
            //     else
            //     {
            //       display_status = "OK";
            //     }
            //     tmp_image_name.remove(0);
            //   }
            // }
        } else {
            led_blinking_wifi_fail();
            if(fresh_start_detection == 1) {
                clear_icon_data(60, 24, 20, 24);
                add_icon_data(WiFi_Fail, 20, 20);
                display_image("imformation_display");
            }
            else if(running_event == "0") {
                clear_icon_data(60, 24, 20, 24);
                add_icon_data(WiFi_Fail, 20, 20);
                display_image("imformation_display");
            }
        }
    }

    power_off_check();

    log_message(DEBUG_MSG, "status_code > " + String(status_code));

    switch (status_code) {
        case 1:
          rtn_status += "none";
          conn_fail_attempts = 0;
          break;
        case 2: // Change success
        case 3: // Downaload success
        case 4: // Delete success
        case 5: // Format success
        case 6: // Rotate success
        case 7: // File_count success
        case 8: // Display clear (white) success
        case 9: // Display clear (black) success
          conn_fail_attempts = 0;
          rtn_status += ACT_SUCESS;
          break;
        case 1100:
        case 1200:
        case 1300:
        case 1401:
        case 1402:
          conn_fail_attempts++;
          rtn_status += read_value_from_eeprom(EEPROM_MEM_ADDR_LST_STATUS);
          break;
        default:
          rtn_status += "error_";
          rtn_status += String(status_code);
    }

    log_message(DEBUG_MSG, "STAT CODE: " + String(status_code));
    log_message(DEBUG_MSG, "RTN_STATUS: " + rtn_status);

    write_value_to_eeprom(EEPROM_MEM_ADDR_LST_STATUS, rtn_status);
}

volatile byte state = LOW;

unsigned long button_time = 0;
unsigned long button_off_time = 0;

void IRAM_ATTR PWR_BT_INT_LOW() {
    button_off_time = millis() - button_time;
    if(button_off_time > 2000) {
        ESP.restart();
    }
}

//  Power button interrupt function
void IRAM_ATTR PWR_BT_INT() {
    button_time = millis();
    digitalWrite(LED_EN, LOW);
    interruptCounter++;
    attachInterrupt(digitalPinToInterrupt(ON_BTN_SIG), PWR_BT_INT_LOW, FALLING);
    // detachInterrupt(32);
}

//  Power button interrupt function
void IRAM_ATTR USB_DETECT_INT() {
    digitalWrite(SOFT_PWR_OFF, LOW);
}

// Watchdog iiteruupt function
void IRAM_ATTR resetModule() {
    log_message(DEBUG_MSG, "Reboot\n");
    ESP.restart();
}

// void print_wakeup_reason() {
//     esp_sleep_wakeup_cause_t wakeup_reason;

//     wakeup_reason = esp_sleep_get_wakeup_cause();

//     switch(wakeup_reason) {
//         case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
//         case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
//         case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
//         case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
//         case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
//         default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
//     }

// }

/////////////////////////////////////////////////////
/*
reset_reason
1  = hard reset
5  = deepsleep reset
20 = powerd on by on butten press
21 = powered on by usb charger connects
22 = wakeup from sleep by butten press 
23 = wakup from sleep by usb unplug 
*/
//////////////////////////////////////////////////////

void setup() {
    active_time = millis();
    
    if(running_cycle == 0) {
        fresh_start_detection = 1;
    }
    running_cycle++;

    ///////////////////////////////////////////////////////////////////////////////// watchdow dog setup /////////////////////////////////////
    timer_w = timerBegin(0, 80, true);                  // timer 0, div 80
    timerAttachInterrupt(timer_w, &resetModule, true);  // attach callback
    timerAlarmWrite(timer_w, wdtTimeout * 1000, false); // set time in us
    timerAlarmEnable(timer_w);                          // /enable interrupt
    ///////////////////////////////////////////////////////////////////////////////// end watchdow dog setup /////////////////////////////////////

    gpio_init();
    delay_ms(10);

    if (!initialize_eeprom()) {
        // device_error();
        log_message(DEBUG_ERROR, "EEPROM Initialization falied.");
    }
    if (!initialize_spiffs()) {
        // device_error();
        log_message(DEBUG_ERROR, "spiffs mount failed");
    }

    if(digitalRead(USB_DETECT_SIG)) {
        DEBUG_ENABLE = true;   
    } else if(read_int_value_from_eeprom(EEPROM_MEM_ADDR_DEVELOPER_MODE)) {
        DEBUG_ENABLE = true;
    }
    if(DEBUG_ENABLE) {
        Serial.begin(115200);
    }

    Wire.begin(I2C_SDA, I2C_SCL);
    delay_ms(10);

    // led_blinking_battery_level(100);

    rtc_begin();
    time_stamp = rtc_get_time();

    timerWrite(timer_w, 0); //reset timer (feed watchdog)

    if (initBattMon()) {
        battery_voltage = getVoltage();
        battery_capacity = getCapacity();
    }

    if (DEBUG_ENABLE) {
        time_stamp = rtc_get_time();

        log_message(DEBUG_SUMMARY, ">>>>>>>>>>>>>  ", true);
        rtc_print_time();
        log_message(DEBUG_SUMMARY, "ver:" + String(CURRENT_VERSION));
        log_message(DEBUG_SUMMARY, "dev_id:" + get_mac_address());
        log_message(DEBUG_SUMMARY, "ota_id:" + device_id());
        log_message(DEBUG_SUMMARY, "Running cycle: " + String(running_cycle));
        log_message(DEBUG_SUMMARY, "time_stamp:" + String(time_stamp));
        log_message(DEBUG_MSG, "Vbatt = " + (String)battery_voltage + "mV");
        log_message(DEBUG_MSG, "Cbatt = " + (String)battery_capacity + "%");
        log_message(DEBUG_SUMMARY, "batt:" + battery_status);
        log_message(DEBUG_MSG, "Connection timer flag = " +String(rtc_alarm_trig));
        log_message(DEBUG_SUMMARY, "<<<<<<<<<<<<<<<<<<<<");

        log_message(DEBUG_MSG, "Press 'D' for device options");
        for(uint8_t count = 0; count < 4; count++) {
            log_message(DEBUG_MSG, ".", true);
            delay_ms(500);
            display_options();
        }
    }

    timerWrite(timer_w, 0);  // Reset watchdog timer

    preferences.begin("on_status", false);

    wakeup_reason = esp_sleep_get_wakeup_cause();
    powered_on_by_usb = digitalRead(USB_DETECT_SIG);
    pre_status = preferences.getUInt("on_status", 0);
    developer_mode = read_int_value_from_eeprom(EEPROM_MEM_ADDR_DEVELOPER_MODE);

    preferences.putUInt("on_status", 0);

    power_off_check();

    rtc_alarm_trig = (fresh_start_detection == 1) ? true : false;

    if (rtc_read_timer_flag()) {
        rtc_alarm_trig = true;
    }

    if (!powered_on_by_usb) {  // No charger detected
        write_value_to_eeprom(EEPROM_MEM_ADDR_DEVELOPER_MODE, "0");
        if(battery_voltage < VBATT_CUTOFF) {
            if (rotation_number == 0) {
                power_off_display_update();
            } 
            delay(3000);
            power_off();
        }
        if(fresh_start_detection) {
            if(battery_voltage < VBATT_MIN) {
                led_blinking_low_battery_power_off();
                power_off_display_update();
                power_off();
            }
        }
    }

    if (powered_on_by_usb) {  // charger connected
  
        power_off_check();

        attachInterrupt(digitalPinToInterrupt(USB_DETECT_SIG), USB_DETECT_INT, LOW);

        uint8_t count = 0;
        while(digitalRead(ON_BTN_SIG)) {
            count++;
            delay(1000);
            if(count > 5) {
                check_for_updates();
                break;
            }
        }
        
        /////////////////////////////////////////// Charge indicationg ///////////////////////////
        bool charging = !digitalRead(BC_CHG_SIG);

        if (charging) {
            led_blinking_charging();
            log_message(DEBUG_MSG, "Charging.");

            add_white_background_data();
            add_icon_data(Batt_00, 180, 20);
            add_icon_data(Batt_CH, 190, 22);
            display_image("imformation_display");
        }
        else {
            led_blinking_full_charge();
            log_message(DEBUG_MSG, "Charged.");

            add_white_background_data();
            add_icon_data(Batt_100, 180, 20);
            add_icon_data(Batt_CH_FL, 190, 22);
            display_image("imformation_display");
        }

        //////////////////////////////////////////// Serial log /////////////////////////////////

        power_off_check();
                   
        preferences.putUInt("on_status", 2);

        if (read_int_value_from_eeprom(EEPROM_MEM_ADDR_DEVELOPER_MODE)) {
            log_message(DEBUG_MSG, "Developer mode|on");
        } else {
            log_message(DEBUG_MSG, "Developer mode|off");

            if ((pre_status == 2) & (wakeup_reason == 3)) {
                if (read_int_value_from_eeprom(EEPROM_MEM_ADDR_DEVELOPER_MODE)) {
                    log_message(DEBUG_MSG, "Restarting with developer mode");
                    ESP.restart();
                }
                log_message(DEBUG_MSG, "\n", 1);
            }
            power_off_check();

            log_message(DEBUG_MSG, "Going sleep");
            if (charging) {
                deep_sleep_while_charging(600);
            } else {
                deep_sleep_when_pluged_not_charging(600);
            }
        }
    } else if ((pre_status == 2) & (wakeup_reason == 2)) { 
        // Charger unplugged
        log_message(DEBUG_MSG, "pwr_off|USB_removed 2");
        power_off_display_update();
        power_off(); 
    } else if ((pre_status == 1) & (wakeup_reason == 3)) {
        // Power buttern press detected while sleep
        battery_status.remove(0);
        log_message(DEBUG_MSG, "pwr_off|btn_dtct");
        power_off_display_update();
        power_off();  
    }

    // Power butten detect intterupt initialization
    attachInterrupt(digitalPinToInterrupt(ON_BTN_SIG), PWR_BT_INT, HIGH);

    // Battery remaining capacity indicating by Power LEDs. (Red and White)
    led_blinking_battery_level(battery_capacity);

    timerWrite(timer_w, 0);  

    power_off_check();  // Check for power butten press counts

    if (battery_voltage > 9999) { // battery voltage set to zero when it wrong
        battery_voltage = 0;
    }
    
    timerWrite(timer_w, 0);  // Reset watchdog timer
}

void loop() {
    
    power_off_check(); // Check for power butten press counts

    run();

    power_off_check(); // Check forpower butten press counts

    String image_path((char *)0);
    image_path += IMG_PATH;
    image_path += "/";
    image_path += last_rotate_event;
    int image_count = count_files(SPIFFS, image_path);
    image_path.remove(0);
    
    battery_status += " | " + (String)battery_voltage + "mV | " + (String)battery_capacity + "%";

    timerWrite(timer_w, 0); // Reset watchdog timer

    time_t connection_time = read_value_from_eeprom(EEPROM_MEM_ADDR_CONN_TIME).toInt();
    time_t rotation_time = read_value_from_eeprom(EEPROM_MEM_ADDR_ROTATION_DURATION).toInt();
    running_event = read_value_from_eeprom(EEPROM_MEM_ADDR_RUNNUNG_EVENT);

    bool timer_sleep_enable = false;

    if(running_event != "0") {
        // connection_time = (conn_fail_attempts > 0) ? DEFAULT_CONNECTION_TIME : connection_time;
        timer_sleep_enable = true;
    } else {
        connection_time = (conn_fail_attempts > 0) ? DEFAULT_CONNECTION_TIME : connection_time;
        timer_sleep_enable = (connection_time > DEFAULT_CYCLE_TIME) ? false : true;
    }

    connection_time = (connection_time < DEFAULT_CYCLE_TIME) ? DEFAULT_CONNECTION_TIME : connection_time;
    connection_time = (connection_time > MAXIMUM_CONNECTION_TIME) ? DEFAULT_CONNECTION_TIME : connection_time;

    if ((last_action != "none") || (new_connection_time == true)) {
        connection_time = DEFAULT_SLEEP_TIME;
    } 

    if(rtc_alarm_trig) {
        rtc_clear_timer_flag();
        rtc_set_timer(connection_time);
    } 
    
    rotation_time = (rotation_time > MAXIMUM_CONNECTION_TIME) ? DEFAULT_CYCLE_TIME : rotation_time;
    rotation_time = (rotation_time < DEFAULT_CYCLE_TIME)   ? DEFAULT_CYCLE_TIME : rotation_time;

    write_value_to_eeprom(EEPROM_MEM_ADDR_ROTATION_DURATION, String(rotation_time));
  
    if (fresh_start_detection) {
        rotation_time   = DEFAULT_CYCLE_TIME;
    } 
    active_time = millis();

    sleep_time_v = rotation_time - (active_time / 1000);

    sleep_time_v = (sleep_time_v > rotation_time) ? 20 : sleep_time_v;
    sleep_time_v = (sleep_time_v == 0) ? 1 : sleep_time_v;

    power_off_check();

    if (DEBUG_ENABLE) {
        time_stamp = rtc_get_time();

        log_message(DEBUG_SUMMARY, ">>>>>>>>>>>>>  ", true);
        rtc_print_time();
        log_message(DEBUG_SUMMARY, "ver:" + String(CURRENT_VERSION));
        log_message(DEBUG_SUMMARY, "dev_id:" + get_mac_address());
        log_message(DEBUG_SUMMARY, "ota_id:" + device_id());
        log_message(DEBUG_SUMMARY, "Running cycle: " + String(running_cycle));
    
        if (rtc_alarm_trig) {
            log_message(DEBUG_SUMMARY, "WiFi:" + wifi_signal + "dB");
            log_message(DEBUG_SUMMARY, "ip:" + localIP);
        }
    
        log_message(DEBUG_SUMMARY, "time_stamp:" + String(time_stamp));
        log_message(DEBUG_SUMMARY, "batt:" + battery_status);
        log_message(DEBUG_SUMMARY, "crnt_image:" + img_curr);
        log_message(DEBUG_SUMMARY, "last_image:" + img_last);
        log_message(DEBUG_SUMMARY, "rotation:" + String(rotation_number) + "/" + String(image_count));
        log_message(DEBUG_SUMMARY, "last_event:" + last_event);
        log_message(DEBUG_SUMMARY, "rotate_event:" + last_rotate_event);
        log_message(DEBUG_SUMMARY, "action:" + last_action);
        log_message(DEBUG_SUMMARY, "status:" + rtn_status);
        log_message(DEBUG_SUMMARY, "stat_code:" + String(status_code));
        log_message(DEBUG_SUMMARY, "active_time:" + String(active_time) + "ms");
        log_message(DEBUG_SUMMARY, "Sleep_time:" + String(sleep_time_v) + "s"); 
        log_message(DEBUG_SUMMARY, "rotate_dur:" + String(rotation_time) + "s"); 
        log_message(DEBUG_SUMMARY, "con_dur:" + String(connection_time) + "s");
        log_message(DEBUG_SUMMARY, "con_attemps:" + String(conn_fail_attempts));
        log_message(DEBUG_SUMMARY, "<<<<<<<<<<<<<<<<<<<<");
    }
  

    // Check forpower butten press counts
    power_off_check();

    preferences.putInt("key2", conn_fail_attempts);
    preferences.putUInt("on_status", 1);
    preferences.end();
    btStop();
    esp_bt_controller_disable();

    adc_power_off();

    if (timer_sleep_enable) {
        deep_sleep(sleep_time_v);
    } else {
        add_white_background_data();
        add_icon_data_battery_status(180, 20);
        add_icon_data(No_Task, 140, 18);
        add_icon_data(WiFi_Sleep, 20, 20);
        display_image("imformation_display");
        delay(100);
        deep_sleep_no_timer_wakeup();
    }
}
