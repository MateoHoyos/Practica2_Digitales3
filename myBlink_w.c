/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

// libreias necesarias
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "pico/time.h"
#include "hardware/timer.h"

// Definimos los pines para los LEDs
#define LED_1 10 
#define LED_2 11
#define LED_3 12

// Definimos los pines para los botones
#define BUTTON_1 16 
#define BUTTON_2 17
#define BUTTON_3 18
#define BUTTON_4 19

// Definimos los pines para los segmentos de los displays de 7-segmentos
#define SEG_A 0
#define SEG_B 1
#define SEG_C 2
#define SEG_D 3
#define SEG_E 4
#define SEG_F 5
#define SEG_G 6
#define SEG_DP 7

// Definimos los pines para los displays (para la multiplexacion)
#define D_1 13
#define D_2 14
#define D_3 15

// Secuencia de estados de los LEDs
#define SEQUENCE_LENGTH 4 // Longitud de la secuencia
bool led_states[SEQUENCE_LENGTH][3] = {
    {true, true, true},   // "111"
    {false, true, true},  // "011"
    {false, false, true}, // "001"
    {false, false, false} // "000"
};

// variables goblales para configurar los tiempos de conteo de los timers
int sequence_index = 5; // indice de la secuencia inicial (arranca en un numero mayor que 4 para garantizar que que la secuencia se ralice correctamente)
int contador = 0; // contador de segundos
int contador_ms = 0; // contador en milisegundos
int time_pessed = 0; // para medir el tiempo en que se demora el jugador en pesionar el boton

// Declaration of funtions used in the program
bool to_do_sequence(); // Function to perform the initial sequence
uint8_t random_number(uint8_t rangoMin, uint8_t rangoMax); // Function that returns a random number in the set rangoMin and rangoMax
void turn_LED(uint8_t pindLed); // function that turns a spesific LED on or off
void timer_callback_1(struct repeating_timer *t); // function to count seconds
void timer_callback_2(struct repeating_timer *t); // function to count seconds
void timer_callback_3(struct repeating_timer *t); // function to count miliseconds
void sieteSegmentos(const uint8_t numero); // function that show a segmentation
void descompose_number(int numero, u_int8_t *centenas, uint8_t *decenas, u_int8_t *unidades); // functio to descompose a number in units, decimas and centecimas

