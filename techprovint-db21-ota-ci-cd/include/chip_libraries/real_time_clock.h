#ifndef REAL_TIME_CLOCK_H
#define REAL_TIME_CLOCK_H

#define PCF85063_I2C_ADDRESS 0x51
#define PCF85063_OFFSET 0x02
#define REG_CTRL1         0x00
#define REG_CTRL1_STOP    0x20
#define REG_CTRL2         0x01
#define REG_OFFSET        0x02
#define REG_SEC           0x04
#define REG_SEC_OS        0x80
#define REG_MIN           0x05
#define REG_HOUR          0x06
#define REG_DAY_MONTH     0x07
#define REG_DAY_WEEK      0x08
#define REG_MON           0x09
#define REG_YEAR          0x0A
#define REG_TIMER_VAL	  0x10
#define REG_TIMER_MODE	  0x11
#define CAP_SEL_7PF       0
#define CAP_SEL_12_5PF    1

uint8_t rtc_second;
uint8_t rtc_minute;
uint8_t rtc_hour;
uint8_t rtc_day_of_week; // Sunday is day 1
uint8_t rtc_day_of_month;
uint8_t rtc_month;
uint16_t rtc_year;
uint16_t day_of_year;

//Convert binary coded decimal to normal decimal numbers
uint8_t bcdToDec(uint8_t val)
{
  	return ( (val/16*10) + (val%16) );
}
uint8_t decToBcd(uint8_t val)
{
  	return ( (val/10*16) + (val%10) );
}

uint8_t PCF85063_cap_sel(uint8_t value)
{
	uint8_t control_1 = read_i2c(PCF85063_I2C_ADDRESS, REG_CTRL1);
	control_1 = (control_1 & 0xFE) | (0x01 & value);
	write_i2c(PCF85063_I2C_ADDRESS, REG_CTRL1, control_1);

  	return read_i2c(PCF85063_I2C_ADDRESS, REG_CTRL1) & 0x01;
}

/*Function: The clock timing will start */
void PCF85063_start_clock(void)        // set the ClockHalt bit low to start the rtc
{
  	read_i2c(PCF85063_I2C_ADDRESS, REG_CTRL1);
  	read_i2c(PCF85063_I2C_ADDRESS, REG_CTRL1);
}

void rtc_stop_clock(void)         // set the ClockHalt bit high to stop the rtc 
{
  	read_i2c(PCF85063_I2C_ADDRESS, REG_CTRL1);
  	read_i2c(PCF85063_I2C_ADDRESS, REG_CTRL1);
}

time_t rtc_get_time()
{
  // Reset the register pointer
	Wire.beginTransmission(PCF85063_I2C_ADDRESS);
	Wire.write((uint8_t)0x04);
	Wire.endTransmission();
	Wire.requestFrom(PCF85063_I2C_ADDRESS, 7);
	// A few of these need masks because certain bits are control bits
	rtc_second     	= bcdToDec(Wire.read() & 0x7f);
	rtc_minute     	= bcdToDec(Wire.read());
	rtc_hour     		= bcdToDec(Wire.read() & 0x3f);// Need to change this if 12 hour am/pm
	rtc_day_of_month 	= bcdToDec(Wire.read());
	rtc_day_of_week  	= bcdToDec(Wire.read());
	rtc_month      	= bcdToDec(Wire.read());
	rtc_year     		= bcdToDec(Wire.read());

	setTime(rtc_hour, rtc_minute, rtc_second, rtc_day_of_month, rtc_month, rtc_year);
	return now();
}

void rtc_set_time(time_t time_stamp)
{
	rtc_stop_clock();

	write_i2c(PCF85063_I2C_ADDRESS, REG_SEC, decToBcd(second(time_stamp)));// 0 to bit 7 starts the clock, bit 8 is OS reg
	write_i2c(PCF85063_I2C_ADDRESS, REG_MIN, decToBcd(minute(time_stamp)));
	write_i2c(PCF85063_I2C_ADDRESS, REG_HOUR, decToBcd(hour(time_stamp)));  // If you want 12 hour am/pm you need to set bit 6
	write_i2c(PCF85063_I2C_ADDRESS, REG_DAY_MONTH, decToBcd(day(time_stamp)));
	write_i2c(PCF85063_I2C_ADDRESS, REG_DAY_WEEK, decToBcd(weekday(time_stamp)));  // Sunday is day 1
	write_i2c(PCF85063_I2C_ADDRESS, REG_MON, decToBcd(month(time_stamp)));
	write_i2c(PCF85063_I2C_ADDRESS, REG_YEAR, decToBcd(year(time_stamp)-2000));

	PCF85063_start_clock();
}

