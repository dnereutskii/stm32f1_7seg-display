#include "display.h"
#include "display_gpio.h"
#include "display_timer.h"

const uint8_t chars[] = {0xC0,    /*0*/
                    0xF9,   /*1*/
                    0xA4,   /*2*/
                    0xB0,   /*3*/
                    0x99,   /*4*/
                    0x92,   /*5*/
                    0x82,   /*6*/
                    0xF8,   /*7*/
                    0x80,   /*8*/
                    0x90,   /*9*/
                    0x7F,   /*:*/
                    0xFF,   /*None*/
};


volatile digits digs;


void TIM2_IRQHandler()
{   
    static uint8_t dig_order = 0U;
    if(TIM2->SR&TIM_SR_UIF) /*if overflow interrupt flag*/
    {
        TIM2->SR &= ~TIM_SR_UIF;    /*clear interrupt flag*/
        if(dig_order == 0)
        {   
            GPIOA->BSRR = ((0xFF&~digs.digit1)<<16)|\
                            (0xFF&digs.digit1)|\
                            GPIO_BSRR_BR8|\
                            GPIO_BSRR_BS9|\
                            GPIO_BSRR_BS10|\
                            GPIO_BSRR_BS11;
            // GPIOA->ODR = digs.digit1;
            // GPIOA->BSRR = GPIO_BSRR_BR8|GPIO_BSRR_BS9|GPIO_BSRR_BS10|GPIO_BSRR_BS11;
        }
        if(dig_order == 1)
        {
            GPIOA->BSRR = ((0xFF&~digs.digit2)<<16)|\
                            (0xFF&digs.digit2)|\
                            GPIO_BSRR_BS8|\
                            GPIO_BSRR_BR9|\
                            GPIO_BSRR_BS10|\
                            GPIO_BSRR_BS11;
            // GPIOA->ODR = digs.digit2;
            // GPIOA->BSRR = GPIO_BSRR_BS8|GPIO_BSRR_BR9|GPIO_BSRR_BS10|GPIO_BSRR_BS11;
        }
        if(dig_order == 2)
        {
            GPIOA->BSRR = ((0xFF&~digs.digit3)<<16)|\
                            (0xFF&digs.digit3)|\
                            GPIO_BSRR_BS8|\
                            GPIO_BSRR_BS9|\
                            GPIO_BSRR_BR10|\
                            GPIO_BSRR_BS11;
            // GPIOA->ODR = digs.digit3;
            // GPIOA->BSRR = GPIO_BSRR_BS8|GPIO_BSRR_BS9|GPIO_BSRR_BR10|GPIO_BSRR_BS11;
        }
        if(dig_order == 3)
        {
            GPIOA->BSRR = ((0xFF&~digs.digit4)<<16)|\
                            (0xFF&digs.digit4)|\
                            GPIO_BSRR_BS8|\
                            GPIO_BSRR_BS9|\
                            GPIO_BSRR_BS10|\
                            GPIO_BSRR_BR11;
            // GPIOA->ODR = digs.digit4;
            // GPIOA->BSRR = GPIO_BSRR_BS8|GPIO_BSRR_BS9|GPIO_BSRR_BS10|GPIO_BSRR_BR11;
        }
        dig_order++;
        if(dig_order > 3){dig_order = 0;}
    }
}


void display_init()
{
    display_gpio_init();                /*gpio initialization*/
    display_timer_init(3599, 100);      /*5 ms overflow*/
    TIMER_Enable();                     /*timer start*/
    digs.digit1 = digs.digit2 = \
    digs.digit3 = digs.digit4 =0;       /*clear digits*/
}


void display_print_number(uint16_t number)
{
    if(number < 1000) {digs.digit1 = chars[11];} 
    else {digs.digit1 = chars[number/1000];}
    
    if(number < 100){digs.digit2 = chars[11];}
    else {digs.digit2 = chars[(number/100)%10];}

    if(number < 10){digs.digit3 = chars[11];}
    else {digs.digit3 = chars[(number/10)%10];}
    
    digs.digit4 = chars[number%10];
}
