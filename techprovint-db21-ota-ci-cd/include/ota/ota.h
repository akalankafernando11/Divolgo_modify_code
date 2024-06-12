
#ifndef OTA_H
#define OTA_H  
#define CLOUD_FUNCTION_URL "https://us-central1-divolgo-jenkins.cloudfunctions.net/getDH40DownloadUrl" //Techprovint

String getDevId()
{
    byte mac[6];
    WiFi.macAddress(mac);
    // USE_SERIAL.println(devId);
    return String(mac[5], HEX) + String(mac[4], HEX) + String(mac[3], HEX) + String(mac[2], HEX) + String(mac[1], HEX) + String(mac[0], HEX);
}

String getDH40DownloadUrl()
{
    HTTPClient http;
    String downloadUrl;
    log_message(DEBUG_OTA, "[HTTP] begin");


timerWrite(timer_w, 0); 

    String url = CLOUD_FUNCTION_URL;
    url += String("?version=v") + CURRENT_VERSION;
    url += String("&variant=") + VARIANT;
    url += String("&devid=") + getDevId();

    log_message(DEBUG_OTA, "url:" + url);
    
    http.begin(url);


timerWrite(timer_w, 0); 

    int httpCode = http.GET(); // start connection and send HTTP header > httpCode will be negative on error


timerWrite(timer_w, 0); 
    
    if (httpCode > 0)
    {
        // HTTP header has been send and Server response header has been handled
        log_message(DEBUG_OTA, "[HTTP] GET code: " + String(httpCode));

    
timerWrite(timer_w, 0); 

        if (httpCode == HTTP_CODE_OK) { // file found at server
            String payload = http.getString();
            log_message(DEBUG_OTA, "payload:" + payload);
            downloadUrl = payload;
        }
        else
        {
            log_message(DEBUG_OTA, "Device up to date!");
            
            // add_white_background_data();
            // add_icon_data(Update_untodate, 150, 20);
            // add_icon_data(Batt_full, 300, 20);
            // display_image("imformation_display");
        }
    }
    else
    {
        log_message(DEBUG_ERROR, "Server con Failed.!");
        // led_blinking_ota_update_fail();
    }


timerWrite(timer_w, 0); 

    http.end();


timerWrite(timer_w, 0); 

    return downloadUrl;
}

/* 
 * Download binary image and use Update library to update the device.
 */
bool downloadUpdate(String url)
{
    HTTPClient http;
    // led_blinking_ota_update_receive();

    log_message(DEBUG_OTA, "[HTTP] Download begin");


timerWrite(timer_w, 0); 

    http.begin(url);


timerWrite(timer_w, 0); 
    // start connection and send HTTP header
    int httpCode = http.GET();


timerWrite(timer_w, 0); 

    if (httpCode > 0)
    {
        // HTTP header has been send and Server response header has been handled
        log_message(DEBUG_OTA, "[HTTP] GET... code: " + String(httpCode));

        // file found at server
        if (httpCode == HTTP_CODE_OK) {

            int contentLength = http.getSize();
            log_message(DEBUG_OTA, "content size:" + String(contentLength));

        
timerWrite(timer_w, 0); 

            if (contentLength > 0) {
                bool canBegin = Update.begin(contentLength);

            
timerWrite(timer_w, 0); 

                if (canBegin)
                {
                    WiFiClient stream = http.getStream();



timerWrite(timer_w, 0); // Reset watchdog timer 

                    log_message(DEBUG_OTA, "Begin update");
                    size_t written = Update.writeStream(stream);

                    if (written == contentLength) {
                        log_message(DEBUG_OTA, "Written : " + String(written) + " successfully");
                    }
                    else
                    {
                        log_message(DEBUG_OTA, "Written only : " + String(written) + "/" + String(contentLength) + ". Retry?");
                    }



timerWrite(timer_w, 0); // Reset watchdog timer 

                    if (Update.end()) {
                        log_message(DEBUG_OTA, "OTA done!");

    

timerWrite(timer_w, 0); // Reset watchdog timer 

                        if (Update.isFinished())
                        {
                            log_message(DEBUG_OTA, "Updated");
                            return true;
                        }
                        else
                        {
        

timerWrite(timer_w, 0); // Reset watchdog timer 

                            log_message(DEBUG_OTA, "Update failed");
                            return false;
                        }
                    }
                    else
                    {
    

timerWrite(timer_w, 0); // Reset watchdog timer 

                        log_message(DEBUG_OTA, "Error Occurred");
                        return false;
                    }
                }
                else
                {


timerWrite(timer_w, 0); // Reset watchdog timer 

                    log_message(DEBUG_OTA, "less storage");
                    client.flush();
                    return false;
                }
            }
            else {
            
timerWrite(timer_w, 0); 

                log_message(DEBUG_OTA, "Responce is empty");
                client.flush();
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}


bool check_for_updates()
    {
        if(battery_voltage < 3900) {
            add_white_background_data();
            add_icon_data(update, 100, 30);
            add_icon_data(warning, 112, 37);
            add_icon_data_battery_status(200, 30);
            add_icon_data(warning, 190, 32);
            display_image("imformation_display");
            delay(10);
            ESP.restart();
        }
        bool success = false;
        
        // led_blinking_ota_check();
        led_blinking_trying_to_connect();

        add_white_background_data();
        add_icon_data(update, 100, 30);
        add_icon_data(WiFi_Connecting, 40, 30);
        display_image("imformation_display");

        if (wifi_connect_for_ota()) {
            led_blinking_wifi_success();
            log_message(DEBUG_OTA, "\n\n ", true);
            log_message(DEBUG_OTA, "WiFi Ok");
            
        add_white_background_data();
        add_icon_data(update, 100, 30);
        add_icon_data(WiFi_OK, 40, 30);
        display_image("imformation_display");

           
            String version = String("Crrnt ver:") + String(CURRENT_VERSION);
            log_message(DEBUG_OTA, version);
            String downloadUrl = getDH40DownloadUrl();
            
            if (downloadUrl.length() > 0) {
                success = downloadUpdate(downloadUrl);

                if (!success)
                {
                    add_white_background_data();
                    add_icon_data(update, 100, 30);
                    add_icon_data(warning, 112, 37);
                    // add_icon_data(WiFi_Fail, 40, 30);
                    display_image("imformation_display");

                    return false;
                }
                else 
                {
                    add_white_background_data();
                    add_icon_data(update, 100, 30);
                    add_icon_data(right, 108, 40);
                    display_image("imformation_display");
                    delay(10);
                }
            }
        }
        else
        {
            log_message(DEBUG_OTA, "WiFi Fail");

            add_white_background_data();
            add_icon_data(update, 100, 30);
            add_icon_data(warning, 112, 37);
            add_icon_data(WiFi_Fail, 40, 30);
            display_image("imformation_display");

            led_blinking_wifi_fail();
            // return false;
        }
        
        // http.end();
        WiFi.disconnect(true);
        WiFi.mode(WIFI_OFF);
        
        return success;
    }
#endif /* OTA_H*/ 