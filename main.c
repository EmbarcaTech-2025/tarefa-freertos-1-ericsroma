/*
 * Autor: Eric Senne Roma
 * Data de Criação: 09/06/2025
 * Descrição: Este arquivo contém o código da Atividade 1 de FreeRTOS para a BitDogLab
 *            O objetivo é implementar um sistema multitarefa usando FreeRTOS na placa BitDogLab com Raspberry Pi Pico.
 *            O sistema controla um LED RGB que alterna entre as cores, um buzzer que emite bipes periódicos,
 *            e dois botões: um para suspender/retomar o LED e outro para suspender/retomar o buzzer.
 *            O gerenciamento é feito por tarefas FreeRTOS, demonstrando controle concorrente de periféricos.
 */

 
// Inclusão das bibliotecas do FreeRTOS, periféricos e drivers da placa
#include "FreeRTOS.h"
#include "task.h"
#include "include/led.h"      // Controle do LED RGB
#include "include/buzzer.h"   // Controle do buzzer via PWM
#include "include/button.h"   // Controle dos botões A e B
#include "hardware/pwm.h"
#include "pico/stdlib.h"      // Biblioteca padrão do Pico SDK

// Handles das tarefas para controle via botões
TaskHandle_t led_task_handle = NULL;      // Handle da tarefa do LED RGB
TaskHandle_t buzzer_task_handle = NULL;   // Handle da tarefa do buzzer

/**
 * Tarefa do LED RGB
 * Alterna ciclicamente entre vermelho, verde e azul a cada 500ms.
 */
void led_task(void *pvParameters) 
{
    setup_led_rgb(); // Inicializa os pinos do LED RGB
    while (1) 
    {
        set_led_color(true, false, false);  // Liga LED vermelho
        vTaskDelay(pdMS_TO_TICKS(500));     // Aguarda 500ms
        set_led_color(false, true, false);  // Liga LED verde
        vTaskDelay(pdMS_TO_TICKS(500));     // Aguarda 500ms
        set_led_color(false, false, true);  // Liga LED azul
        vTaskDelay(pdMS_TO_TICKS(500));     // Aguarda 500ms
    }
}

/**
 * Tarefa do buzzer
 * Emite um beep curto (100ms) a cada 1 segundo.
 */
void buzzer_task(void *pvParameters) 
{
    uint buzzer_pin = 21;               // Pino do buzzer A
    pwm_init_buzzer(buzzer_pin, 4000); // Inicializa PWM do buzzer em 5Hz
    while (1) 
    {
        pwm_set_gpio_level(buzzer_pin, 128); // Liga o buzzer 
        vTaskDelay(pdMS_TO_TICKS(100));      // Beep de 100ms
        pwm_set_gpio_level(buzzer_pin, 0);   // Desliga o buzzer
        vTaskDelay(pdMS_TO_TICKS(900));      // Aguarda até completar 1s
    }
}

/**
 * Tarefa dos botões
 * Faz polling das flags dos botões a cada 100ms e suspende/retoma as tarefas do LED e do buzzer.
 */
void button_task(void *pvParameters) 
{
    setup_buttons(); // Inicializa os botões A e B
    
    while (1) 
    {
        if (button_a_pressed) 
        {
            button_a_pressed = false;
            // Suspende ou retoma a tarefa do LED RGB
            if (eTaskGetState(led_task_handle) == eSuspended) 
            {
                vTaskResume(led_task_handle);
            } 
            else 
            {
                vTaskSuspend(led_task_handle);
            }
        }
        if (button_b_pressed) 
        {
            button_b_pressed = false;
            // Suspende ou retoma a tarefa do buzzer
            if (eTaskGetState(buzzer_task_handle) == eSuspended) 
            {
                vTaskResume(buzzer_task_handle);
            } 
            else 
            {
                vTaskSuspend(buzzer_task_handle);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(100)); // Polling a cada 100ms
    }
}

/**
 * Função principal do programa
 * Inicializa o sistema, cria as tarefas e inicia o escalonador do FreeRTOS.
 */
int main() 
{
    stdio_init_all(); // Inicializa entrada/saída padrão (UART/USB)

    // Cria as tarefas do LED, buzzer e botões, atribuindo prioridades e handles
    xTaskCreate(led_task, "LED", 256, NULL, 2, &led_task_handle);
    xTaskCreate(buzzer_task, "Buzzer", 256, NULL, 1, &buzzer_task_handle);
    xTaskCreate(button_task, "Buttons", 256, NULL, 3, NULL);

    vTaskStartScheduler(); // Inicia o escalonador do FreeRTOS

    while (1) {}
    return 0;
}