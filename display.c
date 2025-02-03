#include "display.h"
#include "pico/stdlib.h"
#include "ws2812.pio.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"


// Variáveis globais de cor
uint8_t selected_r = 50;  
uint8_t selected_g = 50;
uint8_t selected_b = 50;


// Variáveis PIO
PIO pio = pio0;
int sm = 0;
uint offset;


// Função para configurar a cor global
void display_set_color(uint8_t r, uint8_t g, uint8_t b) {
    selected_r = r;
    selected_g = g;
    selected_b = b;
}


// Função para inicializar a PIO para o controle do WS2812
void pio_init_ws2812(int pin) {
    offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, pin, 800000, false);  // WS2812 sem suporte a RGBW (false)
}


// Função para converter RGB para o formato GRB de 32 bits
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}


// Função para enviar dados para os LEDs
static inline void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio, sm, pixel_grb << 8u);  // Envia dados para os LEDs
}


// Função para atualizar os LEDs com base no desenho
void update_led_buffer(double desenho[25]) {
    uint32_t led_buffer[25] = {0};  // Buffer de LEDs


    for (int i = 0; i < 25; i++) {
        if (desenho[i] > 0) {
            // Se o valor for maior que 0, acende o LED com a cor selecionada
            led_buffer[i] = urgb_u32(selected_r, selected_g, selected_b);
        } else {
            // Se for 0, apaga o LED
            led_buffer[i] = 0;
        }
    }


    // Envia os dados para os LEDs
    for (int i = 0; i < 25; i++) {
        put_pixel(led_buffer[i]);
    }
}

void clear_led_buffer() {
    uint32_t led_buffer[25] = {0};  // Buffer de LEDs com todos apagados (valor 0)

    // Envia os dados para os LEDs (todos apagados)
    for (int i = 0; i < 25; i++) {
        put_pixel(led_buffer[i]);  // Envia o valor 0 para apagar o LED
    }
}

// Função para arrumar a matriz (inverter a ordem e ajustar as linhas pares)
void arruma_matriz(double vetor[25], double resultado[25]) {
    // Passo 1: Inverter a ordem do vetor original
    double desenho_ok[25];
    for (int i = 0; i < 25; i++) {
        desenho_ok[i] = vetor[24 - i]; // Inversão dos índices
    }


    // Passo 2: Criar a matriz 5x5 a partir do vetor invertido
    double matriz[5][5];
    int index = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            matriz[i][j] = desenho_ok[index++];
        }
    }


    // Passo 3: Inverter as linhas pares da matriz (0, 2, 4)
    for (int i = 1; i < 4; i += 2) {  // Linhas 0, 2, 4
        for (int j = 0; j < 2; j++) {  // Trocar os elementos simétricos
            double temp = matriz[i][j];
            matriz[i][j] = matriz[i][4 - j];
            matriz[i][4 - j] = temp;
        }
    }


    // Passo 4: Transformar a matriz de volta para o vetor resultado
    index = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            resultado[index++] = matriz[i][j];
        }
    }
}


