#ifndef __GRAY_DETECTION_H
#define __GRAY_DETECTION_H



typedef struct
{
	uint8_t bit1	:1;
	uint8_t bit2	:1;
	uint8_t bit3	:1;
	uint8_t bit4	:1;
	uint8_t bit5	:1;
	uint8_t bit6	:1;
	uint8_t bit7	:1;
	uint8_t bit8	:1;
	uint8_t bit9	:1;
	uint8_t bit10	:1;
	uint8_t bit11	:1;
	uint8_t bit12	:1;
	uint8_t bit13	:1;
	uint8_t bit14	:1;
	uint8_t bit15	:1;
	uint8_t bit16	:1;
}gray_flags;


typedef union
{
	gray_flags gray;
	uint16_t state;
}_gray_state; 


void gpio_input_init(void);
void gpio_input_check_channel_12(void);


extern float gray_status[2],gray_status_backup[2][20];
extern uint32_t gray_status_worse;
extern _gray_state gray_state; 

#endif
