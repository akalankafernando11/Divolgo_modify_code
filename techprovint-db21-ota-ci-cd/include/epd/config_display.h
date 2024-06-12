#ifndef CONFIG_DISPLAY_H
#define CONFIG_DISPLAY_H

enum DISPLAY_TYPE { S011_T1_1, S011_T1_2, S014_T1_1, S014_T1_2, S021_T1_1, S021_T3_1/****change 2024.06.11 */, S025_T1_1, S031_T1_1, D011_T1_1, UNKNOWN};
uint16_t GATE_LINES; //global variable
uint16_t SOURCE_LINES; //global variable
uint16_t PIXEL_COUNT; //global variable
regSetting_t *REG_SETTINGS; //global variable
uint8_t NUMBER_OF_REGISTER_OVERWRITES; //global variable



regSetting_t reg_settings_S011_T1_1[] =
{
		{0x01, 1, {0x12}},
		{0x02, 2, {0x25, 0xff}},
		{0x06, 2, {0x67, 0x55}},
		{0x07, 1, {0x0a}},
		{0x0c, 4, {0x00, 0xef, 0x00, 0x9f}}, // {0, 240-1, 0, 160-1}
		{0x0d, 4, {0x00, 0x45, 0x00, 0x93}}, // {0, 70-1, 0, 148-1}
		{0x0e, 2, {0x00, 0x93}},             // {0, 148-1}
		{0x0f, 1, {0x02}},
		{0x18, 4, {0x00, 0x00, 0x24, 0x07}},
		{0x1d, 1, {0x04}},
		{0x1f, 3, {0x00, 0x00, 0x00}},
		{0x44, 1, {0x60}}
};

regSetting_t reg_settings_S011_T1_2[] =
{
		{0x01, 1, {0x12}},
		{0x02, 2, {0x70, 0xaa}},			// set Vgate to +17V/-25V
		{0x06, 2, {0xff, 0x55}},			// set timing to LAT=105, S2G+G2S=5
		{0x07, 1, {0x0a}},
		{0x0c, 4, {0x00, 0x45, 0x00, 0x93}}, // physical 240x160 panel resolution setting
		{0x0d, 4, {0x00, 0x45, 0x00, 0x93}}, // {0, 70-1, 0, 148-1}
		{0x0e, 2, {0x00, 0x93}},             // {0, 148-1}
		{0x0f, 1, {0x02}},
		{0x18, 4, {0x00, 0x00, 0x24, 0x07}},
		{0x1d, 1, {0x04}},
		{0x1f, 3, {0x00, 0x00, 0x00}},
		{0x44, 1, {0x60}}
};

regSetting_t reg_settings_S014_T1_1[] =
{
		{0x01, 1, {0x12}},
		{0x02, 2, {0x25, 0xff}},
		{0x06, 2, {0x67, 0x55}},
		{0x07, 1, {0x0a}},
		{0x0c, 4, {0, 0xef, 0, 0x9f}},         // {0, 240-1, 0, 160-1}
		{0x0d, 4, {0, 0xb3, 0x3c, 0x9f}},      // {0, 180-1, 60, 160-1}
		{0x0e, 2, {0, 0x9f}},                  // {0, 60}
		{0x0f, 1, {0x02}},
		{0x18, 4, {0x00, 0x00, 0x24, 0x07}},
		{0x1d, 1, {0x04}},
		{0x1f, 3, {0x00, 0x00, 0x00}},
		{0x44, 1, {0x60}}
};

regSetting_t reg_settings_S014_T1_2[] =
{
		{0x01, 1, {0x12}},
		{0x02, 2, {0x70, 0xaa}},				// set Vgate to +17V/-25V
		{0x06, 2, {0xff, 0x55}},				// set timing to LAT=105, S2G+G2S=5
		{0x07, 1, {0x0a}},
		{0x0c, 4, {0, 0xb3, 0x3c, 0x9f}},       // physical 240x160 panel resolution setting
		{0x0d, 4, {0, 0xb3, 0x3c, 0x9f}},      // {0, 180-1, 60, 160-1}
		{0x0e, 2, {0, 0x9f}},                  // {0, 60}
		{0x0f, 1, {0x02}},
		{0x18, 4, {0x00, 0x00, 0x24, 0x07}},
		{0x1d, 1, {0x04}},
		{0x1f, 3, {0x00, 0x00, 0x00}},
		{0x44, 1, {0x60}}
};

