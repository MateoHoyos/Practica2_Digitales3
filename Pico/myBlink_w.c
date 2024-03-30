/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

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

// Pines reservados para los segmentos de los displays
#define SEG_A 0
#define SEG_B 1
#define SEG_C 2
#define SEG_D 3
#define SEG_E 4
#define SEG_F 5
#define SEG_G 6
#define SEG_DP 7

// Definimos los pines para los transistores
#define TRAN_1 13
#define TRAN_2 14
#define TRAN_3 15

// Secuencia de estados de los LEDs
#define SEQUENCE_LENGTH 4 // Longitud de la secuencia
bool led_states[SEQUENCE_LENGTH][3] = {
    {true, true, true},   // "111"
    {false, true, true},  // "011"
    {false, false, true}, // "001"
    {false, false, false} // "000"
};

// Índice actual de la secuencia
int sequence_index = 5;
int contador = 0;
int contadorus = 0;
int time_pessed = 0;

// Definiciones para los temporizadores
// Tiempo de actualización del display en milisegundos
#define INTERVAL_US 1000000 // Intervalo de tiempo en microsegundos (1 segundo)

#define INTERVAL_USd 5000 // Por ejemplo, actualiza cada 3 milisegundos
volatile bool Band_dis = false;
volatile uint32_t timer_ticks = 0; // Contador de ticks del temporizador
uint8_t numD = 0;
//volatile uint32_t timer_disp = 0; // Contador de ticks del temporizador


// Función de interrupción del temporizador
void timer_callback1(struct repeating_timer *t) {
    timer_ticks++; // Incrementar el contador de ticks
}
void timer_callback2(struct repeating_timer *t) {
    Band_dis = true;
}

bool to_do_sequence();
uint8_t to_do_random(uint8_t rangoMin, uint8_t rangoMax);
void encender_LED(uint8_t pindLed);

/*
// Función de interrupción del botón
void button_callback() {
    // Reiniciar el temporizador cuando se presiona el botón
    struct repeating_timer timer3;
    add_repeating_timer_ms(1000, timer_callback3, NULL, &timer3);
    printf("Botón presionado.\n");
}
*/
void timer_callback3(struct repeating_timer *t);
void timer_callback4(struct repeating_timer *t);
void timer_callback5(struct repeating_timer *t);
void sieteSegmentos(const uint8_t numero);
// Función para descomponer un entero de tres dígitos en unidades, decenas y centenas
void descomponer_entero(int numero, u_int8_t *centenas, uint8_t *decenas, u_int8_t *unidades);


