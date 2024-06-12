/*#ifndef FLOW_BASIS_H
#define FLOW_BASIS_H  

void basic_flow(int display_color)
{
	enum DISPLAY_TYPE display_type;
	uint8_t waveform_from_file[WAVEFORM_LENGTH];
	char path[64];

	UC8156_hardware_reset(); // UC8156 hardware reset
	UC8156_wait_for_BUSY_inactive(); // wait for power-up completed

	// 1st try to read display-type from MTP
	display_type = read_display_type_from_MTP();

	if (display_type == UNKNOWN)
	{
		Serial.println("display_type: UNKNOWN");
		return 0;
	}

	set_display_type(display_type);

	UC8156_wait_for_BUSY_inactive(); // wait for power-up completed

	delay_ms(5);

	UC8156_hardware_reset(); // UC8156 hardware reset

	UC8156_wait_for_BUSY_inactive(); // wait for power-up completed

	UC8156_check_RevID(); // optional -> verifies successful power-up

	UC8156_init_registers(); // over-writes some register values with display-specific values

	// optional -> check for possible problems
	UC8156_check_status_register(0x00);

	// sprintf(path2, "/%s/%s", PATH, "display/S021_T1.1_SPP0B9_V0.uc8156_type2");
    // sprintf(path1, "/%s/%s", PATH, "display/S021_T1.1_SPP0B9_V0.uc8156_type1");
    UPDATE_COMMAND_WAVEFORMSOURCESELECT_PARAM =  WAVEFORM_FROM_LUT;                             // waveform read from sd-card, 2 waveforms for tricolor


	// u8 current_vcom_u8 = print_current_VCOM();   // get the Vcom from display
	// current_vcom = current_vcom_u8 * 30;        // set the Vcom, unit: mV

	// register_vcom_set();

	int vcom_mv_value = 3900;
	UC8156_set_Vcom(vcom_mv_value);

	// UC8156_send_waveform(waveform_data);


    // path_find();

	  if(display_color == 0)   // choose the type of display, 0: yellow;
	  {
	                    // find the path for the PGM picture.
	     tricolor_yellow();
	  }
	  else if (display_color == 1)  // 1: red
 	  {
	      //tricolor_red_path();           // find the path for the PGM picture.
	     // tricolor_red(FULL_UPDATE);
	    // tricolor_red_partial(PARTIAL_UPDATE);
	     //tricolor_red_partial(FULL_UPDATE);
	     //tricolor_red_all(FULL_UPDATE);
	   //  tricolor_red_opti(FULL_UPDATE);
	      tricolor_yellow_test();
	  }

	  else if (display_color == 2)
	  {
	      tricolor_binary();
	  }

}



bool eval_kit_flow(void) {
	enum DISPLAY_TYPE display_type;
	uint8_t waveform_from_file[WAVEFORM_LENGTH];
	char path[64];

	UC8156_hardware_reset(); // UC8156 hardware reset
	UC8156_wait_for_BUSY_inactive(); // wait for power-up completed

	// 1st try to read display-type from MTP
	display_type = read_display_type_from_MTP();

	if (display_type == UNKNOWN)
	{
		Serial.println("display_type: UNKNOWN");
		return 0;
	}

	set_display_type(display_type);

	UC8156_wait_for_BUSY_inactive(); // wait for power-up completed

	delay_ms(5);

	UC8156_hardware_reset(); // UC8156 hardware reset

	UC8156_wait_for_BUSY_inactive(); // wait for power-up completed

	UC8156_check_RevID(); // optional -> verifies successful power-up

	UC8156_init_registers(); // over-writes some register values with display-specific values
	
	UC8156_check_status_register(0x00); // optional -> check for possible problems

	int vcom_mv_value = 3900;
	UC8156_set_Vcom(vcom_mv_value);

	UC8156_send_waveform(waveform_data);
	UPDATE_COMMAND_WAVEFORMSOURCESELECT_PARAM =  WAVEFORM_FROM_LUT;

	// while(1) {
		clear_display();

		delay_ms(2000);

		UC8156_show_image(image_data, FULL_UPDATE, NORMAL_4GL);
		Serial.println("displayed");

		delay_ms(2000);
	// }
	// black_display();

	// while(1){}
}

#endif /* FLOW_BASIS_H */