// Função para desenhar um número
void display_number(uint8_t number) {
    //double desenho[25] = {0};  // Matriz 5x5 para o desenho
    double ordenado[25] = {0};
    // Preenche a matriz de acordo com o número
    switch (number) {
        case 0:            
            double desenho_0[25] = {
                0, 1, 1, 1, 0,
                0, 1, 0, 1, 0,
                0, 1, 0, 1, 0,
                0, 1, 0, 1, 0,
                0, 1, 1, 1, 0
            };
        arruma_matriz(desenho_0, ordenado);
        //update_led_buffer(ordenado);
        break;
        // Adicione casos para outros números de 1 a 9
        case 1:
            double desenho_1[25] = {
                0, 0, 1, 0, 0,
                0, 1, 1, 0, 0,
                0, 0, 1, 0, 0,
                0, 0, 1, 0, 0,
                0, 1, 1, 1, 0
            };
        arruma_matriz(desenho_1, ordenado);
        //update_led_buffer(ordenado);
        break;
        // Adicione casos para outros números de 1 a 9
        case 2:
            double desenho_2[25] = {
                0, 1, 1, 1, 0,
                0, 0, 0, 1, 0,
                0, 1, 1, 1, 0,
                0, 1, 0, 0, 0,
                0, 1, 1, 1, 0
            };
        arruma_matriz(desenho_2, ordenado);
        //update_led_buffer(ordenado);
        break;
        // Adicione casos para outros números de 1 a 9
        case 3:
            double desenho_3[25] = {
                0, 1, 1, 1, 0,
                0, 0, 0, 1, 0,
                0, 1, 1, 1, 0,
                0, 0, 0, 1, 0,
                0, 1, 1, 1, 0
            };
        arruma_matriz(desenho_3, ordenado);
        //update_led_buffer(ordenado);
        break;
        // Adicione casos para outros números de 1 a 9
        case 4:
            double desenho_4[25] = {
                0, 1, 0, 1, 0,
                0, 1, 0, 1, 0,
                0, 1, 1, 1, 0,
                0, 0, 0, 1, 0,
                0, 0, 0, 1, 0
            };
        arruma_matriz(desenho_4, ordenado);
        //update_led_buffer(ordenado);
        break;
        // Adicione casos para outros números de 1 a 9
        case 5:
            double desenho_5[25] = {
                0, 1, 1, 1, 0,
                0, 1, 0, 0, 0,
                0, 1, 1, 1, 0,
                0, 0, 0, 1, 0,
                0, 1, 1, 1, 0
            };
        arruma_matriz(desenho_5, ordenado);
        //update_led_buffer(ordenado);
        break;
        // Adicione casos para outros números de 1 a 9
        case 6:
            double desenho_6[25] = {
                0, 1, 0, 0, 0,
                0, 1, 0, 0, 0,
                0, 1, 1, 1, 0,
                0, 1, 0, 1, 0,
                0, 1, 1, 1, 0
            };
        arruma_matriz(desenho_6, ordenado);
        //update_led_buffer(ordenado);
        break;
        // Adicione casos para outros números de 1 a 9
        case 7:
            double desenho_7[25] = {
                0, 1, 1, 1, 0,
                0, 1, 0, 1, 0,
                0, 0, 0, 1, 0,
                0, 0, 0, 1, 0,
                0, 0, 0, 1, 0
            };
        arruma_matriz(desenho_7, ordenado);
        //update_led_buffer(ordenado);
        break;
        // Adicione casos para outros números de 1 a 9
        case 8:
            double desenho_8[25] = {
                0, 1, 1, 1, 0,
                0, 1, 0, 1, 0,
                0, 1, 1, 1, 0,
                0, 1, 0, 1, 0,
                0, 1, 1, 1, 0
            };
        arruma_matriz(desenho_8, ordenado);
        //update_led_buffer(ordenado);
        break;
                // Adicione casos para outros números de 1 a 9
        case 9:
            double desenho_9[25] = {
                0, 1, 1, 1, 0,
                0, 1, 0, 1, 0,
                0, 1, 1, 1, 0,
                0, 0, 0, 1, 0,
                0, 0, 0, 1, 0
            };
        arruma_matriz(desenho_9, ordenado);
      //  update_led_buffer(ordenado);
        break;

    }


    // Atualiza os LEDs com o desenho
    update_led_buffer(ordenado);
}


// Função para desenhar uma seta para a esquerda
void display_left_arrow() {
    double ordenado[25];
    double desenho[25] = {
        0, 0, 0, 0, 0,
        0, 1, 0, 0, 0,
        1, 1, 0, 0, 0,
        0, 1, 0, 0, 0,
        0, 0, 0, 0, 0
    };


    arruma_matriz(desenho, ordenado);
    update_led_buffer(ordenado);
   
}
// Função para desenhar uma seta para a direita
void display_right_arrow() {
    double ordenado[25];
    double desenho[25] = {
        0, 0, 0, 0, 0,
        0, 0, 0, 1, 0,
        0, 0, 0, 1, 1,
        0, 0, 0, 1, 0,
        0, 0, 0, 0, 0
    };
    arruma_matriz(desenho, ordenado);
    update_led_buffer(ordenado);
}



