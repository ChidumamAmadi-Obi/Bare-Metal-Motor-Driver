#include "stm32f4xx.h"

#define CMD_BUFFER_SIZE 100
#define SystemCoreClock 16000000 // 16MHz in datasheet

#define ASCII_BACKSPACE 0x7F // used in puTTy's terminal
#define ASCII_ESC 0x1B
#define ASCII_OPENING_BRACKET 0x5B
#define ASCII_UP 0x41
#define ASCII_DOWN 0x42
#define ASCII_RIGHT 0x43
#define ASCII_LEFT 0x44

#define MIN_DEADBAND_THRESHOLD 1048
#define MAX_DEADBAND_THRESHOLD 3048


