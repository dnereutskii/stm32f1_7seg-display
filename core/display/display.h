#ifndef __DISPLAY_H
#define __DISPLAY_H

#include <stm32f1xx.h>

typedef struct {
    uint8_t digit1;
    uint8_t digit2;
    uint8_t digit3;
    uint8_t digit4;
} digits;

void display_init();
void display_print_number(uint16_t number);


#endif /*__DISPLAY_H*/