int main() {
	stdio_init_all();

    // configuramos los pines de los segmentos
    gpio_init(SEG_A); 
    gpio_init(SEG_B); 
    gpio_init(SEG_C); 
    gpio_init(SEG_D); 
    gpio_init(SEG_E); 
    gpio_init(SEG_F); 
    gpio_init(SEG_G); 
    gpio_init(SEG_DP); 
    gpio_set_dir(SEG_A, GPIO_OUT); 
    gpio_set_dir(SEG_B, GPIO_OUT); 
    gpio_set_dir(SEG_C, GPIO_OUT); 
    gpio_set_dir(SEG_D, GPIO_OUT); 
    gpio_set_dir(SEG_E, GPIO_OUT); 
    gpio_set_dir(SEG_F, GPIO_OUT); 
    gpio_set_dir(SEG_G, GPIO_OUT); 
    gpio_set_dir(SEG_DP, GPIO_OUT); 

    // configuramos los pines de los displays
    gpio_init(D_1);
    gpio_init(D_2);
    gpio_init(D_3);
    gpio_set_dir(D_1, GPIO_OUT);
    gpio_set_dir(D_2, GPIO_OUT);
    gpio_set_dir(D_3, GPIO_OUT);

    // configuramos los pines para los LEDs
    gpio_init(LED_1);
    gpio_init(LED_2);
    gpio_init(LED_3);
    gpio_set_dir(LED_1, GPIO_OUT);
    gpio_set_dir(LED_2, GPIO_OUT);
    gpio_set_dir(LED_3, GPIO_OUT);
    
    // configuramos los pines para los botones
    gpio_init(BUTTON_1);
    gpio_init(BUTTON_2);
    gpio_init(BUTTON_3);
    gpio_set_dir(BUTTON_1, GPIO_IN);
    gpio_set_dir(BUTTON_2, GPIO_IN);
    gpio_set_dir(BUTTON_3, GPIO_IN);
    gpio_pull_down(BUTTON_1);
    gpio_pull_down(BUTTON_2);
    gpio_pull_down(BUTTON_3); 
    
    // variables para las unidades, decimas, centecimas y milecimas
    uint8_t u = 0;
    uint8_t d = 0;
    uint8_t c = 0;
    uint8_t m = 0;

    // variables para almacenar los numeros aleatorios de tiempo y led 
    uint8_t randomTime = 0;
    uint8_t randomLed = 0;

    // timers para controlar los tiempos de funcionamiento del programa
    struct repeating_timer timer_1, timer_2, timer_3;
    add_repeating_timer_ms(1000, timer_callback_1, NULL, &timer_1);
    add_repeating_timer_ms(1000, timer_callback_2, NULL, &timer_2);
    add_repeating_timer_ms(10, timer_callback_3, NULL, &timer_3);

    // flags para conrolar las etapas de flujo del progrma
    bool flag_1 = false; // hacer secuencia inicial
    bool flag_2 = false; // parar secuencia inicial, generar numero aleatorio y esperar ese tiempo
    bool flag_3 = false; // encender un led aleatoriamente y empesar a contar el tiempo depues de encender el led
    bool flag_4 = false; // mostar el tiempo hasta que se precionó el boton correcto.

    // bucle prinsipal
    while (1){

        // hacer secuencia inicial
        if (gpio_get(BUTTON_1) || flag_1) {
            flag_1 = true;
            flag_2 = to_do_sequence();
        }

        // parar secuencia inicial, generar numero aleatorio y esparar ese tiempo
        if (flag_2){
            flag_1 = false;
            sequence_index = 5;
            // Generamos un tiempo y pin para el led aleatoriamente
            randomTime = random_number(0, 10);
            randomLed = random_number(10, 12);

            // Para esperar un tiempo generado aleatoriamente [Segundos]
            if (contador >= randomTime){
                turn_LED(randomLed);
                contador_ms = 0;
                flag_2 = false;
                flag_3 = true;
            }
        }

        //encender un led aleatoriamente y empesar a contar el tiempo depues de encender el led
        if(flag_3){
            descompose_number(contador_ms, &c, &d, &u);

            sieteSegmentos(c);
            gpio_put(SEG_DP, 1);
            gpio_put(D_1, 1);
            sleep_ms(5);
            gpio_put(D_1, 0);
            gpio_put(SEG_DP, 0);

            sieteSegmentos(d);
            gpio_put(D_2, 1);
            sleep_ms(5);
            gpio_put(D_2, 0);

            sieteSegmentos(u);
            gpio_put(D_3, 1);
            sleep_ms(5);
            gpio_put(D_3, 0);

            // Si presionamos el boton asociado al led 1
            if(gpio_get(BUTTON_2) && gpio_get(LED_3)){
                flag_3 = false;
                flag_4 = true;
                time_pessed = contador_ms;
                sleep_us(5);
            }

            // si presionamos el boton asociado al led 2
            if(gpio_get(BUTTON_3) && gpio_get(LED_2)){
                flag_3 = false;
                flag_4 = true;
                time_pessed = contador_ms;
                sleep_us(5);
            }

            // si presionamos el boton asociado al led 3
            if(gpio_get(BUTTON_3) && gpio_get(LED_1)){
                flag_3 = false;
                flag_4 = true;
                time_pessed = contador_ms;
                sleep_us(5);
            }
            /*
            // si no se presiona el boton que es sumamos 1 segundo
            if ((gpio_get(BUTTON_2) && !gpio_get(LED_3)) || (gpio_get(BUTTON_3) && !gpio_get(LED_2)) || (gpio_get(BUTTON_3) && !gpio_get(LED_1))){
                sleep_ms(10);
                contadorus += 100; 
            }
            */
        }
        // mostar el tiempo hasta que se precionó el boton correcto.
        if(flag_4){
            descompose_number(time_pessed, &c, &d, &u);
            sieteSegmentos(c);
            gpio_put(SEG_DP, 1);
            gpio_put(D_1, 1);
            sleep_ms(5);
            gpio_put(D_1, 0);
            gpio_put(SEG_DP, 0);

            sieteSegmentos(d);
            gpio_put(D_2, 1);
            sleep_ms(5);
            gpio_put(D_2, 0);

            sieteSegmentos(u);
            gpio_put(D_3, 1);
            sleep_ms(5);
            gpio_put(D_3, 0);

            gpio_put(randomLed, 0);

            if(gpio_get(BUTTON_1)){
                flag_4 = false;
                flag_1 = true;
            }
        }
    }
    
    return 0;   
}

