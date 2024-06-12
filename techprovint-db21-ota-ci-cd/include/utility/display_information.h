
/* Icon data */

// String Batt_00 = "0";
// String Batt_20 = "0";
// String Batt_40 = "0";
// String Batt_60 = "0";
// String Batt_80 = "0";
// String Batt_100 = "0";
// String Batt_CH = "0";
// String Batt_CH_FL = "0";
// String Device_Off = "0";
// String No_Task = "0";
// String Task_Running = "0";
// String WiFi_Connecting = "0";
// String WiFi_Fail = "0";
// String WiFi_OK = "0";
// String WiFi_Sleep = "0";
// String Server_error = "0";

String Batt_00 = "10400200910913F893F893F893F893F893F883F883F883F883F883F883F893F893F893F893F89091091";
String Batt_20 = "1040020091091027F69027F69027F69027F69027F69027F68027F68027F68027F68027F68027F68027F69027F69027F69027F69027F69091091";
String Batt_40 = "104002009109104F5904F5904F5904F5904F5904F5804F5804F5804F5804F5804F5804F5904F5904F5904F5904F59091091";
String Batt_60 = "1040020091091051F39051F39051F39051F39051F39051F38051F38051F38051F38051F38051F38051F39051F39051F39051F39051F39091091";
String Batt_80 = "1040020091091073F19073F19073F19073F19073F19073F18073F18073F18073F18073F18073F18073F19073F19073F19073F19073F19091091";
String Batt_100 = "104002009109109109109109109109090909090909061091091091091091091";
String Batt_CH = "0024016FFFFFCFFFFF8FFCFF0FF87E1FF07C3FE0787FC070FF8461FF0C43FE1C07FC3C0FF87C1FF0FC3FF1FE7FF3FFFFF7FFFFF";
String Batt_CH_FL = "002401600000300000700300F00781E00F83C01F87803F8F007B9E00F3BC01E3F803C3F00783E00F03C00E01800C00000800000";
String Device_Off = "1024024F9F9033F2033F23F23F23F23F23F53F5386018337AF1BF137AF1BF137AF1BF137AF1BF137A18737AF1BF137AF1BF137AF1BF137AF1BF1386F1BF13F501F501F9F2";
String No_Task = "1024024F9F49E79F1804139E79C3F4C3F4C3F4C09033F4C3E7F13C3F13E7C3F19CF1C3F1C9F1C3F1E3F1C3F1E3F1C3F1C9F1C3F19CF1C3F13E7C3E7F13C3F4C068041";
String Task_Running = "1024024F9F49E79F1804139E79C3F4C3F4C3F4C09033F4C3F4C3F39C3F33C3F2E7C3F2CF1C3CF19F1C3E73F1C3F127F1C3F18F2C3F1DF2C3F4C068041";
String WiFi_Connecting = "1024024F9F9F1C013F3037F1C031F1C3F1E01F1017F28F19F6E0171F2801601F202CE7F101F19F13F19F19F13F39F13F29CE7F201E013F201F111F29F28F5CF9F9F6";
String WiFi_Fail = "1024024F9F9F1C013F3037F1C031F1C3F1E01F1017F28F19F6E017F38017F302F401F1E79F19F233F487F29F1CF301F187F201F133F29E79F9F9F9F3";
String WiFi_OK = "1024024F9F9F1C013F3037F1C031F1C3F1E01F1017F28F19F6E013F3802F1EF1037CF101F39F19F33F3EE7F29E4F301F11F301F1BF39F9F9F9F6";
String WiFi_Sleep = "1024024F9F7E3F4C3F18F283F1C037F1E03F3021F38011F3C011F3E011F3C011F38011F30141F2E01E1F1C011F11F1C013F18F2E7F1CF2E7F1EF2E7F4E7F4E7F9F6";
String Server_error = "1024024F9F9033F2033F23F23F23F23F23F53F1DF1373F1DAD63F1DAD63F1DAD63F1DAD63F1C2D013F2ADE3F2B3E3BEF33DDF33EBF33F17F33EBF301DDF301BEF9";
String warning = "0008016FFE7E7E7E7E7E7E7E7E7E7FFFFE7E7FF";
String update = "1032032F9F9F1C3F5E027F48F1E1F3E3F2CF3CF3E7F29F43F2BF4BF23F49F27F4DF1E7F4CF7EF27F46DE3F429C1F48394F4C7B6F4EF27F73F4E7F1BF4EF29F4CF2DF4DF2CF49F2E7F33F33F2C7F38F21F4E027F5C3F9F9F9";
String right = "0016008FFFFFFE7FFCFFF9FE73FF27FF8FFFDFF";
// String test = "1016008F63F2E7F2CF239F293F2C7F2EF3";
              


