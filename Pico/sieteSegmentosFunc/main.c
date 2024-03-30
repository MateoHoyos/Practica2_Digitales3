#include <stdio.h>
#include <stdint.h>

// Pines reservados para los segmentos de los displays
#define SEG_A 0
#define SEG_B 1
#define SEG_C 2
#define SEG_D 3
#define SEG_E 4
#define SEG_F 5
#define SEG_G 6
#define SEG_DP 7

//Decaracion de la funcion para controlar el display 7-segmentos
void sieteSegmentos(const uint8_t numero);

int main()
{
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

        while (1)
        {
            for(uint8_t i = 0; i <= 9; i++){
                sieteSegmentos(i);
                sleep_ms(1000);
            }
        }

        return 0;
}

// Implementacion de la funcion para controlar el display 7-segmentos
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
