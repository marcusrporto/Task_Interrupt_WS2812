#ifndef DISPLAY_H
#define DISPLAY_H


#include "pico/stdlib.h"


// Definição de cores padrão
extern uint8_t selected_r;
extern uint8_t selected_g;
extern uint8_t selected_b;

void pio_init_ws2812(int pin);

// Função para configurar a cor global
void display_set_color(uint8_t r, uint8_t g, uint8_t b);


// Função para desenhar números de 0 a 9
void display_number(uint8_t number);


// Funções para desenhar setas
void display_left_arrow();
void display_right_arrow();
void clear_led_buffer(void);

#endif