String decompress_icon_data(String icon_data) 
{
    String return_string((char *)0);
    uint8_t length = icon_data.length();
    char character;
    uint16_t i = 1;

    while(i < 7)
    {
        character = icon_data.charAt(i);
        return_string += character;
        i++;
    }

    while(i < length)
    {
        character = icon_data.charAt(i);

        if(character == '0') {
            i++;
            String sub_string = icon_data.substring(i, i+1);
            uint8_t count = sub_string.toInt();
            for(uint8_t k = 0; k < count; k++) {
                return_string += '0';
            }
            i++;
        }
        else if(character == 'F') {
            i++;
            String sub_string = icon_data.substring(i, i+1);
            uint8_t count = sub_string.toInt();
            for(uint8_t k = 0; k < count; k++) {
                return_string += 'F';
            }
            i++;
        }
        else
        {
            return_string += character;
            i++;
        }        
    }
    return return_string;
}

void add_white_background_data()
{
    for(uint16_t byte_count = 0; byte_count < IMAGE_FILE_SIZE; byte_count++)
    {
        spiffs_read_buffer[byte_count] = 'F';
    }
}

void clear_icon_data(uint16_t x_start, uint16_t icon_width, uint16_t y_start, uint16_t icon_hight)
{
    uint16_t byte_count = 0;
    uint16_t x_pos = 0;
    uint16_t y_pos = 0;
    x_start = x_start / 4;

    for(y_pos = y_start; y_pos < (y_start + icon_hight); y_pos++) 
    {
        for(x_pos = x_start; x_pos < (x_start + (icon_width)/4); x_pos++) {
            byte_count = (((y_pos - 1) * 60) + x_pos) + 25;
            spiffs_read_buffer[byte_count] = 'F';
        }
    }
}

void add_icon_data(String icon_data, uint16_t x_start, uint8_t y_start) 
{
    uint16_t byte_count = 0;
    uint16_t string_pos = 6;
    uint16_t x_pos = 0;
    uint16_t y_pos = 0;
    String temp_string_1((char *)0);;

    if(icon_data.startsWith("0")) 
    {
        icon_data.remove(0,1);
        temp_string_1 = icon_data;
    }
    else
    {
        temp_string_1 += decompress_icon_data(icon_data);
    }
    
    String temp_string_2((char *)0);
    temp_string_2 = temp_string_1.substring(0,3);
    uint8_t icon_width = (temp_string_2.toInt());

    temp_string_2 = temp_string_1.substring(3,6);
    uint8_t icon_hight = temp_string_2.toInt();

    x_start = x_start / 4;

    for(y_pos = y_start; y_pos < (y_start + icon_hight); y_pos++) 
    {
        for(x_pos = x_start; x_pos < (x_start + (icon_width)/4); x_pos++) {
            byte_count = (((y_pos - 1) * 60) + x_pos) + 25;
            spiffs_read_buffer[byte_count] = (temp_string_1.charAt(string_pos));
            string_pos++;
        }
    }
}

void add_icon_data_battery_status(uint16_t x, uint8_t y) {
    if(battery_voltage > 4000) 
    {
        add_icon_data(Batt_100, x, y);
    } else if(battery_voltage > 3920 ) 
    {
        add_icon_data(Batt_80, x, y);
    }  else if(battery_voltage > 3850 )
    {
        add_icon_data(Batt_60, x, y);
    }
    else if(battery_voltage > 3790 )
    {
        add_icon_data(Batt_40, x, y);
    }
    else if(battery_voltage > 3750 )
    {
        add_icon_data(Batt_20, x, y);
    }
    else
    {
        add_icon_data(Batt_00, x, y);
    }

    /*
    100%----4.20V
    90%-----4.06V
    80%-----3.98V
    70%-----3.92V
    60%-----3.87V
    50%-----3.82V
    40%-----3.79V
    30%-----3.77V
    20%-----3.74V
    10%-----3.68V
    5%------3.45V
    0%------3.00V
    */
}         

/* Please Don't delete this block!

void compress_icon_data(String icon_data)
{
    uint16_t length = icon_data.length();
    uint8_t _0_count = 0, _F_count = 0;
    char character;

    for(uint8_t i = 0; i < 6; i++)
    {
        character = icon_data.charAt(i);
        Serial.print(character);
    }
    for(uint16_t i = 6; i < length+1; i++)
    {
        character = icon_data.charAt(i);

        if(character == '0') {
            _0_count++;
        }
        else{
            uint8_t _0_count_x = _0_count / 9;
            for(uint8_t k = 0; k < _0_count_x; k++) 
            {
                Serial.print("09");
            }
            _0_count_x = _0_count % 9;
            if(_0_count_x) {
                Serial.print('0');
                Serial.print(_0_count_x);
                _0_count = 0;
            }
            else {
                _0_count = 0;  
            }
        }
        
        if(character == 'F') {
            _F_count++;
        }
        else{
            uint8_t _F_count_x = _F_count / 9;
            for(uint8_t k = 0; k < _F_count_x; k++) 
            {
                Serial.print("F9");
            }
            _F_count_x = _F_count % 9;
            if(_F_count_x) {
                Serial.print('F');
                Serial.print(_F_count_x);
                _F_count = 0;
            }
            else {
                _F_count = 0;  
            }
        }
        if(character != '0' && character != 'F') 
        {
            Serial.print(character);
        }
    }   
    Serial.println();
}
*/
