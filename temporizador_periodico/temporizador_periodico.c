#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

// definição dos pinos dos leds
#define RED_LED 11
#define YELLOW_LED 12
#define GREEN_LED 13

bool callback_timer(struct repeating_timer *t); //true = ativa e mantem ativo o timer | false = timer para

// criação de uma variavel para controlar o estado do led
volatile int led_on = RED_LED; // inicia no led vermelho conforme enunciado

// função callback da função repetidora
bool callback_timer(struct repeating_timer *t){
    
        printf("passou callback\n");
        //if..else vai ser sempre executado 
        //pois led_on = RED sempre
        if(led_on == RED_LED){
            led_on = YELLOW_LED;
            gpio_put(RED_LED, 0); //desliga led vermelho
            gpio_put(YELLOW_LED, 1); //acende led amarelo
        }else if(led_on == YELLOW_LED){
            led_on = GREEN_LED;
            gpio_put(YELLOW_LED, 0); //desliga led amarelo
            gpio_put(GREEN_LED, 1); //acende led verde
        }else if(led_on == GREEN_LED){
            led_on = RED_LED;
            gpio_put(GREEN_LED, 0); //desliga led verde
            gpio_put(RED_LED, 1); //acende led vermelho
        }
        return true; //mantem o timer ativo
}

void init_leds(){ // função para inicializar os leds para nao encher a main
    gpio_init(RED_LED);
    gpio_set_dir(RED_LED, GPIO_OUT);
    gpio_put(RED_LED, 1); // inicia com o led vermelho aceso
    gpio_init(YELLOW_LED);  
    gpio_set_dir(YELLOW_LED, GPIO_OUT);
    gpio_put(YELLOW_LED, 0);
    gpio_init(GREEN_LED);
    gpio_set_dir(GREEN_LED, GPIO_OUT);
    gpio_put(GREEN_LED, 0);
}

int main()
{
    stdio_init_all(); // inicialização da comunicação serial
    init_leds(); // funcão para inicializar os leds
    struct repeating_timer timer; //config de um timer periodico

    // função repetidora que chama a função callback_timer a cada 3 segundos
    add_repeating_timer_ms(3000, callback_timer, NULL, &timer);
    
    while (true) {
        //atraso para não sobrecarregar o cpu
        sleep_ms(10);
        
    }
    return 0;
}
