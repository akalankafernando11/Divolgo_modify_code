
#include "epd/waveform_data.h"
#include "epd/epd_types.h"
#include "epd/config_display.h"
#include "epd/epd_spi.h"
#include "epd/UC8156.h"
#include "epd/display_functions.h"
#include "epd/UC8156_MTP.h"

void epd_gpio_init(){
	// digitalWrite(EPD_PWR_EN, LOW);
	digitalWrite(EPD_RST, 	LOW);
	pinMode(EPD_PWR_EN, 	OUTPUT);
	pinMode(EPD_RST, 		OUTPUT);
	pinMode(EPD_BUSY_N, 	INPUT_PULLUP);
	
	delay_ms(20);
}

void HBZ9_3V3_enable(){
	digitalWrite(EPD_PWR_EN, HIGH);
	delay_ms(10);
    digitalWrite(EPD_RST, 	HIGH);
    delay_ms(10);
}

void HBZ9_3V3_desable(){
	digitalWrite(EPD_PWR_EN, LOW);
	delay_ms(10);
    digitalWrite(EPD_RST, 	LOW);
    delay_ms(10);
}

void add_to_image_buffer_little_endian(uint8_t read_byte, uint16_t buffer_position) {
	uint8_t bit_count = 0;
	uint8_t bit_count_2 = 0;
	uint8_t pixel_byte = 0;
	bool pixel_val;

	if(IMAGE_ORIENTATION == 0) {
		while(bit_count < 4) {
			pixel_val = read_byte & (0x01 << bit_count);
			bit_count++;
			pixel_byte = pixel_byte | (pixel_val << bit_count_2);
			bit_count_2++;
			pixel_byte = pixel_byte | (pixel_val << bit_count_2);
			bit_count_2++;
		}
	} else if(IMAGE_ORIENTATION == 2) {
		while(bit_count < 4) {
			pixel_val = read_byte & (0x08 >> bit_count);
			bit_count++;
			pixel_byte = pixel_byte | (pixel_val << bit_count_2);
			bit_count_2++;
			pixel_byte = pixel_byte | (pixel_val << bit_count_2);
			bit_count_2++;
		}
	}

	image_buffer[buffer_position] = pixel_byte;
}

bool image_data_crete(String image_name) {
    if(image_name != "imformation_display")
    {
        if (!read_file_to_array(SPIFFS, "/", image_name, IMAGE_FILE_SIZE)) {
            return false;
        }
    }

	if(IMAGE_ORIENTATION == 0) {
		int array_no;
		uint8_t read_val;
		uint16_t buffer_position = 0;

		timerWrite(timer_w, 0); //reset timer (feed watchdog)

		for(size_t size = 25; size < 8786; size++) {
			read_val = spiffs_read_buffer[size];
			
			if (read_val > 59) {
				read_val = read_val - 55;
			} else
			{
				read_val = read_val - 48;
			}
				
			if (!(array_no % 2)) {
				add_to_image_buffer_little_endian(read_val, buffer_position);
				buffer_position++;
			} else {	
				add_to_image_buffer_little_endian(read_val, buffer_position);
				buffer_position++;
			}
		}
		
		return true;
	}

	if(IMAGE_ORIENTATION == 2) {
		int array_no;
		uint8_t read_val;
		uint16_t buffer_position = 8760;

		timerWrite(timer_w, 0); //reset timer (feed watchdog)

		for(size_t size = 24; size < 8786; size++) {
			read_val = spiffs_read_buffer[size];
			
			if (read_val > 59) {
				read_val = read_val - 55;
			} else
			{
				read_val = read_val - 48;
			}
				
			if (!(array_no % 2)) {
				add_to_image_buffer_little_endian(read_val, buffer_position);
				buffer_position--;
			} else {	
				add_to_image_buffer_little_endian(read_val, buffer_position);
				buffer_position--;
			}
		}
		return true;
	}
	return true;
}

bool image_change_status = 0;
uint8_t display_fail_count = 0;

