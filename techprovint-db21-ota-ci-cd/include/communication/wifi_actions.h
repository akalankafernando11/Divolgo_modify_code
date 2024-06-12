#ifndef WIFI_ACTIONS_H
#define WIFI_ACTIONS_H

String wifi_signal((char *)0);

const String wifi_status_to_str(wl_status_t wlStatus)
{
    switch (wlStatus)
    {
    case WL_NO_SHIELD:
        return "WL_NO_SHIELD";
    case WL_IDLE_STATUS:
        return "WL_IDLE_STATUS";
    case WL_NO_SSID_AVAIL:
        return "WL_NO_SSID_AVAIL";
    case WL_SCAN_COMPLETED:
        return "WL_SCAN_COMPLETED";
    case WL_CONNECTED:
        return "WL_CONNECTED";
    case WL_CONNECT_FAILED:
        return "WL_CONNECT_FAILED";
    case WL_CONNECTION_LOST:
        return "WL_CONNECTION_LOST";
    case WL_DISCONNECTED:
        return "WL_DISCONNECTED";
    default:
        return "Unknown";
    }
}

String get_mac_address()
{
    byte mac[6];
    WiFi.macAddress(mac);
    return (String)mac[5] + (String)mac[4] + (String)mac[3] + (String)mac[2] + (String)mac[1] + (String)mac[0];
}

String device_id()
{
    byte mac[6];
    WiFi.macAddress(mac);
    return String(mac[5], HEX) + String(mac[4], HEX) + String(mac[3], HEX) + String(mac[2], HEX) + String(mac[1], HEX) + String(mac[0], HEX);
}

bool wifi_connect_for_ota()
{
    bool status = 1;
    uint8_t temp = 0;

    //  log_message(DEBUG_MSG, "Connecting WiFi. " + (String)WIFI_SSID);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay_ms(500);
        log_message(DEBUG_WIFI, " . ", true);

        timerWrite(timer_w, 0); // Reset watchdog timer

        temp++;
        if (temp >= WIFI_RETRY_COUNT) {
            status = 0;
            break;
        }
    }

    log_message(DEBUG_WIFI, "\n", true);

    if (status)
    {
        localIP = (String)WiFi.localIP().toString().c_str();
        wifi_signal = (String)WiFi.RSSI(); //get signal strength.
        //  log_message(DEBUG_MSG, "WiFi Connected: " + String(wifi_signal) + "dB" + String(localIP));
        return true;
    }
    else
    {   
        return false;
    }   
}

bool wifi_connect()
{
    bool status = 1;
    uint8_t temp = 0;

    //  log_message(DEBUG_MSG, "Connecting to WIFI Router: " + (String)WIFI_SSID);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay_ms(500);
        log_message(DEBUG_WIFI, " . ", true);

        timerWrite(timer_w, 0); // Reset watchdog timer

        temp++;
        if (temp >= WIFI_RETRY_COUNT) {
            status = 0;
            break;
        }
    }

    log_message(DEBUG_WIFI, "\n", true);

    if (status)
    {
        localIP = (String)WiFi.localIP().toString().c_str();
        wifi_signal = (String)WiFi.RSSI(); //get signal strength.
        return true;
    }
    else
    {
        WiFi.disconnect(true);
        WiFi.mode(WIFI_OFF);
        
        update_status_code(1100);     
        return false;
    }
}

