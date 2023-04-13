/* Copyright 2017, Esteban Volentini - Facet UNT, Fi UNER
 * Copyright 2014, 2015 Mariano Cerdeiro
 * Copyright 2014, Pablo Ridolfi
 * Copyright 2014, Juan Cecconi
 * Copyright 2014, Gustavo Muro
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/** @file blinking.c
 **
 ** @brief Ejemplo de un led parpadeando
 **
 ** Ejemplo de un led parpadeando utilizando la capa de abstraccion de
 ** hardware y con sistema operativo FreeRTOS.
 **
 ** | RV | YYYY.MM.DD | Autor       | Descripción de los cambios              |
 ** |----|------------|-------------|-----------------------------------------|
 ** |  2 | 2017.10.16 | evolentini  | Correción en el formato del archivo     |
 ** |  1 | 2017.09.21 | evolentini  | Version inicial del archivo             |
 **
 ** @defgroup ejemplos Proyectos de ejemplo
 ** @brief Proyectos de ejemplo de la Especialización en Sistemas Embebidos
 ** @{
 */

/* === Inclusiones de cabeceras ============================================ */
#include "FreeRTOS.h"
#include "bsp.h"
#include "task.h"
#include "event_groups.h"
#include <stdbool.h>

/* === Definicion y Macros ================================================= */

#define BOT_PRUEBA_ACTIVADO (1 << 0)
#define BOT_PRUEBA_LIBERADO (1 << 4)

#define BOT_PRENDER_ACTIVADO (1 << 1)
#define BOT_PRENDER_LIBERADO (1 << 5)

#define BOT_CAMBIAR_ACTIVADO (1 << 2)
#define BOT_CAMBIAR_LIBERADO (1 << 6)

#define BOT_APAGAR_ACTIVADO (1 << 3)
#define BOT_APAGAR_LIBERADO (1 << 7)

/* === Declaraciones de tipos de datos internos ============================ */

/* === Declaraciones de funciones internas ================================= */

/* === Definiciones de variables internas ================================== */

static board_t board;

/* === Definiciones de variables externas ================================== */

EventGroupHandle_t eventos_teclas;

/* === Definiciones de funciones internas ================================== */

void Azul(void * parameters) {
    board_t param = parameters;
    EventBits_t eventos;

    while (true) {
        eventos = xEventGroupWaitBits(eventos_teclas, BOT_PRUEBA_ACTIVADO | BOT_PRUEBA_LIBERADO, pdTRUE, pdFALSE,
                                      portMAX_DELAY);
        if (eventos & BOT_PRUEBA_ACTIVADO) {
            DigitalOutputActivate(param->led_azul);
        } else if (eventos & BOT_PRUEBA_LIBERADO) {
            DigitalOutputDeactivate(param->led_azul);
        }
    }
}

void Rojo(void * parameters) {
    board_t param = parameters;

    while (true) {
        if (xEventGroupWaitBits(eventos_teclas, BOT_CAMBIAR_ACTIVADO, pdTRUE, pdFALSE, portMAX_DELAY)) {
            DigitalOutputToggle(param->led_rojo);
        }
    }
}

void Amarillo(void * parameters) {
    board_t param = parameters;

    while (true) {
        if (xEventGroupWaitBits(eventos_teclas, BOT_PRENDER_ACTIVADO, pdTRUE, pdFALSE, portMAX_DELAY)) {
            DigitalOutputActivate(param->led_amarillo);
        }
        if (xEventGroupWaitBits(eventos_teclas, BOT_APAGAR_ACTIVADO, pdTRUE, pdFALSE, portMAX_DELAY)) {
            DigitalOutputDeactivate(param->led_amarillo);
        }
    }
}

void Verde(void * parameters) {
    board_t param = parameters;

    while (true) {
        DigitalOutputToggle(param->led_verde);
        vTaskDelay(pdMS_TO_TICKS(5 * 150));
    }
}

void Teclado(void * parameters) {
    while (true) {
        if (DigitalInputHasActivated(board->boton_cambiar)) {
            xEventGroupSetBits(eventos_teclas, BOT_CAMBIAR_ACTIVADO);
        } else if (DigitalInputHasDeactivated(board->boton_cambiar)) {
            xEventGroupSetBits(eventos_teclas, BOT_CAMBIAR_LIBERADO);
        }

        if (DigitalInputHasActivated(board->boton_apagar)) {
            xEventGroupSetBits(eventos_teclas, BOT_APAGAR_ACTIVADO);
        } else if (DigitalInputHasDeactivated(board->boton_apagar)) {
            xEventGroupSetBits(eventos_teclas, BOT_APAGAR_LIBERADO);
        }

        if (DigitalInputHasActivated(board->boton_prender)) {
            xEventGroupSetBits(eventos_teclas, BOT_PRENDER_ACTIVADO);
        } else if (DigitalInputHasDeactivated(board->boton_prender)) {
            xEventGroupSetBits(eventos_teclas, BOT_PRENDER_LIBERADO);
        }

        if (DigitalInputHasActivated(board->boton_prueba)) {
            xEventGroupSetBits(eventos_teclas, BOT_PRUEBA_ACTIVADO);
        } else if (DigitalInputHasDeactivated(board->boton_prueba)) {
            xEventGroupSetBits(eventos_teclas, BOT_PRUEBA_LIBERADO);
        }

        vTaskDelay(pdMS_TO_TICKS(150));
    }
}

//void Blinking(void * parameters) {
//    while (true) {
//        DigitalOutputToggle(board->led_azul);
//        vTaskDelay(pdMS_TO_TICKS(500));
//    }
//}

/* === Definiciones de funciones externas ================================== */

/** @brief Función principal del programa
 **
 ** @returns 0 La función nunca debería termina
 **
 ** @remarks En un sistema embebido la función main() nunca debe terminar.
 **          El valor de retorno 0 es para evitar un error en el compilador.
 */
int main(void) {
    /* Inicializaciones y configuraciones de dispositivos */
    board = BoardCreate();

    eventos_teclas = xEventGroupCreate();

    /* Creación de las tareas */
    //xTaskCreate(Blinking, "Baliza", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(Azul, "Azul", configMINIMAL_STACK_SIZE, (void *)board, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(Rojo, "Rojo", configMINIMAL_STACK_SIZE, (void *)board, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(Amarillo, "Amarillo", configMINIMAL_STACK_SIZE, (void *)board, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(Verde, "Verde", configMINIMAL_STACK_SIZE, (void *)board, tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(Teclado, "TecScan", configMINIMAL_STACK_SIZE, (void *)board, tskIDLE_PRIORITY + 1, NULL);

    /* Arranque del sistema operativo */
    vTaskStartScheduler();

    /* vTaskStartScheduler solo retorna si se detiene el sistema operativo */
    while (1) {

    };

    /* El valor de retorno es solo para evitar errores en el compilador*/
    return 0;
}
/* === Ciere de documentacion ============================================== */
/** @} Final de la definición del modulo para doxygen */
