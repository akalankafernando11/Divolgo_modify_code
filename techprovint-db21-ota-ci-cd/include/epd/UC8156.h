#ifndef UC8156_H
#define UC8156_H 

#define GATE_LINES_MAX 160
#define SOURCE_LINES_MAX 240

enum UPDATE_MODES {FULL_UPDATE=0x00, PARTIAL_UPDATE=0x01, EPD_UPD_MONO=0x02, INIT_UPDATE=0x10}; // Reg[14h]
enum WAVEFORM_TYPES {NORMAL_4GL=0x00, FAST_2GL=0x02}; // Reg[40h]
enum WAVEFORM_MODES {WAVEFORM_FROM_LUT=0x00, WAVEFORM_FROM_MTP=0x03}; // Reg[14h]
enum TRANSPARENCY_KEY_VALUE { TRANSPARENCY_GS0 = 0x00,  TRANSPARENCY_GS1 = 0x40, TRANSPARENCY_GS2 = 0x80,TRANSPARENCY_GS3 = 0xc0 }; // Reg[14h]
enum TRANSPARENCY_DISPLAY_ENABLE {  TRANSPARENCY_DISPLAY_ENABLE_OFF = 0x00,  TRANSPARENCY_DISPLAY_ENABLE_ON = 0x08 };  // Reg[14h]
enum DISPLAY_MODE_SELECT {FULL_DISPLAY_UPDATE = 0x00, INITIAL_UPDATE = 0x10, AREA_DISPLAY_UPDATE = 0x20, AREA_DISPLAY_UPDATE_DISABLE_NON_SELECT_GATE_LINE = 0x30};
#define WAVEFORM_LENGTH 120
#define TS_LENGTH 10
int adjustedV;
//global variables
uint8_t UPDATE_COMMAND_WAVEFORMSOURCESELECT_PARAM = WAVEFORM_FROM_MTP;
extern uint16_t PIXEL_COUNT; //global variable
extern regSetting_t *REG_SETTINGS; //global variable
extern uint8_t NUMBER_OF_REGISTER_OVERWRITES; //global variable


bool UC8156_wait_for_BUSY_inactive()
{
	log_message(DEBUG_EPD, "Now > UC8156_wait_for_BUSY_inactive");

	unsigned long counter=0;	 
	while (digitalRead(EPD_BUSY_N) == 0) // BUSY loop
 	{
 		delay_ms(1);
		counter++; // BUSY loop
		if (counter>2000) 
			log_message(DEBUG_ERROR, "Busy-Loop Timeout");
			return false;
 	}
 	log_message(DEBUG_EPD, "UC8156_wait_for: " + String(counter));
 	
	return true;
}

void UC8156_hardware_reset()
{
	log_message(DEBUG_EPD, "Now > UC8156_hardware_reset");

	digitalWrite(EPD_RST, LOW);
	delay_ms(1);
	digitalWrite(EPD_RST, HIGH);
}

bool UC8156_check_RevID()
{
	log_message(DEBUG_EPD, "Now > UC8156_check_RevID");

	uint8_t revID = spi_read_command_1param(0x00);
	
	log_message(DEBUG_EPD, "RevID = " + String(revID, HEX));

	if (revID != 0x56)
	{
		log_message(DEBUG_ERROR, "RevID 0x56 not read correctly");
		return false;
	}

	return true;
}

// UC8156 change registers which need values different from power-up values
void UC8156_init_registers()
{
	log_message(DEBUG_EPD, "Now > UC8156_init_registers");

	int i;
	for (i=0; i<NUMBER_OF_REGISTER_OVERWRITES; i++)
	    spi_write_command((REG_SETTINGS+i)->addr, (REG_SETTINGS+i)->val, (REG_SETTINGS+i)->valCount);
}

bool UC8156_check_status_register(uint8_t expected_value)
{
	log_message(DEBUG_EPD, "Now > UC8156_check_status_register");

	uint8_t status_reg_value = spi_read_command_1param(0x15);

	log_message(DEBUG_EPD, "status_reg_value: " + String(status_reg_value));

	if (status_reg_value != expected_value) //check Status Register
	{
		log_message(DEBUG_ERROR, "Status Register not expected value: " + String(status_reg_value) + "/" + String(expected_value));
		// sprintf(error_message, "Status Register not %x but %x.\n", expected_value, status_reg_value);
		// abort_now(error_message, ABORT_UC8156_INIT);
		return false;
	}
	return true;
}

void setSourceVoltage(int v) { //****change 2024.06.11 */
        Serial.print("Original voltage: ");
    Serial.println(v);
    v = round(v / 1000);
        Serial.print("Rounded voltage (kV): ");
    Serial.println(v);
    int x = (v - 8) / 1 * 2 << 4;
        Serial.print("Intermediate value (x): ");
    Serial.println(x, HEX);
    x = x | (v - 8) / 1 * 2;
        Serial.print("Final value (x) after OR operation: ");
    Serial.println(x, HEX);
	spi_write_command_1param(0x02, 0x25);
} //****change 2024.06.11 */