regSetting_t reg_settings_S021_T1_1[] =
{
		{0x01, 1, {0x10}},
		{0x02, 2, {0x25, 0xff}},
		{0x06, 2, {0x67, 0x55}},
		{0x07, 1, {0x0a}},
		{0x0c, 4, {0, 0xef, 0, 0x9f}},         // {0, 240-1, 0, 160-1}
		{0x0d, 4, {0, 240-1, 0x00, 146-1}},      // {0, 180-1, 60, 160-1}
		{0x0e, 2, {0, 0}},                  // {0, 0}
		{0x0f, 1, {0x20}},
		{0x18, 4, {0x00, 0x00, 0x24, 0x07}},
		{0x1d, 1, {0x04}},
		{0x1f, 3, {0x00, 0x00, 0x00}},
		{0x44, 1, {0x60}}
};

regSetting_t reg_settings_S021_T3_1[] =    //****change 2024.06.11 */
{
		{0x01, 1, {0x10}},
		{0x0d, 4, {0, 240-1, 0x00, 146-1}},      // {0, 180-1, 60, 160-1}
		{0x18, 4, {0x00, 0x00, 0x24, 0x05}},
		{0x0e, 2, {0, 0}},                  // {0, 0}
		{0x0f, 1, {0x20}},
		{0x02, 2, {0x25, 0xff}},
		{0x1d, 1, {0x04}},
		{0x44, 1, {0x60}},
		{0x07, 1, {0x0a}},
		{0x04, 2, {0x22, 0x17}}
		//{0x0c, 4, {0, 0xef, 0, 0x9f}},         // {0, 240-1, 0, 160-1}
		//{0x06, 2, {0x67, 0x55}},
		//{0x1f, 3, {0x00, 0x00, 0x00}},	
}; //****change 2024.06.11 */

regSetting_t reg_settings_S025_T1_1[] =
{
        {0x01, 1, {0x12}},
        {0x02, 2, {0x70, 0xff}},
        {0x06, 2, {0x67, 0x55}},
        {0x07, 1, {0x0a}},
        {0x0c, 4, {0, 0xef, 0, 0x9f}},
        {0x0d, 4, {0, 0xef, 0x00, 0x9f}},
        {0x0e, 2, {0, 0}},
        {0x0f, 1, {0x00}},
        {0x18, 4, {0x00, 0x00, 0x24, 0x07}},
        {0x1d, 1, {0x04}},
        {0x1f, 3, {0x00, 0x00, 0x00}},
        {0x44, 1, {0x60}}
};

regSetting_t reg_settings_S031_T1_1[] =
{
		{0x01, 1, {0x12}},
		{0x02, 2, {0x25, 0xff}},
		{0x06, 2, {0x67, 0x55}},
		{0x07, 1, {0x0a}},
		{0x0c, 4, {0x00, 0xef, 0x00, 0x9f}}, // {0, 240-1, 0, 160-1}
		{0x0d, 4, {0x00, 0x93, 0x00, 0x9b}}, //{0, 148-1, 0, 156-1}
		{0x0e, 2, {0x00, 0x9b}},             //{0, 156-1}
		{0x0f, 1, {0x22}},
		{0x18, 4, {0x50,0x01,0x24,0x07}},
		{0x1d, 1, {0x04}},
		{0x1f, 3, {0x00, 0x00, 0x00}},
		{0x44, 1, {0x60}}
};

regSetting_t reg_settings_D011_T1_1[] =
{
		{0x01, 1, {0x10}}, //GVS=01b (gate active = VGL), SOO=0
		{0x02, 2, {0x25, 0xff}},
		{0x06, 2, {0x67, 0x55}},
		{0x07, 1, {0x0a}},
		{0x0c, 4, {0x00, 0xef, 0x00, 0x9f}},      // {0, 240-1, 0, 160-1}
		{0x0d, 4, {0x00, 0xef, 0x00, 0x73}},      // {0, 240-1, 0, 116-1}
		{0x0e, 2, {0x00, 0x00}},                  // {0, 0}
		{0x0f, 1, {0x20}}, //X-increment, Y-increment
		{0x18, 4, {0x00, 0x00, 0x24, 0x07}}, // TPCOM end of update -> GND, TPCOM_idle -> VCOM -> 70.0L*
		{0x1d, 1, {0x04}},
		{0x1f, 3, {0x00, 0x00, 0x00}},
		{0x44, 1, {0x60}}
};


