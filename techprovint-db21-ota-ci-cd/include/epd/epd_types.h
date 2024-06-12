#ifndef EPD_TYPES_H
#define EPD_TYPES_H  

typedef struct {
	uint8_t addr;
	int valCount;
	uint8_t val[4];
} regSetting_t;

#endif /* EPD_TYPES_H */