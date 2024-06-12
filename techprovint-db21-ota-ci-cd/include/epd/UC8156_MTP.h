#ifndef UC8156_MTP_H
#define UC8156_MTP_H

#include <cstring> // Include for strcmp function

// Define DISPLAY_TYPE enum
/*enum DISPLAY_TYPE {
    S011_T1_1,
    S011_T1_2,
    S014_T1_1,
    S014_T1_2,
    S021_T1_1,
    S025_T1_1,
    S031_T1_1,
    D011_T1_1,
    UNKNOWN
};*/

// Function to read MTP address
uint8_t read_MTP_address(const uint16_t address) {
    spi_write_command_2params(0x41, address & 0xFF, (address >> 8) & 0x07); // set MTP address
    return spi_read_command_1param_1dummy(0x43);
}

// Function to convert string to DISPLAY_TYPE
DISPLAY_TYPE convert_string_to_DISPLAY_TYPE(const char *display_type_string) {
    if (strcmp(display_type_string, "S011_T1.1") == 0)
        return S011_T1_1;
    if (strcmp(display_type_string, "S011_T1.2") == 0)
        return S011_T1_2;
    if (strcmp(display_type_string, "S014_T1.1") == 0)
        return S014_T1_1;
    if (strcmp(display_type_string, "S014_T1.2") == 0)
        return S014_T1_2;
    if (strcmp(display_type_string, "S021_T1.1") == 0)
        return S021_T1_1;
    if (strcmp(display_type_string, "S021_T3.1") == 0) //****change 2024.06.11 */
        return S021_T3_1;//****change 2024.06.11 */
    if (strcmp(display_type_string, "S025_T1.1") == 0)
        return S025_T1_1;
    if (strcmp(display_type_string, "S031_T1.1") == 0)
        return S031_T1_1;
    if (strcmp(display_type_string, "D011_T1.1") == 0)
        return D011_T1_1;

    return UNKNOWN;
}

// Function to read display type from MTP
/*DISPLAY_TYPE read_display_type_from_MTP() {
    char display_type_string[10];
    DISPLAY_TYPE display_type_enum = UNKNOWN;
    uint16_t start_address = 0x4f0;
    int i;

    uint8_t backup_reg40h = spi_read_command_1param(0x40);

    // switch to "type2" MTP area
    spi_write_command_1param(0x40, spi_read_command_1param(0x40) | 0x02);

    // first try to read display type directly
    for (i = 0; i < 9; i++) {
        uint8_t tmp = read_MTP_address(start_address + i);
        display_type_string[i] = tmp;
    }
    display_type_string[9] = '\0';
    display_type_enum = convert_string_to_DISPLAY_TYPE(display_type_string);

    // if not successful, read from MTP using waveform filename
    if (display_type_enum == UNKNOWN) {
        start_address = 0x4d0;
        for (i = 0; i < 9; i++) {
            display_type_string[i] = read_MTP_address(start_address + i);
        }
        display_type_string[9] = '\0';
        display_type_enum = convert_string_to_DISPLAY_TYPE(display_type_string);
    }

    log_message(DEBUG_EPD, "EPD Type: " + String(display_type_enum));

    // restore Reg[40h] value
    spi_write_command_1param(0x40, backup_reg40h);

    return display_type_enum;
}
*/
DISPLAY_TYPE read_display_type_from_MTP() {
    char display_type_string[10];
    DISPLAY_TYPE display_type_enum = UNKNOWN;
    uint16_t start_address = 0x4f0;
    int i;

    uint8_t backup_reg40h = spi_read_command_1param(0x40);

    Serial.print("Initial Reg[40h] value: ");
    Serial.println(backup_reg40h, HEX);

    // switch to "type2" MTP area
    uint8_t new_reg40h_value = spi_read_command_1param(0x40) | 0x02;
    spi_write_command_1param(0x40, new_reg40h_value);

    Serial.print("New Reg[40h] value: ");
    Serial.println(new_reg40h_value, HEX);

    // first try to read display type directly
    for (i = 0; i < 9; i++) {
        uint8_t tmp = read_MTP_address(start_address + i);
        display_type_string[i] = tmp;
        Serial.print("Read byte ");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(tmp, HEX);
    }
    display_type_string[9] = '\0';
    display_type_enum = convert_string_to_DISPLAY_TYPE(display_type_string);

    Serial.print("Direct read display type string: ");
    Serial.println(display_type_string);
    Serial.print("Converted display type enum: ");
    Serial.println(display_type_enum);

    // if not successful, read from MTP using waveform filename
    if (display_type_enum == UNKNOWN) {
        start_address = 0x4d0;
        Serial.println("Direct read failed, trying waveform filename.");
        for (i = 0; i < 9; i++) {
            display_type_string[i] = read_MTP_address(start_address + i);
            Serial.print("Read byte ");
            Serial.print(i);
            Serial.print(": ");
            Serial.println(display_type_string[i], HEX);
        }
        display_type_string[9] = '\0';
        display_type_enum = convert_string_to_DISPLAY_TYPE(display_type_string);

        Serial.print("Waveform filename read display type string: ");
        Serial.println(display_type_string);
        Serial.print("Converted display type enum: ");
        Serial.println(display_type_enum);
    }

    log_message(DEBUG_EPD, "EPD Type: " + String(display_type_enum));

    // restore Reg[40h] value
    spi_write_command_1param(0x40, backup_reg40h);

    Serial.print("Restored Reg[40h] value: ");
    Serial.println(backup_reg40h, HEX);

    return display_type_enum;
}

#endif /* UC8156_MTP_H */
