#ifndef TEMPERATURE_READ
#define TEMPERATURE_READ

#define Addr1 0x48 // 1001000
#define Addr2 0x4A // 1001010


float read_temperature_sensor(bool id) {
    unsigned int data1[2];
    unsigned int data2[2];

    uint8_t address;
    if (id == 0) {
        address = Addr1;
    } else {
        address = Addr2;
    }

    // Start I2C Transmission Addr1
    I2Ctwo.beginTransmission(address);
    // Select data register
    I2Ctwo.write(0x00);
    // Stop I2C Transmission
    I2Ctwo.endTransmission();
    // Request 2 bytes of data
    I2Ctwo.requestFrom(address, 2);
    // Read 2 bytes of data
    // cTemp msb, cTemp lsb
    if(I2Ctwo.available() == 2) {
    data1[0] = I2Ctwo.read();
    data1[1] = I2Ctwo.read();
    }
    // Convert the data
    float cTemp1 = (((data1[0] * 256) + (data1[1] & 0xF0)) / 16) * 0.0625;
    // float fTemp1 = cTemp1 * 1.8 + 32; 

    return cTemp1;
}

#endif