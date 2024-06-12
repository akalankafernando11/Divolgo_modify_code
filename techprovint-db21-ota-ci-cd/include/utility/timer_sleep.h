#ifndef TIMER_SLEEP_H
#define TIMER_SLEEP_H

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */

#include "config.h"
#include "debug.h"

// #define GPIO_BIT_MASK  ((1ULL<<RTC_INT_TRIG) | (1ULL<<RTC_INT_TRIG) | (1ULL<<RTC_INT_TRIG)) 
#define GPIO_BIT_MASK  ((1ULL<<ON_BTN_TRIG) | (1ULL<<ON_BTN_TRIG) | (1ULL<<ON_BTN_TRIG)) 
// #define GPIO_BIT_MASK  ((1ULL<<GPIO_NUM_32) | (1ULL<<GPIO_NUM_33)) 

void deep_sleep_upto_restart()
{
    log_message(DEBUG_MSG, "Device is going to deep sleep.  Need a manual restart to start running");
    esp_sleep_enable_ext0_wakeup(ON_BTN_TRIG, 1);
    // esp_sleep_enable_ext1_wakeup(GPIO_BIT_MASK, ESP_EXT1_WAKEUP_ALL_LOW);
    esp_sleep_pd_config(ESP_PD_DOMAIN_MAX, ESP_PD_OPTION_OFF);
    //esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    esp_deep_sleep_start();
}

void deep_sleep_while_charging(int TIME_TO_SLEEP)
{
    esp_sleep_enable_ext0_wakeup(BC_CHG_TRIG, RISING);
    esp_sleep_enable_ext1_wakeup(GPIO_BIT_MASK, ESP_EXT1_WAKEUP_ANY_HIGH);
    esp_sleep_pd_config(ESP_PD_DOMAIN_MAX, ESP_PD_OPTION_OFF);
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    esp_deep_sleep_start();
}

void deep_sleep_when_pluged_not_charging(int TIME_TO_SLEEP)
{
    log_message(DEBUG_MSG, "Device is going to deep sleep until pluged");
    esp_sleep_enable_ext0_wakeup(USB_DETECT_TRIG, 0);
    esp_sleep_enable_ext1_wakeup(GPIO_BIT_MASK, ESP_EXT1_WAKEUP_ANY_HIGH);
    esp_sleep_pd_config(ESP_PD_DOMAIN_MAX, ESP_PD_OPTION_OFF);    
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    esp_deep_sleep_start();
}

void deep_sleep(uint8_t TIME_TO_SLEEP)
{
    log_message(DEBUG_MSG, "going to deep sleep for " + String(TIME_TO_SLEEP) + " seconds ");
    // esp_sleep_enable_ext0_wakeup(RTC_INT_TRIG, 0);
    esp_sleep_enable_ext1_wakeup(GPIO_BIT_MASK, ESP_EXT1_WAKEUP_ANY_HIGH);
    esp_sleep_pd_config(ESP_PD_DOMAIN_MAX, ESP_PD_OPTION_OFF);
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
    esp_deep_sleep_start();
}

void deep_sleep_no_timer_wakeup()
{
    log_message(DEBUG_MSG, "Deep sleep until butten press or RTC wakeup");
    esp_sleep_enable_ext0_wakeup(RTC_INT_TRIG, 0);
    esp_sleep_enable_ext1_wakeup(GPIO_BIT_MASK, ESP_EXT1_WAKEUP_ANY_HIGH);
    esp_sleep_pd_config(ESP_PD_DOMAIN_MAX, ESP_PD_OPTION_OFF);
    esp_deep_sleep_start();
}

// void light_sleep(int TIME_TO_SLEEP)
// {
//     esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
//     log_message(DEBUG_MSG, "going to light sleep for " + String(TIME_TO_SLEEP) + " seconds ");
//     esp_light_sleep_start();
// }

#endif // TIMER_SLEEP_H
