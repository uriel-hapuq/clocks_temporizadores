#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

//definição dos pinos necessarios
#define RED_LED 11
#define BLUE_LED 12
#define GREEN_LED 13
#define BUTTON_A 5

//declaração de variaveis volateis globais para uso nas funções
volatile int sequence = 0;
volatile bool is_active = false;

//prototipos 
void gpio_irq_handler(uint gpio, uint32_t events);
int64_t callback(alarm_id_t id, void *user_data);

//função que inicializa os pinos dos leds e botoes
void init_gpio() {
    gpio_init(RED_LED);
    gpio_set_dir(RED_LED, GPIO_OUT);
    gpio_put(RED_LED, 0);
    
    gpio_init(BLUE_LED);
    gpio_set_dir(BLUE_LED, GPIO_OUT);
    gpio_put(BLUE_LED, 0);
    
    gpio_init(GREEN_LED);
    gpio_set_dir(GREEN_LED, GPIO_OUT);
    gpio_put(GREEN_LED, 0);

    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);
}

int main() {
    //inicialização da porta serial
    stdio_init_all();
    //inicialização dos pinos
    init_gpio();
    // cnfiguração da interrupção do botão
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    while(1) {
        sleep_ms(10);
    }   
    return 0;
}
//função callback do alarm
int64_t callback(alarm_id_t id, void *user_data) {
    
    if(sequence == 0) {
        gpio_put(RED_LED,0);
        printf("%d\n", sequence);
        sequence++;
        add_alarm_in_ms(3000, callback, NULL, false);
    }
    else if(sequence == 1) {
        gpio_put(BLUE_LED,0);
        printf("%d\n", sequence);
        sequence++;
        add_alarm_in_ms(3000, callback, NULL, false); //ultima chamada 
    }
    else if(sequence == 2) {
        gpio_put(GREEN_LED,0);
        printf("%d\n", sequence);
        sequence = 0;
        is_active = false; // sequencia finaliza 
    }
    
    return 0;
}

//função handler do evento do botao
void gpio_irq_handler(uint gpio, uint32_t events) {
    // variaveis necessarias para controlar o debouce
    static uint32_t last_time = 0; 
    uint32_t now = time_us_32();
    
    if((now - last_time) > 250000) { // debounce basico de 250ms
        last_time = now;
        
        if(gpio == BUTTON_A) {
            if(!is_active){ // possibilita que seja ativado novamente assim que o evento do alarme acabar (quando a ultima led apagar)
                is_active = true;
                sequence = 0; // variavel que controla a sequencia começa em 0
                //liga todos os leds
                gpio_put(RED_LED, 1);
                gpio_put(BLUE_LED, 1);
                gpio_put(GREEN_LED, 1);
                //chamada do alarme
                add_alarm_in_ms(3000, callback, NULL, false);   
            }
            
        }
    }
}