void timer_callback_1(struct repeating_timer *t){
    // Incrementar el índice de la secuencia
    sequence_index++;
    // Reiniciar el índice si llegamos al final de la secuencia
}

void timer_callback_2(struct repeating_timer *t){
    contador++;
    if (contador >= 999){
        contador = 0;
    }
}

void timer_callback_3(struct repeating_timer *t){
    contador_ms++;
    if (contador_ms >= 1000){
        contador_ms = 999;
    }
}

bool to_do_sequence(){
    // Establecer los estados de los LEDs de acuerdo con la secuencia actual
    if (sequence_index >= SEQUENCE_LENGTH){
        sequence_index = 0;
    }
    gpio_put(LED_1, led_states[sequence_index][0]);
    gpio_put(LED_2, led_states[sequence_index][1]);
    gpio_put(LED_3, led_states[sequence_index][2]);

    if(sequence_index >= 3){
        contador = 0;
        return true;
    }
    else{
        return false;
    }
}

uint8_t random_number(uint8_t rangoMin, uint8_t rangoMax){
    // Semilla para la función rand() basada en el tiempo actual
    srand(time(NULL));

    // Calcular el rango de valores posibles
    uint8_t rango = rangoMax - rangoMin + 1;

    // Generar el número aleatorio y ajustarlo al rango deseado
    uint8_t numero_aleatorio = (rand() % rango) + rangoMin;

    return numero_aleatorio;
}

void turn_LED(uint8_t pindLed){
    switch (pindLed)
            {
            case 10:
                gpio_put(LED_1, 1);
                gpio_put(LED_2, 0);
                gpio_put(LED_3, 0);
                break;
            case 11:
                gpio_put(LED_1, 0);
                gpio_put(LED_2, 1);
                gpio_put(LED_3, 0);
                break;
            case 12:
                gpio_put(LED_1, 0);
                gpio_put(LED_2, 0);
                gpio_put(LED_3, 1);
                break;
            default:
                gpio_put(LED_1, 1);
                gpio_put(LED_2, 1);
                gpio_put(LED_3, 1);
                break;
            }
}

void descompose_number(int numero, u_int8_t *centenas, u_int8_t *decenas, u_int8_t *unidades) {
    // Asegurarse de que el número sea de tres dígitos
    if (numero >= 0 && numero <= 999) {
        *centenas = numero / 100;
        *decenas = (numero / 10) % 10;
        *unidades = numero % 10;
    } else {
        *centenas = 0;
        *decenas = 0;
        *unidades = 0;
    }
}