void set_display_type(int display_type)
{
	log_message(DEBUG_EPD, "Now > set_display_type");
	switch (display_type)
	{
		case S011_T1_1:
			GATE_LINES = 70;
			SOURCE_LINES = 148;
			REG_SETTINGS = reg_settings_S011_T1_1;
			NUMBER_OF_REGISTER_OVERWRITES = sizeof(reg_settings_S011_T1_1)/sizeof(regSetting_t);
			// strcpy(PATH, "S011_T1.1");
			// read_image_data_from_file = read_image_data_from_file_default;
			log_message(DEBUG_EPD, "display_type: S011_T1.1");

			break;
		case S011_T1_2:
			GATE_LINES = 70;
			SOURCE_LINES = 148;
			REG_SETTINGS = reg_settings_S011_T1_2;
			NUMBER_OF_REGISTER_OVERWRITES = sizeof(reg_settings_S011_T1_2)/sizeof(regSetting_t);
			// strcpy(PATH, "S011_T1.2");
			// read_image_data_from_file = read_image_data_from_file_default;
			log_message(DEBUG_EPD, "display_type: S011_T1.2");

			break;
		case S014_T1_1:
			GATE_LINES = 180;
			SOURCE_LINES = 100;
			REG_SETTINGS = reg_settings_S014_T1_1;
			NUMBER_OF_REGISTER_OVERWRITES = sizeof(reg_settings_S014_T1_1)/sizeof(regSetting_t);
			// strcpy(PATH, "S014_T1.1");
			// read_image_data_from_file = read_image_data_from_file_default;
			log_message(DEBUG_EPD, "display_type: S014_T1.1");

			break;
		case S014_T1_2:
			GATE_LINES = 180;
			SOURCE_LINES = 100;
			REG_SETTINGS = reg_settings_S014_T1_2;
			NUMBER_OF_REGISTER_OVERWRITES = sizeof(reg_settings_S014_T1_2)/sizeof(regSetting_t);
			// strcpy(PATH, "S014_T1.2");
			// read_image_data_from_file = read_image_data_from_file_default;
			log_message(DEBUG_EPD, "display_type: S014_T1.2");

			break;
		case S021_T1_1:
			GATE_LINES = 146;
			SOURCE_LINES = 240;
			REG_SETTINGS = reg_settings_S021_T1_1;
			NUMBER_OF_REGISTER_OVERWRITES = sizeof(reg_settings_S021_T1_1)/sizeof(regSetting_t);
			// strcpy(PATH, "S021_T1.1");
			// read_image_data_from_file = read_image_data_from_file_S021_T1;
			log_message(DEBUG_EPD, "display_type: S021_T1.1");

			break;
		case S021_T3_1: //****change 2024.06.11 */
			GATE_LINES = 146;
			SOURCE_LINES = 240;
			REG_SETTINGS = reg_settings_S021_T1_1;
			NUMBER_OF_REGISTER_OVERWRITES = sizeof(reg_settings_S021_T1_1)/sizeof(regSetting_t);
			// strcpy(PATH, "S021_T1.1");
			// read_image_data_from_file = read_image_data_from_file_S021_T1;
			log_message(DEBUG_EPD, "display_type: S021_T1.1");

			break;//****change 2024.06.11 */
		case S025_T1_1:
			GATE_LINES = 160;
			SOURCE_LINES = 240;
			REG_SETTINGS = reg_settings_S025_T1_1;
			NUMBER_OF_REGISTER_OVERWRITES = sizeof(reg_settings_S025_T1_1)/sizeof(regSetting_t);
			// strcpy(PATH, "S025_T1.1");
			// read_image_data_from_file = read_image_data_from_file_default;
			log_message(DEBUG_EPD, "display_type: S025_T1.1");

            break;
		case S031_T1_1:
			GATE_LINES = 148;
			SOURCE_LINES = 156;
			REG_SETTINGS = reg_settings_S031_T1_1;
			NUMBER_OF_REGISTER_OVERWRITES = sizeof(reg_settings_S031_T1_1)/sizeof(regSetting_t);
			// strcpy(PATH, "S031_T1.1");
			// LINE_SHARING = true;
			// read_image_data_from_file = read_image_data_from_file_S031_T1;
			log_message(DEBUG_EPD, "display_type: S031_T1.1");

			break;
		case D011_T1_1:
			GATE_LINES = 116;
			SOURCE_LINES = 240;
			REG_SETTINGS = reg_settings_D011_T1_1;
			NUMBER_OF_REGISTER_OVERWRITES = sizeof(reg_settings_D011_T1_1)/sizeof(regSetting_t);
			// strcpy(PATH, "D011_T1.1");
			// single_display = 0;
			// read_image_data_from_file = read_image_data_from_file_D011_T1;
			log_message(DEBUG_EPD, "display_type: D011_T1.1");

			break;
		default:
			log_message(DEBUG_ERROR, "display_type unknown");
	}

	PIXEL_COUNT = GATE_LINES * SOURCE_LINES;
	// image_data = (uint8_t *) malloc(PIXEL_COUNT/4);
	// if (image_data == NULL)
		// abort_now("Fatal error in: config_display_type.c -> set_display_type -> malloc for image_data failed.", ABORT_DISP_INFO);
}
  
#endif /* CONFIG_DISPLAY_H */