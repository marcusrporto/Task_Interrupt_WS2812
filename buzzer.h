#ifndef BUZZER_H
#define BUZZER_H

#include "pico/stdlib.h"

// Definindo as frequências das notas (em Hz)
#define C         261
#define D         294
#define E         329
#define F         349
#define G         391
#define G_SHARP   415
#define A         440
#define A_SHARP   455
#define B         466
#define C_HIGH    523
#define C_SHARP_HIGH 554
#define D_HIGH    587
#define D_SHARP   622
#define E_HIGH    659
#define F_HIGH    698
#define F_SHARP_HIGH 740
#define G_HIGH    784
#define G_SHARP_HIGH 830
#define A_HIGH    880

// Função para configurar o PWM para um pino com a frequência e o duty cycle desejados
void set_pwm_pin(unsigned int pin, unsigned int freq, unsigned int duty_c);

// Função para tocar uma nota com duração variável
void buzzer_beep(unsigned int pin, int note, int duration);

#endif // BUZZER_H