void sieteSegmentos(const uint8_t numero){
    switch (numero) {
        case 0:
            gpio_put(SEG_A, 1);
            gpio_put(SEG_B, 1);
            gpio_put(SEG_C, 1);
            gpio_put(SEG_D, 1);
            gpio_put(SEG_E, 1);
            gpio_put(SEG_F, 1);
            gpio_put(SEG_G, 0);
            gpio_put(SEG_DP, 0);
            break;
        case 1:
            gpio_put(SEG_A, 0);
            gpio_put(SEG_B, 1);
            gpio_put(SEG_C, 1);
            gpio_put(SEG_D, 0);
            gpio_put(SEG_E, 0);
            gpio_put(SEG_F, 0);
            gpio_put(SEG_G, 0);
            gpio_put(SEG_DP, 0);
            break;
        case 2:
            gpio_put(SEG_A, 1);
            gpio_put(SEG_B, 1);
            gpio_put(SEG_C, 0);
            gpio_put(SEG_D, 1);
            gpio_put(SEG_E, 1);
            gpio_put(SEG_F, 0);
            gpio_put(SEG_G, 1);
            gpio_put(SEG_DP, 0);
            break;
        case 3:
            gpio_put(SEG_A, 1);
            gpio_put(SEG_B, 1);
            gpio_put(SEG_C, 1);
            gpio_put(SEG_D, 1);
            gpio_put(SEG_E, 0);
            gpio_put(SEG_F, 0);
            gpio_put(SEG_G, 1);
            gpio_put(SEG_DP, 0);
            break;
        case 4:
            gpio_put(SEG_A, 0);
            gpio_put(SEG_B, 1);
            gpio_put(SEG_C, 1);
            gpio_put(SEG_D, 0);
            gpio_put(SEG_E, 0);
            gpio_put(SEG_F, 1);
            gpio_put(SEG_G, 1);
            gpio_put(SEG_DP, 0);
            break;
        case 5:
            gpio_put(SEG_A, 1);
            gpio_put(SEG_B, 0);
            gpio_put(SEG_C, 1);
            gpio_put(SEG_D, 1);
            gpio_put(SEG_E, 0);
            gpio_put(SEG_F, 1);
            gpio_put(SEG_G, 1);
            gpio_put(SEG_DP, 0);
            break;
        case 6:
            gpio_put(SEG_A, 1);
            gpio_put(SEG_B, 0);
            gpio_put(SEG_C, 1);
            gpio_put(SEG_D, 1);
            gpio_put(SEG_E, 1);
            gpio_put(SEG_F, 1);
            gpio_put(SEG_G, 1);
            gpio_put(SEG_DP, 0);
            break;
        case 7:
            gpio_put(SEG_A, 1);
            gpio_put(SEG_B, 1);
            gpio_put(SEG_C, 1);
            gpio_put(SEG_D, 0);
            gpio_put(SEG_E, 0);
            gpio_put(SEG_F, 0);
            gpio_put(SEG_G, 0);
            gpio_put(SEG_DP, 0);
            break;
        case 8:
            gpio_put(SEG_A, 1);
            gpio_put(SEG_B, 1);
            gpio_put(SEG_C, 1);
            gpio_put(SEG_D, 1);
            gpio_put(SEG_E, 1);
            gpio_put(SEG_F, 1);
            gpio_put(SEG_G, 1);
            gpio_put(SEG_DP, 0);
            break;
        case 9:
            gpio_put(SEG_A, 1);
            gpio_put(SEG_B, 1);
            gpio_put(SEG_C, 1);
            gpio_put(SEG_D, 1);
            gpio_put(SEG_E, 0);
            gpio_put(SEG_F, 1);
            gpio_put(SEG_G, 1);
            gpio_put(SEG_DP, 0);
            break;
        default:
            gpio_put(SEG_A, 0);
            gpio_put(SEG_B, 0);
            gpio_put(SEG_C, 0);
            gpio_put(SEG_D, 1);
            gpio_put(SEG_E, 0);
            gpio_put(SEG_F, 0);
            gpio_put(SEG_G, 0);
            gpio_put(SEG_DP, 1);
            break;
    }
}