bool action_event_image_change(String event, String image_name)
{
    String image_path((char *)0);
    
    timerWrite(timer_w, 0); // Reset watchdog timer

    String img_curr_temp((char *)0);
    img_curr_temp += img_curr.substring(0, img_curr.length());

    if (image_name != "default.jpg" && image_name.endsWith(img_curr_temp))
    {
        //  log_message(DEBUG_MSG, "Same image is displaying right now");
        return true;
    }
    if(rotation_number == 0) 
    {
        if (!write_value_to_eeprom(EEPROM_MEM_ADDR_IMG_CURR, image_name)) { // Image name
            return false;
        }
    }
    
    String image_name_tmp((char *)0);

    if (image_name == "default.jpg")
    {
        image_path += DEF_IMG_PATH;
        image_path += "/";
        image_path += event;
    }
    else
    {
        image_path += IMG_PATH;
        image_path += "/";
        image_path += event;
    }
    image_name_tmp = find_file(SPIFFS, image_path, image_name, 0);

    if (image_name != NULL && image_name != "")
    {

        if (!display_image(image_name_tmp)) {
            return false;
        }

        // if (DELETE_AFTER_CHANGE)
        // {
        //     String rotation_num_str = read_value_from_eeprom(EEPROM_MEM_IMG_ROTATION);
        //     uint8_t rotation_num = rotation_num_str.toInt();
        //     if (!rotation_num && image_name != "default.jpg")
        //     {
        //         image_name_tmp = find_file(SPIFFS, IMG_PATH, image_name, 0);
        //         delete_file(SPIFFS, "/", image_name_tmp);
        //     }
        // }
    }

    return true;
}

uint8_t fail_count = 0;
bool action_event_image_rotation(String event, uint8_t image_number)
{
    
    timerWrite(timer_w, 0); // Reset watchdog timer
    String image_path((char *)0);

    image_path += IMG_PATH;
    image_path += "/";
    image_path += event;

    int image_count = count_files(SPIFFS, image_path);

    if (image_count == 0)
    {
        // log_message(DEBUG_ERROR, "No image to rotate");
        return false;
    }
    else if (image_number > image_count)
    {
        image_number = 1;
    }
    //  log_message(DEBUG_MSG, "Image count = " + (String)image_count);

    String image_name((char *)0);

    image_name += find_file_by_number(SPIFFS, image_path, image_number);

    if (image_name == "0")
    {
        image_number++;
        // log_message(DEBUG_ERROR, "Image not found");
        fail_count++;
        if (fail_count > 5) {

            image_path.remove(0);
            return false;
        }
        action_event_image_rotation(event, image_number);
    }
    else if (!validate_file_size(SPIFFS, "/", image_name, IMAGE_FILE_SIZE, true))
    {
        image_number++;
        // log_message(DEBUG_ERROR, "File size validation fail");
        fail_count++;
    }
    else
    {
        image_name.replace(image_path, "");
        image_name = image_name.substring(4, image_name.length());

        //  log_message(DEBUG_MSG, "event: " + event + " image_name: " + image_name);
        action_event_image_change(event, image_name);
        img_curr = image_name;

        image_number++;

        if (image_count < image_number) {
            image_number = 1;
        }

        rotation_number = image_number;
        // write_value_to_eeprom(EEPROM_MEM_IMG_ROTATION, String(image_number)); // Image number
        // write_value_to_eeprom(EEPROM_MEM_IMG_NUMBER, String(image_number));   // Image number
        image_path.remove(0);

        return true;
    }
    return 0;
}

String get_request_URL(bool protocol) {

    timerWrite(timer_w, 0); 

    String requestURL((char *)0);
    
    String device_id((char *)0);        // requestURL.reserve(200);
    device_id += get_mac_address();

    if (protocol == HTTPS) {
        requestURL = "https://" + (String)HOST + "/";
    } else {
        requestURL = "http://" + (String)HOST + "/";
    }

    if (last_action == ACT_CHANGE) {
        requestURL += device_id;
        requestURL += "/";
        requestURL += img_curr;
        requestURL += "/";
        requestURL += img_last;
        requestURL += "/";
        requestURL += last_event;
        requestURL += "/";
        requestURL += last_action;
        requestURL += "/";
        requestURL += img_curr;
        requestURL += "/";
        requestURL += last_status;
    } else if (last_action == ACT_DOWNLOAD) {
        requestURL += device_id;
        requestURL += "/";
        requestURL += img_curr;
        requestURL += "/";
        requestURL += img_last;
        requestURL += "/";
        requestURL += last_event;
        requestURL += "/";
        requestURL += last_action;
        requestURL += "/";
        requestURL += img_last;
        requestURL += "/";
        requestURL += last_status;
    } else if (last_action == ACT_DELETE || last_action == ACT_WHITE || last_action == ACT_BLACK || last_action == ACT_FORMAT || last_action == ACT_ROTATE) {
        requestURL += device_id;
        requestURL += "/";
        requestURL += img_curr;
        requestURL += "/";
        requestURL += img_last;
        requestURL += "/";
        requestURL += last_event;
        requestURL += "/";
        requestURL += last_action;
        requestURL += "/";
        requestURL += "none";
        requestURL += "/";
        requestURL += last_status;
    } else if (last_action == ACT_FILE_COUNT) {
        String dir_path((char *)0);
        dir_path += IMG_PATH;

        if (last_event != "0") {
            dir_path += "/";
            dir_path += last_event;
        }
        int image_count = count_files(SPIFFS, dir_path);

        requestURL += device_id;
        requestURL += "/";
        requestURL += img_curr;
        requestURL += "/";
        requestURL += img_last;
        requestURL += "/";
        requestURL += last_event;
        requestURL += "/";
        requestURL += last_action;
        requestURL += "/";
        requestURL += "none";
        requestURL += "/";
        requestURL += last_status;
        requestURL += "/";
        requestURL += String(image_count);
    } else { // none
        requestURL += device_id;
        requestURL += "/";
        requestURL += img_curr;
        requestURL += "/";
        requestURL += img_last;
        requestURL += "/";
        requestURL += last_event;
        requestURL += "/";
        requestURL += last_action;
        requestURL += "/";
        requestURL += "none";
        requestURL += "/";
        requestURL += "none";
    }
    return requestURL;
}

