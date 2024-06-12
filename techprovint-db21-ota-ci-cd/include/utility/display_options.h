#ifndef DISPLAY_OPTIONS_H
#define DISPLAY_OPTIONS_H

String completed = "==================== COMPLETED ====================";
String line = "====================";
String string_fail = "Fail";
String string_success = "Success";

bool optFlash = false;
bool optDisplay = false;
bool optMisc = false;
bool opt_settings = false;
bool opt_wifi = false;
bool dev_options = false;

char serial_read_char(){
    char read_char = '~', tmp;
    while(Serial.available()) {
        timerWrite(timer_w, 0);
        tmp = Serial.read();
        if(tmp != '\n') {
            // Serial.println("You pressed: " + (String)tmp);
            read_char = tmp;
        }
    }
    if(read_char != '~'){
    }
    return read_char;
}

bool serial_read_check(char inchar1, char inchar2){
    while(1){
        char read_char = serial_read_char();
        if(read_char == '~'){
            // ignore
        } else if((read_char == inchar1) || (read_char == inchar2)) {
            return 1;
        } else {
            return 0;
        }
    }
}

void main_menu() {
    Serial.println(line);   // line = "===========================================";
    Serial.println("1 > Developer mode");
    Serial.println("2 > EPD Test");
    Serial.println("3 > File management");
    Serial.println("9 > Back");
    Serial.println("0 > EXIT");
    Serial.println(line);
}

void menu_developer_mode(){
    Serial.println(line);   // line = "===========================================";
    Serial.println("1 > Developer mode on");
    Serial.println("2 > Developer mode off");
    Serial.println("9 > Back");
    Serial.println("0 > EXIT");
    Serial.println(line);
}

void menu_EPD() {
    Serial.println(line);   // line = "===========================================";
    Serial.println("1 > Pattern Check");
    Serial.println("2 > Fill White");
    Serial.println("3 > Fill Black");
    Serial.println("- > Back");
    Serial.println("0 > EXIT");
    Serial.println(line);
}

void menu_file_management() {
    Serial.println(line);       // line = "===========================================";
    Serial.println("1 > File list");
    Serial.println("2 > Log files");
    Serial.println("3 > Batt_log files");
    Serial.println("4 > Delete log files");
    Serial.println("5 > Delete image files");
    Serial.println("6 > Format SPIFFS");
    Serial.println("7 > Clear EEPROM");
    Serial.println("- > Back");
    Serial.println("0 > Exit");
    Serial.println(line);       // line = "===========================================";
}


void options_file_management(){
    menu_file_management();
    Serial.flush();
    optFlash = true;
    while (optFlash) {
        //timerWrite(timer_w, 0);
        debug_mem_en = 1;
        char read_char = serial_read_char();
        switch (read_char) {
            case '~':
                break;
            case '1':
                Serial.println("Start");
                list_dir(SPIFFS, "/", 0);
                Serial.println("End");
                break;
            case '2':
                Serial.println("Start");
                // DEBUG_MEM_EN = 0;
                print_dir_all_files_content(SPIFFS, LOG_PATH);
                Serial.println("End");
                break;
            case '3':
                Serial.println("Start");
                // DEBUG_MEM_EN = 0;
                print_dir_all_files_content(SPIFFS, BATT_LOG);
                Serial.println("End");
                break;
            case '4': 
                Serial.println("Delete log files (Y/N)");

                if(serial_read_check('y', 'Y')) {
                    delete_dir(SPIFFS, String(LOG_PATH));
                    delete_dir(SPIFFS, String(BATT_LOG));
                    Serial.println("completed");
                    break;
                } else if(serial_read_check('n', 'N')) {
                    break;
                }
                menu_file_management();
                break;

            case '5':
                Serial.println("Delete Image files? (Y/N)");
                if(serial_read_check('y', 'Y')) {
                    delete_dir(SPIFFS, IMG_PATH);
                    Serial.println("completed");
                    break;
                } else if(serial_read_check('n', 'N')) {
                    break;
                }
                menu_file_management();
                break;

            case '6':
                Serial.println("Delete all files? (Y/N)");
                if(serial_read_check('y', 'Y')) {
                    Serial.println("Wait.");
                    format_spiffs();

                    timerWrite(timer_w, 0); // Reset watchdog timer 

                    menu_file_management();
                    break;
                } else if(serial_read_check('n', 'N')) {
                    break;
                }   
                menu_file_management();
            case '7':
                Serial.println("Clear EEPROM? (Y/N)");
                if(serial_read_check('y', 'Y')) {
                    bool erase_status = false;
                    erase_status = write_value_to_eeprom(EEPROM_MEM_ADDR_IMG_LAST, "none");
                    erase_status = write_value_to_eeprom(EEPROM_MEM_ADDR_IMG_CURR, "none");
                    erase_status = write_value_to_eeprom(EEPROM_MEM_ADDR_LST_ACTION, "none");
                    erase_status = write_value_to_eeprom(EEPROM_MEM_ADDR_LST_EVENT, "none");
                    erase_status = write_value_to_eeprom(EEPROM_MEM_ADDR_LST_ROTATE_EVENT, "none");
                    erase_status = write_value_to_eeprom(EEPROM_MEM_ADDR_LST_STATUS, "none");
                    // erase_status = write_value_to_eeprom(EEPROM_MEM_IMG_ROTATION, "none");
                    // erase_status = write_value_to_eeprom(EEPROM_MEM_IMG_NUMBER, "none");
                    erase_status = write_value_to_eeprom(EEPROM_MEM_ADDR_LST_ROTATE_EVENT_TMP, "none");

                    if ( erase_status ) {
                        Serial.println("Done");
                    } else {

                    }
                    
                    timerWrite(timer_w, 0);  // Reset watchdog timer
                    
                    break;
                } else if(serial_read_check('n', 'N')) {
                    break;
                }
                menu_file_management();
                break;

            case '0':
                optFlash = false;
                DISPLAY_OPTIONS = false;
                break;

            case '-':
                optFlash = false;
                break;

            default:
                Serial.println("Please re-enter");
                menu_file_management();
        }
        
    }   
}

