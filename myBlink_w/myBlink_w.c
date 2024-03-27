/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/gpio.h"
#include "pico/time.h"

#define LED_PIN1 18 // Pin GPIO25 para el LED
#define LED_PIN2 19 // Pin GPIO25 para el LED
#define LED_PIN3 20 // Pin GPIO25 para el LED

int main() {
	stdio_init_all();

    // Inicializamos tres pines como salida para los LEDs
    gpio_init(LED_PIN1); // Inicializar el pin del LED
    gpio_set_dir(LED_PIN1, GPIO_OUT); // Establecer el pin del LED como salida
    gpio_init(LED_PIN2); // Inicializar el pin del LED
    gpio_set_dir(LED_PIN2, GPIO_OUT); // Establecer el pin del LED como salida
    gpio_init(LED_PIN3); // Inicializar el pin del LED
    gpio_set_dir(LED_PIN3, GPIO_OUT); // Establecer el pin del LED como salida

    int estadoLEDs[] = {1, 1, 1};

	if (cyw43_arch_init()) {
        printf("Wi-Fi init failed");
        return -1;
    }
    while (true) {

        gpio_put(LED_PIN1, estadoLEDs[0]); // Encender el LED
        gpio_put(LED_PIN2, estadoLEDs[1]); // Encender el LED
        gpio_put(LED_PIN3, estadoLEDs[2]); // Encender el LED

        sleep_ms(2000);
        estadoLEDs[0] = 0;
        gpio_put(LED_PIN1, estadoLEDs[0]); // Encender el LED

        sleep_ms(2000);
        estadoLEDs[1] = 0;
        gpio_put(LED_PIN2, estadoLEDs[1]); // Encender el LED

        sleep_ms(2000);
        estadoLEDs[2] = 0;
        gpio_put(LED_PIN3, estadoLEDs[2]); // Encender el LED

        sleep_ms(2000);
        estadoLEDs[2] = 1;
        estadoLEDs[0] = 1;
        estadoLEDs[1] = 1;

        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(250);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        sleep_ms(250);
    }
}
