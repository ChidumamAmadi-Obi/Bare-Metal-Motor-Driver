#include "stm32f4xx.h"

#define CMD_BUFFER_SIZE 100
#define SystemCoreClock 16000000 // 16MHz in datasheet
#define BACKSPACE_KEY 0x7F

#define MIN_DEADBAND_THRESHOLD 1048
#define MAX_DEADBAND_THRESHOLD 3048