void setTPCOM(int v) { //****change 2024.06.11 */
    if (v >= -2800){
        adjustedV = (v + 2800) / 30;
        spi_write_command_2params(0x1B, lowByte((v + 2800) / 30), highByte((v + 2800) / 30));
    }else{
        adjustedV = (v + 2800) / 30 * (-1);
        spi_write_command_2params(0x1B, lowByte((v + 2800) / 30 * (-1)), highByte((v + 2800) / 30 * (-1)) | 4);
    }
} //****change 2024.06.11 */
// send Vcom value (in mV) to UC8156
/*void UC8156_set_Vcom(int Vcom_mv_value)
{
	log_message(DEBUG_EPD, "Now > UC8156_set_Vcom");

	uint16_t Vcom_register_value = (float)Vcom_mv_value/(float)30.0;
	spi_write_command_2params(0x1B, (uint8_t)Vcom_register_value, (uint8_t)((Vcom_register_value>>8)&0x03));
}*/

// send waveform to UC8156
void UC8156_send_waveform(uint8_t *waveform)
{
	log_message(DEBUG_EPD, "Now > UC8156_send_waveform");

	spi_write_command_and_bulk_data(0x1C, waveform, WAVEFORM_LENGTH);
}

// waits for Power_ON RDY
unsigned long UC8156_wait_for_PowerON_ready()
 {
 	log_message(DEBUG_EPD, "Now > UC8156_wait_for_PowerON_ready");

	 unsigned long counter=0;
 	while (spi_read_command_1param(0x15)!=4 && counter<1000)
 	{
  		delay_ms(1);
 		counter++; // BUSY loop
  	}
 	return(counter);
 }

// UC8156 HV power-on (enable charge pumps, execute power-on sequence for outputs)
void UC8156_HVs_on()
{
	log_message(DEBUG_EPD, "Now > UC8156_HVs_on");

	uint8_t reg_value = spi_read_command_1param(0x03); //read power control setting register
	reg_value |= 0x11; //switch on CLKEN+PWRON bits
	spi_write_command_4params(0x1c, 0, 239, 0, 159); //****change 2024.06.11 */
	spi_write_command_2params(0x06, 0x67, 0x55);//****change 2024.06.11 */
	spi_write_command_3params(0x1f, 0x00, 0x00, 0x00);//****change 2024.06.11 */
	spi_write_command_1param (0x03, 0xD1);//reg_value); //write power control setting register --> switch on CLKEN+PWRON bits //****change 2024.06.11 */

	UC8156_wait_for_PowerON_ready();
//	UC8156_wait_for_PowerON_ready_debug();
}

// UC8156 power-off sequence
void UC8156_HVs_off()
{
	log_message(DEBUG_EPD, "Now > UC8156_HVs_off");

	uint8_t reg_value = spi_read_command_1param(0x03); //read power control setting register
	reg_value &= ~0x01; //switch off PWRON bit
	spi_write_command_1param (0x03, 0xD0); //write power control setting register //****change 2024.06.11 */
	UC8156_wait_for_BUSY_inactive();
	reg_value &= ~0x10; //switch off CLKEN bit
	spi_write_command_1param (0x03, 0xC0);  //****change 2024.06.11 */
	UC8156_wait_for_BUSY_inactive(); //****change 2024.06.11 */
}


//update display and wait for BUSY-pin low
void UC8156_update_display(uint8_t update_mode, uint8_t waveform_mode)
{
	log_message(DEBUG_EPD, "Now > UC8156_update_display");

	//spi_write_command_1param(0x40, spi_read_command_1param(0x40) | waveform_mode);
	//spi_write_command_1param(0x14, UPDATE_COMMAND_WAVEFORMSOURCESELECT_PARAM | update_mode | 1 );
	            spi_write_command_1param(0x40, 0x00); //****change 2024.06.11 */
            spi_write_command_1param(0x14, 0x03); //****change 2024.06.11 */
	//spi_write_command_1param(0x14, UPDATE_COMMAND_WAVEFORMSOURCESELECT_PARAM | update_mode | 1 | 0x20); // test area update mode
	UC8156_wait_for_BUSY_inactive();
	//UC8156_wait_for_BUSY_inactive_debug();
}

//send an repeated byte to the image buffer --> used to create a solid image like all white
void UC8156_send_repeated_image_data(uint8_t image_data)
{
	log_message(DEBUG_EPD, "Now > UC8156_send_repeated_image_data");

	spi_write_command_byte_repeat(0x10, image_data, PIXEL_COUNT/4);
}

//send an image to UC8156 image data memory
void UC8156_send_image_data(uint8_t *image_data)
{
	log_message(DEBUG_EPD, "Now > UC8156_send_image_data");

	spi_write_command_and_image_data(0x10, image_data, PIXEL_COUNT/4);
}

void UC8156_update_display_with_power_on_off(uint8_t update_mode, uint8_t waveform_mode)
{
	log_message(DEBUG_EPD, "Now > UC8156_update_display_with_power_on_off");
	
	UC8156_HVs_on();
	UC8156_update_display(update_mode, waveform_mode);
    UC8156_HVs_off();
}

void UC8156_show_image(uint8_t *image_data, uint8_t update_mode, uint8_t waveform_mode)
{
	log_message(DEBUG_EPD, "Now > UC8156_show_image");
	
	UC8156_send_image_data(image_data);
	UC8156_update_display_with_power_on_off(update_mode, waveform_mode);
}

#endif /* UC8156_H  */