void display_options_EPD() {
    menu_EPD();
    while (optDisplay) {
        char read_string = serial_read_char();
        switch (read_string) {
            case '1':
                Serial.println("Pattern check");
                display_image("pattern");
                Serial.println("Pattern check complete");
                break;
            case '2':
                Serial.println("EPD Check > White");
                display_image("white");
                Serial.println("EPD Check > Complete");
                break;
            case '3':
                Serial.println("EPD Check > Black");
                display_image("black");
                Serial.println("EPD Check > Complete");
                break;
            case '-':
                optDisplay = 0;
                break;
            case '0':
                optDisplay = 0;
                DISPLAY_OPTIONS = 0;
                break;
            case '~':
                break;
            default:
                Serial.println("Please re-enter");
                menu_EPD();
        }
    }
}

void display_options() {
    
    DISPLAY_OPTIONS = false;
    char read_char = serial_read_char();

    if(read_char == 'D' || read_char == 'd') {
        DISPLAY_OPTIONS = true;
    }

    while(DISPLAY_OPTIONS) {
        main_menu();
        bool tmp = 1;
        while (tmp) {
            //Serial.println(F("B");
            //timerWrite(timer_w, 0);

            char read_string = serial_read_char();
        
            switch (read_string) {
                case '3':
                    optFlash = 1;
                    options_file_management();
                    tmp = 0;
                    break;
                case '2':
                    optDisplay = 1;
                    display_options_EPD();
                    tmp = 0;
                    break;
                case '1':
                    if(read_int_value_from_eeprom(EEPROM_MEM_ADDR_DEVELOPER_MODE)){
                        Serial.println("Turn off developer mode? (Y/N)");
                        if(serial_read_check('y', 'Y')) {
                            if(!write_value_to_eeprom(EEPROM_MEM_ADDR_DEVELOPER_MODE, "0")) {
                                Serial.println(string_fail);
                            } else{
                                Serial.println(string_success);
                            }   
                        } else{
                            main_menu();
                            break;
                        }     
                    } else {
                        Serial.println("Turn on developer mode? (Y/N)");
                        if(serial_read_check('y', 'Y')) {
                            if(!write_value_to_eeprom(EEPROM_MEM_ADDR_DEVELOPER_MODE, "1")) {
                                Serial.println(string_fail);
                            } else{
                                Serial.println(string_success);
                            }   
                        } else{
                            main_menu();
                            break;
                        }     
                    }
                case '0':
                    DISPLAY_OPTIONS = 0;
                    tmp = 0;
                    break;
                case '~':
                    break;
                default:
                    main_menu();
                
            }
        }
    }
}


#endif /* DISPLAY_OPTIONS_H  */