int main() {
	stdio_init_all();
    gpio_init(SEG_A); // Inicializar el pin del LED
    gpio_init(SEG_B); // Inicializar el pin del LED
    gpio_init(SEG_C); // Inicializar el pin del LED
    gpio_init(SEG_D); // Inicializar el pin del LED
    gpio_init(SEG_E); // Inicializar el pin del LED
    gpio_init(SEG_F); // Inicializar el pin del LED
    gpio_init(SEG_G); // Inicializar el pin del LED
    gpio_init(SEG_DP); // Inicializar el pin del LED
    gpio_set_dir(SEG_A, GPIO_OUT); // Establecer el pin del LED como salida
    gpio_set_dir(SEG_B, GPIO_OUT); // Establecer el pin del LED como salida
    gpio_set_dir(SEG_C, GPIO_OUT); // Establecer el pin del LED como salida
    gpio_set_dir(SEG_D, GPIO_OUT); // Establecer el pin del LED como salida
    gpio_set_dir(SEG_E, GPIO_OUT); // Establecer el pin del LED como salida
    gpio_set_dir(SEG_F, GPIO_OUT); // Establecer el pin del LED como salida
    gpio_set_dir(SEG_G, GPIO_OUT); // Establecer el pin del LED como salida
    gpio_set_dir(SEG_DP, GPIO_OUT); // Establecer el pin del LED como salida

    gpio_init(TRAN_1);
    gpio_init(TRAN_2);
    gpio_init(TRAN_3);
    gpio_set_dir(TRAN_1, GPIO_OUT); // Establecer el pin del LED como salida
    gpio_set_dir(TRAN_2, GPIO_OUT); // Establecer el pin del LED como salida
    gpio_set_dir(TRAN_3, GPIO_OUT); // Establecer el pin del LED como salida

    
    // Inicializamos tres pines como salida para los LEDs
    gpio_init(LED_1); // Inicializar el pin del LED
    gpio_set_dir(LED_1, GPIO_OUT); // Establecer el pin del LED como salida
    gpio_init(LED_2); // Inicializar el pin del LED
    gpio_set_dir(LED_2, GPIO_OUT); // Establecer el pin del LED como salida
    gpio_init(LED_3); // Inicializar el pin del LED
    gpio_set_dir(LED_3, GPIO_OUT); // Establecer el pin del LED como salida

    // Configurar el temporizador para generar una interrupción cada segundo
    

    // configuracion del boton
    gpio_init(BUTTON_1); // Inicializar el pin del LED
    gpio_set_dir(BUTTON_1, GPIO_IN); // Establecer el pin del LED como salida
    gpio_pull_down(BUTTON_1); // Inicializ
    // configuracion del boton
    gpio_init(BUTTON_2); // Inicializar el pin del LED
    gpio_set_dir(BUTTON_2, GPIO_IN); // Establecer el pin del LED como salida
    gpio_pull_down(BUTTON_2); // Inicializ
    

    uint8_t u = 9;
    uint8_t d = 3;
    uint8_t c = 4;

    uint8_t randomTime = 0;
    uint8_t randomLed = 0;

    //clock_t lastUpdateTime = clock(); // Tiempo de la última actualización
    // Configurar un temporizador para controlar la secuencia de LEDs
    struct repeating_timer timer1, timer2, timer3, timer4, timer5;
    add_repeating_timer_us(INTERVAL_US, timer_callback1, NULL, &timer1);
    add_repeating_timer_us(INTERVAL_USd, timer_callback2, NULL, &timer2);
    add_repeating_timer_ms(2000, timer_callback3, NULL, &timer3);
    add_repeating_timer_ms(1000, timer_callback4, NULL, &timer4);
    add_repeating_timer_ms(1000, timer_callback5, NULL, &timer5);
    // Bucle infinito para mostrar números en los displays
    uint16_t number = 1234;

    bool flag_1 = false;
    bool flag_2 = false;
    bool flag_3 = false;
    bool flag_4 = false;

    while (1)
    {

        if (gpio_get(BUTTON_1) || flag_1) {
            flag_1 = true;
            flag_2 = to_do_sequence();
            printf("Botón presionado\n");
        }
        if (flag_2){
            flag_1 = false;
            sequence_index = 5;

            randomTime = to_do_random(0, 10);
            randomLed = to_do_random(10, 12);

            if (contador >= randomTime){
                encender_LED(randomLed);
                contadorus = 0;
                flag_2 = false;
                flag_3 = true;
            }
        }
        if(flag_3){
            
            descomponer_entero(contadorus, &c, &d, &u);
            sieteSegmentos(c);
            gpio_put(TRAN_1, 1);
            sleep_ms(5);
            gpio_put(TRAN_1, 0);

            sieteSegmentos(d);
            gpio_put(TRAN_2, 1);
            sleep_ms(5);
            gpio_put(TRAN_2, 0);

            sieteSegmentos(u);
            gpio_put(TRAN_3, 1);
            sleep_ms(5);
            gpio_put(TRAN_3, 0);

            if(gpio_get(BUTTON_2)){
                flag_3 = false;
                flag_4 = true;
                time_pessed = contadorus;
                sleep_us(5);
            }
        }
        if(flag_4){
            descomponer_entero(time_pessed, &c, &d, &u);
            sieteSegmentos(c);
            gpio_put(TRAN_1, 1);
            sleep_ms(5);
            gpio_put(TRAN_1, 0);

            sieteSegmentos(d);
            gpio_put(TRAN_2, 1);
            sleep_ms(5);
            gpio_put(TRAN_2, 0);

            sieteSegmentos(u);
            gpio_put(TRAN_3, 1);
            sleep_ms(5);
            gpio_put(TRAN_3, 0);

            gpio_put(randomLed, 0);

            if(gpio_get(BUTTON_1)){
                flag_4 = false;
                flag_1 = true;
            }
        }
    }
    
    return 0;   
}



void timer_callback3(struct repeating_timer *t){
    // Incrementar el índice de la secuencia
    sequence_index++;
    // Reiniciar el índice si llegamos al final de la secuencia
}

void timer_callback4(struct repeating_timer *t){
    contador++;
    if (contador >= 999){
        contador = 0;
    }
}

void timer_callback5(struct repeating_timer *t){
    contadorus++;
    if (contadorus >= 999){
        contadorus = 0;
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

uint8_t to_do_random(uint8_t rangoMin, uint8_t rangoMax){
    // Semilla para la función rand() basada en el tiempo actual
    srand(time(NULL));

    // Calcular el rango de valores posibles
    uint8_t rango = rangoMax - rangoMin + 1;

    // Generar el número aleatorio y ajustarlo al rango deseado
    uint8_t numero_aleatorio = (rand() % rango) + rangoMin;

    return numero_aleatorio;
}

void encender_LED(uint8_t pindLed){
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

// Función para descomponer un entero de tres dígitos en unidades, decenas y centenas
void descomponer_entero(int numero, u_int8_t *centenas, u_int8_t *decenas, u_int8_t *unidades) {
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