void PCF85063_set_calibration(int mode, float Fmeas)
{
	float offset = 0;
	float Tmeas = 1.0 / Fmeas;
	float Dmeas = 1.0 / 32768 - Tmeas;
	float Eppm = 1000000.0 * Dmeas / Tmeas;
	if (mode == 0)
		offset = Eppm / 4.34;
	else if (mode == 1)
		offset = Eppm / 4.069;

	uint8_t data = ((mode << 7) & (0x80)) | ((int)(offset + 0.5) & 0x7f);
	write_i2c(PCF85063_I2C_ADDRESS, PCF85063_OFFSET, data);
}

uint8_t PCF85063_calibrate_by_seconds(int mode, float offset_sec)
{
	float Fmeas = 32768.0 + offset_sec * 32768.0;
	PCF85063_set_calibration(mode, Fmeas);
	return read_i2c(PCF85063_I2C_ADDRESS, PCF85063_OFFSET);
}

void rtc_begin() 
{
  	PCF85063_cap_sel(1);           // 0 - 7 pF, 1 - 12.5 pF
  	PCF85063_start_clock();
  	PCF85063_set_calibration(1, 32767.2);  // Setting offset by clock frequency
  	uint8_t ret = PCF85063_calibrate_by_seconds(0, -0.000041);
}

void PCF85063_reset()
{
  	write_i2c(PCF85063_I2C_ADDRESS, REG_CTRL1, 0x10);
}

String rtc_date()
{
	rtc_get_time();
	String rtc_date ((char *)0);
	rtc_date += String(rtc_year+2000);
	rtc_date += "-";
	rtc_date += String(rtc_month);
	rtc_date += "-";
	rtc_date += String(rtc_day_of_month);

	return rtc_date;
}

uint16_t rtc_day_of_year()
{
	rtc_get_time();
	uint16_t tmp = (rtc_month*31) + rtc_day_of_month;
	return tmp;
}

uint8_t rtc_get_day_of_week()
{
	rtc_get_time();
	return rtc_day_of_week;
}

void rtc_print_time()
{
  rtc_get_time();
  Serial.print(rtc_hour, DEC);
  Serial.print(":");
  Serial.print(rtc_minute, DEC);
  Serial.print(":");
  Serial.print(rtc_second, DEC);
  Serial.print("  ");
  Serial.print(rtc_month, DEC);
  Serial.print("/");
  Serial.print(rtc_day_of_month, DEC);
  Serial.print("/");
  Serial.print(rtc_year+2000, DEC);
  Serial.print(" ");
  Serial.print(rtc_day_of_month);
  Serial.print(" Week day:");
  Serial.print(rtc_day_of_week);

  Serial.println(" ");
}

void rtc_set_timer(time_t time_seconds)
{
	time_t time_rm = time_seconds % 30;
	time_seconds = time_seconds - time_rm;

	log_message(DEBUG_MSG, "Set connection timer to : " + String(time_seconds));
	
	write_i2c(PCF85063_I2C_ADDRESS, REG_CTRL2 , 0b00001000);

	time_seconds = (time_seconds <= 30) ? 30 : time_seconds;
	
	if (time_seconds < 255) {
		time_seconds = time_seconds - 25;
		write_i2c(PCF85063_I2C_ADDRESS, REG_TIMER_MODE, 0b00010110);
		write_i2c(PCF85063_I2C_ADDRESS, REG_TIMER_VAL, time_seconds);
	} else {
		time_t time_minutes = time_seconds / 60;
		write_i2c(PCF85063_I2C_ADDRESS, REG_TIMER_MODE, 0b00011110);
		write_i2c(PCF85063_I2C_ADDRESS, REG_TIMER_VAL, time_minutes);
	}
}

bool rtc_read_timer_flag()
{
	uint8_t read_val = read_i2c(PCF85063_I2C_ADDRESS, REG_CTRL2);
	return bitRead(read_val, 3);
}

void rtc_clear_timer_flag()
{
	write_i2c(PCF85063_I2C_ADDRESS, REG_CTRL2 , 0b00000000);
}

void rtc_set_timer_flag()
{
	write_i2c(PCF85063_I2C_ADDRESS, REG_CTRL2 , 0b00001000);
}

#endif /* REAL_TIME_CLOCK_H */