bool push_pull() {
    bool status = 0;
    uint16_t status_update_code = 0;

    HTTPClient http;
    String payload((char *)0);
    // payload.reserve(30000);
    String request((char *)0);
    String image_path((char *)0);
    String image_name((char *)0);
    String image_name_s((char *)0);
    String image_type((char *)0);
    String image_duration((char *)0);
    String current_event((char *)0);
    String image_data_color((char *)0);
    String image_meta_data((char *)0);
    String rotate_param((char *)0);
    String rotate_time((char *)0);

    uint16_t file_size = 0;

    uint8_t action_number = 0;

    timerWrite(timer_w, 0); 

    // temp_1 = read_temperature_sensor(0);
    // temp_2 = read_temperature_sensor(1);

    String connection_time = read_value_from_eeprom(EEPROM_MEM_ADDR_CONN_TIME);

    String telemetry_data((char *)0);
    telemetry_data += "/";
    telemetry_data += String(battery_voltage);
    telemetry_data += "/";
    telemetry_data += wifi_signal;
    telemetry_data += "/Cbatt:";
    telemetry_data += String(battery_capacity);
    // telemetry_data += "_Temp1:";
    // telemetry_data += String(temp_1);
    // telemetry_data += "_Temp2:";
    // telemetry_data += String(temp_2);
    telemetry_data += "_ConTime:";
    telemetry_data += connection_time;
    telemetry_data += "_Ver:";
    telemetry_data += String(CURRENT_VERSION);

    request = get_request_URL(protocol);

    request += telemetry_data;
    telemetry_data.remove(0);

    log_message(DEBUG_WIFI, "request: " + request);

    // http.clear();

    http.setConnectTimeout(CLIENT_CONNECTION_TIMEOUT);
    
    if (protocol == HTTPS) {
        WiFiClientSecure *client = new WiFiClientSecure;
        client->setCACert(SSL_Certificate);

        if (http.begin(*client, request)) {
            status = 1;
        }
    } else {
        if (http.begin(request)) {
            status = 1;
        }
    }

    timerWrite(timer_w, 0); 

    if (status) {
        delay(500);
        int httpCode = http.GET();
        int payloadSize = http.getSize();
        String httpError = http.errorToString(httpCode);

        // Stream *payload_stream;
        // int http_return = writeToStream(&payload_stream);

        //  log_message(DEBUG_MSG, "[HTTP] GET... code: " + String(httpCode));
        //  log_message(DEBUG_MSG, "[HTTP] GET... size: " + String(payloadSize));
        //  log_message(DEBUG_MSG, "[HTTP] GET... error: " + String(httpError));

        if (payloadSize > MAX_PAYLOAD_SIZE) {
            http.end();
            WiFi.disconnect(true);
            WiFi.mode(WIFI_OFF);

            // log_message(DEBUG_ERROR, "Payload size is exeeded the limit");
            update_status_code(1201);
            return 0;
        }

        uint16_t FreeHeap = ESP.getFreeHeap();
        if (FreeHeap < payloadSize) {
            http.end();
            WiFi.disconnect(true);
            WiFi.mode(WIFI_OFF);
            
            // log_message(DEBUG_ERROR, "Device free heap is not enough to read the payload");
            update_status_code(1202);
            return 0;
        }

        if (httpCode > 0) {
            payload += http.getString();
            uint16_t payload_size = payload.length();

            if (payload_size < 5) {
                // log_message(DEBUG_ERROR, "Payload is empty, retrying..");
                delay(500);
                // payload += http.getString();
            }
            // http.clear();
            // http.end();
            // WiFi.disconnect(true);
            // WiFi.mode(WIFI_OFF);
            // esp_wifi_stop();

            payload_size = payload.length();

            if (payload_size < 5) {
                update_status_code(1401);
                return 0;
            } else if (payload_size > MAX_PAYLOAD_SIZE) {
                update_status_code(1402);
                return 0;
            }
            payload += "\n";
        } else {
            http.end();
            WiFi.disconnect(true);
            WiFi.mode(WIFI_OFF);

            update_status_code(1200);
            return 0;
        }
    }
    String substring((char *)0);

    char char_temp;
    uint16_t char_count = 0;

    timerWrite(timer_w, 0); 

    while (status) {
        char_temp = payload.charAt(char_count);

        if (char_temp == '|') {
            substring.remove(0);
            substring += payload.substring(1, char_count);
            payload.remove(0, char_count);
            char_count = 0;
            ///////////////////////////////////////////////////////// CONNECTION TIMER //////////////////////////////////////////////
            if (substring.startsWith("connect:")) {
                bitWrite(status_update_code, 0, 1);

                substring.replace("connect:", "");

                time_t connection_time = substring.toInt();

                if ((connection_time < 30) || (connection_time > 14400)) {
                    connection_time = DEFAULT_CONNECTION_TIME;
                } 
                
                time_t pre_connection_time = read_value_from_eeprom(EEPROM_MEM_ADDR_CONN_TIME).toInt();

                if (pre_connection_time != connection_time) {
                    write_value_to_eeprom(EEPROM_MEM_ADDR_CONN_TIME,  String(connection_time));
                    new_connection_time = true;
                } 
            }   

            ///////////////////////////////////////////////////////////// TIMESTAMP /////////////////////////////////////////////////
            if (substring.startsWith("timestamp:")) {
                substring.replace("timestamp:", "");

                substring.remove(10, 12);

                bitWrite(status_update_code, 1, 1);

                time_t time_stamp_receive = substring.toInt();

                time_t time_stamp_2021_01_01 = 1609502400;

                if (time_stamp_2021_01_01 > time_stamp_receive) {
                    // log_message(DEBUG_ERROR, "wrong timestamp");
                    update_status_code(2401);
                    return false;
                } else {
                    time_stamp = rtc_get_time();

                    time_t time_def = time_stamp - time_stamp_receive;

                    if (time_def > 100 || time_def < -100) {
                         log_message(DEBUG_MSG, "RTC Time is updating");
                        rtc_set_time(time_stamp_receive);
                    }
                }
                substring.remove(0);
                substring += payload.substring(1, char_count);
                payload.remove(0, char_count);
                char_count = 0;
            }
            ///////////////////////////////////////////////////////////// EVENT /////////////////////////////////////////////////////
            if (substring.startsWith("event:")) {
                substring.replace("event:", "");

                //  log_message(DEBUG_MSG, "event id:           \t\t" + substring);

                current_event += substring;

                if (!write_value_to_eeprom(EEPROM_MEM_ADDR_LST_EVENT, current_event)) {
                    return false;
                }

                bitWrite(status_update_code, 2, 1);

                substring.remove(0);
                substring += payload.substring(1, char_count);
                payload.remove(0, char_count);
                char_count = 0;
            }
            ///////////////////////////////////////////////////////////// ACTION ////////////////////////////////////////////////////
            if (substring.startsWith("action:")) {
                substring.replace("action:", "");

                //  log_message(DEBUG_MSG, "event:       \t\t" + substring);

                if (substring == "none") {
                    if (!write_value_to_eeprom(EEPROM_MEM_ADDR_LST_ACTION, ACT_NONE)) {
                        return false;
                    }
                    action_number = 1;
                } else if (substring == "change") {
                    if (!write_value_to_eeprom(EEPROM_MEM_ADDR_LST_ACTION, ACT_CHANGE)) {
                        return false;
                    }
                    action_number = 2;
                } else if (substring == "download") {
                    if (!write_value_to_eeprom(EEPROM_MEM_ADDR_LST_ACTION, ACT_DOWNLOAD)) {
                        return false;
                    }
                    action_number = 3;
                } else if (substring == "delete") {
                    if (!write_value_to_eeprom(EEPROM_MEM_ADDR_LST_ACTION, ACT_DELETE)) {
                        return false;
                    }
                    action_number = 4;
                } else if (substring == "format") {
                    if (!write_value_to_eeprom(EEPROM_MEM_ADDR_LST_ACTION, ACT_FORMAT)) {
                        return false;
                    }
                    action_number = 5;
                } else if (substring == "rotate") {
                    if (!write_value_to_eeprom(EEPROM_MEM_ADDR_LST_ACTION, ACT_ROTATE)) {
                        return false;
                    }
                    action_number = 6;
                } else if (substring == "file-count") {
                    action_number = 7;
                } else if (substring == "white") {
                    action_number = 8;
                } else if (substring == "black") {
                    action_number = 9;
                }

                substring.remove(0);
                substring += payload.substring(1, char_count);
                payload.remove(0, char_count);
                char_count = 0;
            }

            ///////////////////////////////////////////////////////////// DATASIZE ///////////////////////////////////////////////////
            if (substring.startsWith("data_size:")) {
                substring.replace("data_size:", "");

                //  log_message(DEBUG_MSG, "data_size:       \t\t" + substring);

                bitWrite(status_update_code, 3, 1);

                file_size = substring.toInt();

                substring.remove(0);
                substring += payload.substring(1, char_count);
                payload.remove(0, char_count);
                char_count = 0;
            }
            ///////////////////////////////////////////////////////////// NAME ///////////////////////////////////////////////////////
            if (substring.startsWith("name:")) {
                substring.replace("name:", "");

                //  log_message(DEBUG_MSG, "name:            \t\t" + substring);

                if (substring != NULL && substring != "" && substring.length() <= 32) {
                    image_name = substring;
                    bitWrite(status_update_code, 4, 1);
                } else {
                    update_status_code(2404);
                    return false;
                }

                substring.remove(0);
                substring += payload.substring(1, char_count);
                payload.remove(0, char_count);
                char_count = 0;
            }
            ///////////////////////////////////////////////////////////// TYPE ////////////////////////////////////////////////////////
            if (substring.startsWith("type:")) {
                substring.replace("type:", "");

                //  log_message(DEBUG_MSG, "type:\t\t" + substring);

                if (substring == "bw" || substring == "rw" || substring == "br") {
                    image_type = substring;
                    bitWrite(status_update_code, 5, 1);
                } else {
                    // log_message(DEBUG_ERROR, "Wrong data format!");
                    update_status_code(2403);
                    return false;
                }

                substring.remove(0);
                substring += payload.substring(1, char_count);
                payload.remove(0, char_count);
                char_count = 0;
            }
            ///////////////////////////////////////////////////////////// DURATION /////////////////////////////////////////////////////
            if (substring.startsWith("duration:")) {
                substring.replace("duration:", "");

                log_message(DEBUG_MSG, "duration: \t\t" + substring);
                image_duration += substring;

                bitWrite(status_update_code, 6, 1);

                substring.remove(0);
                substring += payload.substring(1, char_count);
                payload.remove(0, char_count);
                char_count = 0;
            }
            ///////////////////////////////////////////////////////////// BLACK (IMAGE DATA) ///////////////////////////////////////////
            if (substring.startsWith("blk:")) {
                substring.replace("blk:", "");

                image_data_color += "blk";
                bitWrite(status_update_code, 7, 1);
            }
            ///////////////////////////////////////////////////////////// ROTATE PARAMETER ///////////////////////////////////////////
            if (substring.startsWith("param:")) {
                substring.replace("param:", "");

                bitWrite(status_update_code, 8, 1);

                rotate_param += substring;
                substring.remove(0);
                substring += payload.substring(1, char_count);
                payload.remove(0, char_count);
                char_count = 0;
            }
            ///////////////////////////////////////////////////////////// ROTATE TIME ///////////////////////////////////////////
            if (substring.startsWith("time:")) {              
                substring.replace("time:", "");

                bitWrite(status_update_code, 9, 1);

                rotate_time += substring;
                substring.remove(0);
                substring += payload.substring(1, char_count);
                payload.remove(0, char_count);
                char_count = 0;
            }
        } else if (char_temp == '\n') {
            substring += payload.substring(1, char_count);
            payload.remove(0, char_count); ///////////////////////////////////////////////////////////// END ////////////////////////////////////////////////////////
            if (substring.endsWith("end")) {
                //  log_message(DEBUG_MSG, "end");
                substring.replace("end", "");
                bitWrite(status_update_code, 15, 1);
                status = 0;
            } else {
                update_status_code(2412);
                status = 0;
            }
        }
        char_count++;
    }
    //  log_message(DEBUG_MSG, "status_update_code:\t\t" + String(status_update_code));

    timerWrite(timer_w, 0); 

    switch (action_number) {
        case 1: /// none  ///
            //  log_message(DEBUG_MSG, "NONE");

            if (status_update_code == 0b1000000000000111) {
                update_status_code(1); // 1 = Action event "none"
                break;
            } else {
                update_status_code(2411);
                return false;
            }
        case 2: /// change  ///
            //  log_message(DEBUG_MSG, "CHANGE");

            if (status_update_code == 0b1000000000011111) {
                if (image_name != NULL && image_name != "") {
                    log_message(DEBUG_WIFI, "Image changing: " + image_name);
                    image_name.trim();

                    if (!write_value_to_eeprom(EEPROM_MEM_ADDR_RUNNUNG_EVENT, "1")) {
                        return false;
                    }
                    if (!action_event_image_change(current_event, image_name)) {
                        return false;
                    }
                    if (!write_value_to_eeprom(EEPROM_MEM_ADDR_IMG_CURR, image_name)) {
                        return false;
                    }
                }
                // write_value_to_eeprom(EEPROM_MEM_IMG_ROTATION, "0"); // Image rotation off
                rotation_number = 0;

                update_status_code(2); // 2 = Action event "change" is Success
            } else {
                update_status_code(2411);
                return false;
            }
            break;
        case 3: /// donload  ///
            if (status_update_code == 0b1000000011111111) {
                if (!write_value_to_eeprom(EEPROM_MEM_ADDR_IMG_LAST, image_name)) {
                    return false;
                }
                if (image_name == "default.jpg") {
                    image_path.remove(0);
                    image_path += DEF_IMG_PATH;
                    image_path += "/";
                    image_path += current_event;
                    image_name_s += image_name;
                } else {
                    image_path += IMG_PATH;
                    image_path += "/";
                    image_path += current_event;
                    int image_count = count_files(SPIFFS, image_path);
                    //  log_message(DEBUG_MSG, "Image count = " + (String)image_count);
                    image_count++;
                    image_name_s += number_to_string(image_count);
                    image_name_s += image_name;
                }
                //  log_message(DEBUG_MSG, "Image name: " + image_path + "/" + image_name_s);
                image_meta_data += "type:";
                image_meta_data += image_type;
                image_meta_data += "|duration:";
                image_meta_data += image_duration;
                image_meta_data += "|";
                image_meta_data += image_data_color;
                image_meta_data += ":";
                substring += "|";

                uint16_t received_file_size = image_meta_data.length();
                received_file_size = received_file_size + substring.length();

                if (received_file_size != file_size) {
                    update_status_code(2406);
                }

                //  log_message(DEBUG_MSG, "received_file_size" + String(received_file_size));

                if (!write_file(SPIFFS, image_path, image_name_s, image_meta_data)) {
                    // log_message(DEBUG_ERROR, "Fail to write file:" + image_name_s);
                    return false;
                }
                if (!append_file(SPIFFS, image_path, image_name_s, substring)) {
                    // log_message(DEBUG_ERROR, "Fail to append file:" + image_name_s);
                }

                if (!validate_file_size(SPIFFS, image_path, image_name_s, file_size, true)) {
                    // log_message(DEBUG_ERROR, "File size doesn't match");
                    update_status_code(2405);
                    // delete_file(SPIFFS, image_path, image_name_s);
                    return false;
                }
                update_status_code(3); // 3 = Action event "Download" is Success
            } else {
                update_status_code(2411);
                return false;
            }
            break;
        case 4: /// delete   ///
            //  log_message(DEBUG_MSG, "DELETE");

            if (status_update_code == 0b1000000000010111) {
                if (image_name != NULL && image_name != "") {
                    log_message(DEBUG_WIFI, "Image name: " + image_name);
                    log_message(DEBUG_WIFI, "Image data deleting");

                    if (!delete_file(SPIFFS, IMG_PATH, image_name)) {
                        return false;
                    }
                }
                update_status_code(4); // 4 = Action event "Delete" is Success
            } else {
                update_status_code(2411);
            }
            break;
        case 5: /// format   ///
            //  log_message(DEBUG_MSG, "FORMAT");
            if (status_update_code == 0b1000000000001111) {
            
                timerWrite(timer_w, 0); 

                if (current_event == "0") {
                    if (!write_value_to_eeprom(EEPROM_MEM_ADDR_RUNNUNG_EVENT, "0")) {
                        return false;
                    }
                    // if (!write_value_to_eeprom(EEPROM_MEM_IMG_ROTATION, "0"))
                    // { // Image rotation off
                    //     return false;
                    // }
                    rotation_number = 0;
                    //  log_message(DEBUG_MSG, "SPIFFS Formatting.. Please wait...");
                    if (!format_spiffs()) {

                    timerWrite(timer_w, 0); // Reset watchdog timer 
                    return false;
                    }
                } else {
                    image_path += IMG_PATH;
                    image_path += "/";
                    image_path += current_event;

                    if (last_rotate_event == current_event) {
                        // if (!write_value_to_eeprom(EEPROM_MEM_IMG_ROTATION, "0"))
                        // { // Image rotation off
                        //     return false;
                        // }
                        rotation_number = 0;
                    }

                    if (!delete_dir(SPIFFS, image_path)) {
                        // log_message(DEBUG_ERROR, "Failed to delete /images");
                        return false;
                    }
                }
                update_status_code(5); // 5 = Action event "Format" is Success
            } else {
                update_status_code(2411);
                return false;
            }
            break;
        case 6: /// rotate   ///
            //  log_message(DEBUG_MSG, "ROTATE");
            if (status_update_code == 0b1000001100001111) {
                if (rotate_param == "1") {
                    String last_image_number_str((char *)0);
                    log_message(DEBUG_MSG, "Rotation > 0");
                    
                    if (!write_value_to_eeprom(EEPROM_MEM_ADDR_RUNNUNG_EVENT, "1")) {
                        return false;
                    }

                    log_message(DEBUG_MSG, "Rotation > 1");

                    if (!write_value_to_eeprom(EEPROM_MEM_ADDR_LST_ROTATE_EVENT, current_event)) {
                        return false;
                    }
                    if (!write_value_to_eeprom(EEPROM_MEM_ROTATION_EN, "1")) {
                        return false;
                    }

                    rotation_number = 1;

                    if (!write_value_to_eeprom(EEPROM_MEM_ADDR_LST_ACTION, ACT_ROTATE)) {
                        return false;
                    }

                    uint16_t rtc_day_of_year_tmp = rtc_day_of_year();
                    if (!write_value_to_eeprom(EEPROM_MEM_ROTATION_ON_DATE, String(rtc_day_of_year_tmp))) { 
                        return false;
                    }
                    
                    add_icon_data(Task_Running, 140, 18);
                } else if (rotate_param == "0") {
                    if (!write_value_to_eeprom(EEPROM_MEM_ADDR_RUNNUNG_EVENT, "0")) {
                        return false;
                    }
                    if (!write_value_to_eeprom(EEPROM_MEM_ADDR_LST_ROTATE_EVENT, current_event)) {
                        return false;
                    }
                    if (!write_value_to_eeprom(EEPROM_MEM_ROTATION_EN, "0")) {
                        return false;
                    }
                    rotation_number = 0;
                    if (!write_value_to_eeprom(EEPROM_MEM_ADDR_LST_ACTION, ACT_ROTATE)) {
                        return false;
                    }
                } else {
                    // log_message(DEBUG_ERROR, "Wrong data format!");
                    update_status_code(2403);
                    return false;
                }
                
                uint16_t received_rotation_time = rotate_time.toInt(); 
                uint16_t previous_rotation_time = read_value_from_eeprom(EEPROM_MEM_ADDR_ROTATION_DURATION).toInt();
                uint16_t new_rotation_time = 0;

                if (received_rotation_time != previous_rotation_time) {
                    new_rotation_time = (received_rotation_time > MAXIMUM_CONNECTION_TIME) ? previous_rotation_time : received_rotation_time;
                    new_rotation_time = (received_rotation_time < DEFAULT_CYCLE_TIME)   ? previous_rotation_time : received_rotation_time;
                    
                    write_value_to_eeprom(EEPROM_MEM_ADDR_ROTATION_DURATION, String(new_rotation_time));
                    log_message(DEBUG_MSG, "new rotatation time: " + String(new_rotation_time));
                } else {
                    log_message(DEBUG_MSG, "No changes to the rotation time: " + String(previous_rotation_time));
                }
                log_message(DEBUG_MSG, "Updating status code: 6");
                update_status_code(6); // 6 = Action event "Rotation" is Success
            } else {
                update_status_code(2411);
            }
            break;
        case 7: /// file count  ///
            //  log_message(DEBUG_MSG, "FILE_COUNT");
            if (status_update_code == 0b1000000000001111) {
                if (!write_value_to_eeprom(EEPROM_MEM_ADDR_LST_ACTION, ACT_FILE_COUNT)) {
                    return false;
                }
                update_status_code(7); // 7 = Action event "File-list" is Success
                break;
            } else {
                update_status_code(2411);
            }
            break;
        case 8: /// display-white  ///
            //  log_message(DEBUG_MSG, "DISPLAY > WHITE");
            if (status_update_code == 0b1000000000001111) {

                // write_value_to_eeprom(EEPROM_MEM_IMG_ROTATION, "0"); // Image rotation off
                rotation_number = 0;

                if (!write_value_to_eeprom(EEPROM_MEM_ADDR_RUNNUNG_EVENT, "0"));
                {
                    return false;
                }
                if (!write_value_to_eeprom(EEPROM_MEM_ADDR_LST_ACTION, ACT_WHITE)) {
                    return false;
                }
                if (!display_image("white")) {
                    return false;
                }
                if (!write_value_to_eeprom(EEPROM_MEM_ADDR_IMG_CURR, "white")) {
                    return false;
                }
                update_status_code(8); // 7 = Action event "File-list" is Success
            } else {
                update_status_code(2411);
            }
            break;

        case 9: /// display-black  ///
            //  log_message(DEBUG_MSG, "DISPLAY > BLACK");
            if (status_update_code == 0b1000000000001111) {

                // write_value_to_eeprom(EEPROM_MEM_IMG_ROTATION, "0"); // Image rotation off
                rotation_number = 0;

                if (!write_value_to_eeprom(EEPROM_MEM_ADDR_RUNNUNG_EVENT, "0"));
                {
                    return false;
                }

                if (!write_value_to_eeprom(EEPROM_MEM_ADDR_LST_ACTION, ACT_BLACK)) {
                    return false;
                }
                if (!display_image("black")) {
                    return false;
                }
                if (!write_value_to_eeprom(EEPROM_MEM_ADDR_IMG_CURR, "black")) {
                    return false;
                }
                update_status_code(9); // 7 = Action event "File-list" is Success
            } else {
                update_status_code(2411);
            }
            break;
        default:
            //  log_message(DEBUG_MSG, "default");
            update_status_code(2401);
            return 0;
    }
    return 0;
}
#endif /* WIFI_ACTIONS_H */
