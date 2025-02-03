#include "buzzer.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

// Função para configurar o PWM para um pino com a frequência e o duty cycle desejados
void set_pwm_pin(unsigned int pin, unsigned int freq, unsigned int duty_c) {
    gpio_set_function(pin, GPIO_FUNC_PWM);  // Configura o pino para PWM
    uint slice_num = pwm_gpio_to_slice_num(pin);  // Obtém o número do slice para o pino
    pwm_config config = pwm_get_default_config();  // Obtém a configuração padrão de PWM

    // Calcula o divisor de clock
    float div = (float)clock_get_hz(clk_sys) / (freq * 10000);
    pwm_config_set_clkdiv(&config, div);  // Define o divisor de clock
    pwm_config_set_wrap(&config, 10000);  // Define o wrap (ciclo máximo de 10000)

    pwm_init(slice_num, &config, true);  // Inicializa o PWM no slice
    pwm_set_gpio_level(pin, duty_c);  // Configura o duty cycle para o pino
}

// Função para tocar uma nota com duração variável
void buzzer_beep(unsigned int pin, int note, int duration) {
    uint duty_cycle = (note > 400) ? 5000 : 3000;  // Ajuste do duty cycle baseado na nota
    set_pwm_pin(pin, note, duty_cycle);  // Frequência e volume da nota
    sleep_ms(duration);  // Duração da nota
    set_pwm_pin(pin, pin, 0);  // Desliga o buzzer
    sleep_ms(50);  // Intervalo após o beep
}