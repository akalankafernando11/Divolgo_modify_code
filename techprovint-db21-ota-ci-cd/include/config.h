#ifndef CONFIG_H
#define CONFIG_H

#define EEPROM_MAX_CHAR_LENGTH 20
#define EEPROM_SIZE 1000

#define EEPROM_MEM_ADDR_IMG_LAST 0
#define EEPROM_MEM_ADDR_IMG_CURR 50
#define EEPROM_MEM_ADDR_LST_ACTION 100
#define EEPROM_MEM_ADDR_LST_STATUS 120
#define EEPROM_MEM_ROTATION_EN 140
#define EEPROM_MEM_ADDR_LST_EVENT 160
#define EEPROM_MEM_ADDR_LST_ROTATE_EVENT 180
#define EEPROM_MEM_ADDR_LST_ROTATE_EVENT_TMP 200
#define EEPROM_MEM_ROTATION_ON_DATE 220

#define EEPROM_MEM_ADDR_DEVELOPER_MODE 240
#define EEPROM_MEM_ADDR_CONN_TIME 260
#define EEPROM_MEM_ADDR_ROTATION_DURATION 280
#define EEPROM_MEM_ADDR_RUNNUNG_EVENT 300

/*
Power off reeson code
0: Default / No specific status
1: Power off by user butten press
2: Auto power off due to low battery
*/

const uint8_t EEPROM_FAIL_COUNT = 5;

const uint8_t DEFAULT_SLEEP_TIME = 20;
const uint8_t DEFAULT_CYCLE_TIME = 30;
const uint16_t DEFAULT_CONNECTION_TIME = 900;
const uint16_t MAXIMUM_CONNECTION_TIME = 14400;
const uint8_t CONNECTION_SKIP_CYCLES = 0;

#define PCB_VERSION_2_3
// const String FIRMWARE_VERSION = "v0.0.3";

bool DISPLAY_OPTIONS = true;//false;

bool DEBUG_ENABLE = true;  // Enable only for testing and develpoment 

const bool AUTO_IMAGE_CHANGE     = 0;
const bool AUTO_FORMAT           = 0;
const bool DELETE_AFTER_CHANGE   = 1;
const bool SAVE_DEVICE_LOGS      = 1;

const uint8_t IMAGE_ORIENTATION = 0;
//wifi
const int WIFI_RETRY_COUNT = 10;
const int CONNECTION_FAIL_RETRIES = 3;

const bool AUTO_ROTATION_STOP = true;

const int VBATT_MIN = 3600;                 // Voltage in miliVolts
const int VBATT_CUTOFF = 3500;
const uint8_t MIN_START_BATT_CAPACITY = 40;
// const int VBATT_LOW = 3550;

const uint16_t MAX_FILE_SIZE = 30000; // File size => Bytes or Number of characters
const uint16_t MAX_PAYLOAD_SIZE = 25000; // Maximum size that can download to the device
const uint16_t IMAGE_FILE_SIZE = 8786;

const uint16_t CLIENT_CONNECTION_TIMEOUT = 30000; // Time in ms


const char* WIFI_SSID      = "DIVOLGO";
// const char* WIFI_SSID      = "DIVOLGO_D";
const char* WIFI_PASSWORD  = "password1";
// const char* WIFI_PASSWORD  = "asdf1234";

// file paths
const char *IMG_PATH = "/i";
const char *DEF_IMG_PATH = "/j";
const char *LOG_PATH = "/log";
const char *BATT_LOG = "/log_batt";

// actions
const String ACT_NONE = "none";
const String ACT_CHANGE = "change";
const String ACT_DOWNLOAD = "download";
const String ACT_FORMAT = "format";
const String ACT_DELETE = "delete";
const String ACT_ROTATE = "rotate";
const String ACT_WHITE = "white";
const String ACT_BLACK = "black";
const String ACT_FILE_COUNT = "file-count";
const String ACT_SUCESS = "success";

// 1= Production, 2=DEV or localhost
#define WEBHOST 2 // Running with webhost
#define HTTPS 1
#define HTTP 0

#if WEBHOST == 1
const char *HOST = "divolgo-controlpanel-psuhbzzwea-uc.a.run.app/api/ack";  // Production
const int protocol = HTTPS;

#elif WEBHOST == 2
const char *HOST = "cpanel-stg.divolgodigital.com/api/ack";                 // Staging
const int protocol = HTTPS;


#elif WEBHOST == 3
const char *HOST = "cpanel-qa.divolgodigital.com/api/ack";                  // QA
const int protocol = HTTPS;

#elif WEBHOST == 4
const char *HOST = "control-panel-simulator.herokuapp.com/api/ack";         // Simulator
const int protocol = HTTPS;

#else
const char *HOST = "192.168.8.109";
const int PORT = 8080;
const String CLIENT_ADDR = "http://" + (String)HOST + ":" + (String)PORT;
#endif

#endif /* CONFIG_H */
