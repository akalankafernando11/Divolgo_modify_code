#ifndef LC709203F_H
#define LC709203F_H

#define BATTERY_MONITOR_ADDRESS   0x0B

uint16_t getVoltage(void) 
{
    return read_i2c_16(BATTERY_MONITOR_ADDRESS, 0x09);
}

uint8_t getCapacity(void) 
{
    return (read_i2c_16(BATTERY_MONITOR_ADDRESS, 0x0F)) * 0.1;
}

uint16_t getBConst(void) 
{
    return read_i2c_16(BATTERY_MONITOR_ADDRESS, 0x06);
}

// uint16_t getCellTemperature(void) 
// {
//     write_i2c_16(BATTERY_MONITOR_ADDRESS, 0x16, 0x0001);
     
//     return read_i2c_16(BATTERY_MONITOR_ADDRESS, 0x08);
// }

uint16_t getDirection(void) 
{
    return read_i2c_16(BATTERY_MONITOR_ADDRESS, 0x0A);
}

uint16_t getAPT(void) 
{
    return read_i2c_16(BATTERY_MONITOR_ADDRESS, 0x0C);
}

uint16_t getRSOC(void) 
{
    return read_i2c_16(BATTERY_MONITOR_ADDRESS, 0x0D);
}

uint16_t getICVersion(void) 
{
    return read_i2c_16(BATTERY_MONITOR_ADDRESS, 0x11);
}

uint16_t getProfCode(void) 
{
    return read_i2c_16(BATTERY_MONITOR_ADDRESS, 0x1A);
}

void setAlarm(uint8_t value) 
{
    write_i2c_16(BATTERY_MONITOR_ADDRESS, 0x13, value);
}

void setVoltageAlarm(uint16_t value) 
{
    write_i2c_16(BATTERY_MONITOR_ADDRESS, 0x14, value);
}

void adjust(uint16_t value) 
{
    write_i2c_16(BATTERY_MONITOR_ADDRESS, 0x0B, value);
}




bool initBattMon() {
    if (read_i2c_16(BATTERY_MONITOR_ADDRESS, 0x11) == 0x00)
        return false;
    // delay_ms(1);
    // write_i2c_16(BATTERY_MONITOR_ADDRESS, 0x04, 0xAA55);        //set initial RSOC (necessary?)
    // delay_ms(1);
    // write_i2c_16(BATTERY_MONITOR_ADDRESS, 0x07, 0xAA55);        //set initial RSOC (necessary?)
    // delay_ms(1);
    write_i2c_16(BATTERY_MONITOR_ADDRESS, 0x15, 0x0001);        //set power mode : Operating
    write_i2c_16(BATTERY_MONITOR_ADDRESS, 0x0B, 0x000E);        //set Adjustment Pack Application Value
    write_i2c_16(BATTERY_MONITOR_ADDRESS, 0x12, 0x0001);        //choose default battery profile (see datasheet)
    write_i2c_16(BATTERY_MONITOR_ADDRESS, 0x04, 0xAA55);        //set initial RSOC (necessary?)
    write_i2c_16(BATTERY_MONITOR_ADDRESS, 0x07, 0xAA55);        //set initial RSOC (necessary?)
    write_i2c_16(BATTERY_MONITOR_ADDRESS, 0x16, 0x0001);        //Enable thermistor
    write_i2c_16(BATTERY_MONITOR_ADDRESS, 0x06, 0x0D49);        //Sets B−constant of the thermistor to be measured (0x0D49 = 3401K)
    
    // write_i2c_16(BATTERY_MONITOR_ADDRESS, 0x0A, 0x0000);        //set auto current detection mode
    // write_i2c_16(BATTERY_MONITOR_ADDRESS, 0x0B, 0x000E);        //set auto current detection mode
    // write_i2c_16(BATTERY_MONITOR_ADDRESS, 0x13, 0x0005);        //activate alarm if RSOC lower than 5%
    // write_i2c_16(BATTERY_MONITOR_ADDRESS, 0x14, 0x3500);        //activate alarm if voltage under 2,8volts
    return true;
}



#endif /* LC709203F_H */