bool display_image_file(String image_name) {
	enum DISPLAY_TYPE display_type;

	if ((image_name != "pattern") & (image_name != "white") & (image_name != "black") ) { 
		clear(true);
		if(!image_data_crete(image_name)){
			update_status_code(8003);
			return false;
		}
	}
	
	SPI.begin(SPI_CLK, SPI_MISO, SPI_MOSI, SPI_CS);
  	SPI.beginTransaction(SPISettings(9600000, MSBFIRST, SPI_MODE0));
	
	epd_gpio_init();

	HBZ9_3V3_enable();

	delay_ms(500);

	display_type = read_display_type_from_MTP();
	if (display_type == UNKNOWN)
	{
		log_message(DEBUG_ERROR, "display_type: UNKNOWN");
		display_type = S021_T1_1;

		SPI.endTransaction();
    	SPI.end();
		delay_ms(5);
		HBZ9_3V3_desable();
		delay(500);

		display_fail_count++;
		if(display_fail_count > 1) {
			return 0;
		}

		display_image_file(image_name);
	}
	if(image_change_status) {
		return 1;
	}
	delay_ms(5);
	set_display_type(display_type);
	delay_ms(5);
	if(!UC8156_wait_for_BUSY_inactive()) { // wait for power-up completed
		update_status_code(8501);
		return false;
	}

	delay_ms(5);

	UC8156_hardware_reset(); // UC8156 hardware reset
	delay_ms(5);
	if(!UC8156_wait_for_BUSY_inactive()) { // wait for power-up completed
		update_status_code(8501);
		return false;
	}
	delay_ms(5);
	if(!UC8156_check_RevID()){ // optional -> verifies successful power-up
		update_status_code(8502);
		return false;
	}
	delay_ms(5);
	UC8156_init_registers(); // over-writes some register values with display-specific values
	delay_ms(5);
	if(!UC8156_check_status_register(0x00)) { // optional -> check for possible problems
		update_status_code(8503);
		return false;	
	}
	delay_ms(5);
	//int vcom_mv_value = 3900;
	//UC8156_set_Vcom(vcom_mv_value);
	setTPCOM(0); //****change 2024.06.11 */
    setSourceVoltage(20000); //****change 2024.06.11 */
	clear_display();
	clear_display();
	delay_ms(5);
	UC8156_send_waveform(waveform_data);
	UPDATE_COMMAND_WAVEFORMSOURCESELECT_PARAM =  WAVEFORM_FROM_LUT;
	delay_ms(5);
	
	if (image_name == "white"){ 
		clear_display();
		delay_ms(1500);
	} else if(image_name == "black") {
		black_display();
		delay_ms(1500);
	} else {
		UC8156_show_image(image_buffer, FULL_UPDATE, NORMAL_4GL);
		delay_ms(1000);
		UC8156_show_image(image_buffer, FULL_UPDATE, NORMAL_4GL);
		delay_ms(1500);
	}
	
	log_message(DEBUG_EPD, "Displayed !");
	image_change_status = 1;

	return true;
}

bool display_image(String image_name) {

	image_change_status = 0;

	log_message(DEBUG_MSG, "Image changing: " + image_name);
	time_t timer_initial = timer_start();
	time_t image_change_duration = 0;
	bool return_val = 0;

    timerWrite(timer_w, 0); //reset timer (feed watchdog)

    if ((image_name != "pattern") && (image_name != "white") && (image_name != "black") && (image_name != "imformation_display"))
    {
        if (!check_availability(SPIFFS, "/", image_name)) {
            //  log_message(DEBUG_ERROR, "image not available");
            update_status_code(8001);
            return false;
        }
        if (!validate_file_size(SPIFFS, "/", image_name, IMAGE_FILE_SIZE, 0)) {
            //  log_message(DEBUG_ERROR, "image size validation failed");
            update_status_code(8805);
            return false;
        }
    }

	if(display_image_file(image_name))
	{
		return_val = true;
	} else 
	{	
		return_val = false;
	}

	SPI.endTransaction();
    SPI.end();
	delay_ms(5);

	HBZ9_3V3_desable();

	image_change_duration = timer_stop(timer_initial);
	if(image_change_duration > 7000) {
		update_status_code(8504);
		return_val = false;
	}

	if (return_val)
	{
		log_message(DEBUG_MSG,  "Image change STATUS = Success");
	} 
	else
	{
		log_message(DEBUG_MSG,  "Image change STATUS = Fail");
	}
	return return_val;
}

void power_off_display_update(){
    add_white_background_data();
    add_icon_data_battery_status(180, 20);
    add_icon_data(Device_Off, 150, 18);
    display_image("imformation_display");
}

void power_off_display_update_Low_Batt()
{
    add_white_background_data();
    add_icon_data_battery_status(180, 20);
    add_icon_data(warning, 197, 22);
    add_icon_data(Device_Off, 150, 18);
    display_image("imformation_display");
}
 