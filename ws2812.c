/*
 * Por: Marcus Porto
 *
 * Este código é uma adaptação do código original do SDK Pico
 * para a utilização da matriz de LEDs WS2812 do BitDogLab.
 *
 * Ele trata os Botões A e B da placa como interrupções
 * E faz Deboucing nesses para atenuar o efeito Boucing de botões
 * Também utiliza-se de uma Função de Callback do Temporizador
 * usado para blinkar o LED RGB da placa no Pino 13 - RED
 * Original em:
 * https://github.com/raspberrypi/pico-examples/tree/master/pio/ws2812
*/

#include <stdio.h>
#include "pico/stdlib.h"
#include "./display.h"
#include "hardware/timer.h" // Inclui a biblioteca para gerenciamento de temporizadores de hardware.
#include "pico/bootrom.h"
#include "buzzer.h"

#define WS2812_PIN 7

// Configurações dos pinos GPIO
const uint led_pin_red = 13;       //Red=13, Blue=12, Green=11
const uint button_pin_a = 5;       // Botão A = 5, Botão B = 6 , BotãoJoy = 22
const uint button_pin_b = 6;
const uint button_pin_c = 22;
const uint buzzer_pin = 21;

// Variável volátil para armazenar o número (de 0 a 9)
static volatile int numero = 0;          // Esta variável será alterada nas interrupções
static volatile uint32_t last_time = 0;  // Armazena o tempo do último evento (em microssegundos)
volatile bool beep_needed = false;       // Flag para indicar que o beep deve ser feito
volatile int beep_note = 0;              // Armazenar a nota do beep

// Variável Controle do LED
bool led_on = false;

// Prototipo da função de interrupção
static void gpio_irq_handler(uint gpio, uint32_t events);

// Função de callback que será chamada repetidamente pelo temporizador
// O tipo bool indica que a função deve retornar verdadeiro ou falso para continuar ou parar o temporizador.
bool repeating_timer_callback(struct repeating_timer *t) {
    // Imprime uma mensagem na saída serial indicando que 1 segundo se passou.
    //Liga ou desliga o led.
    led_on = !led_on;
    gpio_put(led_pin_red,led_on);
    // Retorna true para manter o temporizador repetindo. Se retornar false, o temporizador para.
    return true;
}

int main() {
    //                      Inicializações
    stdio_init_all();
    pio_init_ws2812(WS2812_PIN);             // Inicializa a PIO para controlar os LEDs WS2812
    gpio_init(led_pin_red);                  // Inicializa o pino do LED RGB RED
    gpio_set_dir(led_pin_red, GPIO_OUT);     // Configura o pino como saída
    
    // Inicializa o botão A (incremento)
    gpio_init(button_pin_a);                  // Inicializa o botão a 
    gpio_set_dir(button_pin_a, GPIO_IN);      // Configura o pino como entrada 
    gpio_pull_up(button_pin_a);               // Habilita o pull-up interno
    
    // Inicializa o botão B (decremento)
    gpio_init(button_pin_b);                  // Inicializa o botão a 
    gpio_set_dir(button_pin_b, GPIO_IN);      // Configura o pino como entrada 
    gpio_pull_up(button_pin_b);               // Habilita o pull-up interno

     // Inicializa o botão Joy (decremento)
    gpio_init(button_pin_c);                  // Inicializa o botão a 
    gpio_set_dir(button_pin_c, GPIO_IN);      // Configura o pino como entrada 
    gpio_pull_up(button_pin_c);               // Habilita o pull-up interno

    
    gpio_init(buzzer_pin);
    gpio_set_dir(buzzer_pin, GPIO_OUT);


    // Declaração de uma estrutura de temporizador de repetição.
    // Esta estrutura armazenará informações sobre o temporizador configurado.
    struct repeating_timer timer;

    // Configura o temporizador para chamar a função de callback a cada 1 segundo.
    add_repeating_timer_ms(200, repeating_timer_callback, NULL, &timer);
    
    // Configuração da interrupção com callback para incremento
    gpio_set_irq_enabled_with_callback(button_pin_a, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    // Configuração da interrupção com callback para decremento
    gpio_set_irq_enabled_with_callback(button_pin_b, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    // Configuração da interrupção com callback para decremento
    gpio_set_irq_enabled_with_callback(button_pin_c, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    // Seta cores Personalizadas no display
    display_set_color(20, 20, 20);
     // Exibe o número inicial no display
    display_number(numero);

    // Loop principal
    while (1) {
        // Verifica se o beep é necessário
        if (beep_needed) {
            buzzer_beep(buzzer_pin, beep_note, 500);  // Executa o beep
            beep_needed = false;  // Reseta a flag após o beep
        }
    }
    return 0;
}

// Função de interrupção para os botões A e B com debouncing
void gpio_irq_handler(uint gpio, uint32_t events) {
    // Obtém o tempo atual em microssegundos
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    
    // Verifica se passou tempo suficiente desde o último evento (debouncing de 200ms)
    if (current_time - last_time > 300000) { // 300ms
        last_time = current_time;  // Atualiza o tempo do último evento
        
        // Verifica qual botão foi pressionado e realiza a ação apropriada
        if (gpio == button_pin_a) {
            // Incrementa o número, garantindo que fique entre 0 e 9
            if (numero < 9) {
                numero++;
            } else {
                beep_needed = true;  // Ativa a flag para indicar que o beep deve ser tocado
                beep_note = A;       // Define a nota
            }
        } else if (gpio == button_pin_b) {
            // Decrementa o número, garantindo que não fique abaixo de 0
            if (numero > 0) {
                numero--;
            } else {
                beep_needed = true;  // Ativa a flag para indicar que o beep deve ser tocado
                beep_note = A;       // Define a nota
            }
        } else if (gpio == button_pin_c) {
         printf("Interrupção ocorreu no pino %d, no evento %d\n", gpio, events);
         printf("HABILITANDO O MODO GRAVAÇÃO");
         clear_led_buffer(); // Limpa Matriz LEDS
	     reset_usb_boot(0,0); //habilita o modo de gravação do microcontrolador
        }   
    display_number(numero);
    // Imprime na porta serial para debbug
    printf("Numero Atual: %d\n", numero);
    }
}