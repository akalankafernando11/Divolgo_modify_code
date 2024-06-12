#ifndef EPD_SPI_H
#define EPD_SPI_H  

uint8_t spi_read_command_1param(uint8_t command)
{   uint8_t read_data = 0;

    digitalWrite(SPI_CS, LOW);
    command |= 0x80;
    SPI.write(command);
    read_data += SPI.transfer(0x00);
    digitalWrite(SPI_CS, HIGH);
    return read_data;
}

void spi_write_command_1param(uint8_t command, uint8_t param1)
{
    digitalWrite(SPI_CS, LOW);
    command &= ~0x80;
    SPI.write(command);
    SPI.write(param1);
    digitalWrite(SPI_CS, HIGH);
}

void spi_write_command_2params(uint8_t command, uint8_t param1, uint8_t param2)
{
    digitalWrite(SPI_CS, LOW);
    command &= ~0x80;
    SPI.write(command);
    SPI.write(param1);
    SPI.write(param2);
    digitalWrite(SPI_CS, HIGH);
}

void spi_write_command_3params(uint8_t command, uint8_t param1, uint8_t param2, uint8_t param3)
{//****change 2024.06.11 */
    digitalWrite(SPI_CS, LOW);
    command &= ~0x80;
    SPI.write(command);
    SPI.write(param1);
    SPI.write(param2);
    SPI.write(param3);
    digitalWrite(SPI_CS, HIGH);
}//****change 2024.06.11 */

void spi_write_command_4params(uint8_t command, uint8_t param1, uint8_t param2, uint8_t param3, uint8_t param4)
{ //****change 2024.06.11 */
    digitalWrite(SPI_CS, LOW);
    command &= ~0x80;
    SPI.write(command);
    SPI.write(param1);
    SPI.write(param2);
    SPI.write(param3);
    SPI.write(param4);
    digitalWrite(SPI_CS, HIGH);
} //****change 2024.06.11 */

uint8_t spi_read_command_1param_1dummy(uint8_t command)
{
    uint8_t read_data = 0;
    digitalWrite(SPI_CS, LOW);
    command |= 0x80;
    SPI.write(command);
    SPI.write(command); //dummy
    read_data += SPI.transfer(0x00);
    digitalWrite(SPI_CS, HIGH);
    return read_data;
}

void spi_write_command(uint8_t command, uint8_t *params, int count)
{
    int i;

    digitalWrite(SPI_CS, LOW);
    command &= ~0x80;
    SPI.write(command);
    for (i=0; i<count; i++)
        SPI.write(*(params+i));
    digitalWrite(SPI_CS, HIGH);
}

void spi_write_command_and_bulk_data(uint8_t command, uint8_t *buffer, size_t size)
{
    digitalWrite(SPI_CS, LOW);
    command &= ~0x80;
    SPI.write(command);

    while (size--) {
        SPI.write(*buffer);
        // delay_msMicroseconds(10);
        buffer++;
    }

    digitalWrite(SPI_CS, HIGH);
}

uint8_t swap_byte(uint8_t byte_data) {
    uint8_t bit_endian_swap = 0;

    for(uint8_t i = 0; i <= 7; i++){
        bit_endian_swap <<= 1;
        bit_endian_swap |= byte_data & 1;
        byte_data >>= 1; 
    }
    return bit_endian_swap;
}

void spi_write_command_and_image_data(uint8_t command, uint8_t *buffer, size_t size)
{
    digitalWrite(SPI_CS, LOW);
    command &= ~0x80;
    uint32_t colomn_count = 0;
    int row_count = 0; 
    int byte_count = 0;

    SPI.write(command);

    while (size) {
        for(row_count = 30; row_count < 60; row_count++) {   
            byte_count =  colomn_count + row_count;
            SPI.write(buffer[byte_count]);
            // delay_msMicroseconds(10);
            size--;
        }
        for(row_count = 29; row_count >= 0; row_count--) {    
            byte_count =  colomn_count + row_count;
            SPI.write(swap_byte(buffer[byte_count]));
            // delay_msMicroseconds(10);
            size--;
        }
        colomn_count = colomn_count + 60;

    }

    digitalWrite(SPI_CS, HIGH);
}

void spi_write_command_byte_repeat(uint8_t command, uint8_t data, size_t size)
{
    digitalWrite(SPI_CS, LOW);
    command &= ~0x80;
    SPI.write(command);

    while (size--) {
        SPI.write(data);
    }
    digitalWrite(SPI_CS, HIGH);
}

#endif /* EPD_SPI_H */