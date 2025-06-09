# Sistema Multitarefa FreeRTOS para BitDogLab

Este projeto implementa um sistema multitarefa embarcado utilizando a placa BitDogLab com o microcontrolador Raspberry Pi Pico e o kernel FreeRTOS. O sistema controla um LED RGB que alterna entre as cores, um buzzer que emite bipes periódicos e dois botões para suspender/retomar as tarefas dos periféricos, demonstrando o uso de RTOS em aplicações embarcadas.

## Objetivo

O objetivo é demonstrar o uso do FreeRTOS para gerenciamento concorrente de múltiplos periféricos em C estruturado, utilizando tarefas, controle de GPIOs e manipulação de eventos por botões.

## Lista de Materiais

| Componente            | Conexão na BitDogLab                |
|-----------------------|-------------------------------------|
| BitDogLab (RP2040)    | -                                   |
| Botão A               | GPIO5                               |
| Botão B               | GPIO6                               |
| LED RGB               | R: GPIO13 / G: GPIO11 / B: GPIO12   |
| Buzzer                | GPIO21                              |

## Execução

1. Abra o projeto no VS Code, com ambiente configurado para Raspberry Pi Pico (CMake + ARM GCC Toolchain).
2. Compile o projeto (Ctrl+Shift+B no VS Code ou via terminal com `cmake` e `make`).
3. Conecte a BitDogLab via USB e coloque a Pico em modo bootloader (pressione BOOTSEL e conecte o cabo).
4. Copie o arquivo `.uf2` gerado para a unidade RPI-RP2.
5. A Pico reiniciará automaticamente e o sistema estará pronto para uso.
6. Utilize os botões:
   - Botão A: Suspende/retoma a tarefa do LED RGB.
   - Botão B: Suspende/retoma a tarefa do buzzer.

## Funcionalidade do Sistema

### 1. Inicialização do Sistema
- O LED RGB começa alternando ciclicamente entre vermelho, verde e azul.
- O buzzer emite bipes curtos periodicamente.

### 2. Controle por Botões
- Botão A: Suspende ou retoma a tarefa do LED RGB.
- Botão B: Suspende ou retoma a tarefa do buzzer.
- Ambos os botões utilizam debounce e interrupção para evitar múltiplos acionamentos.

### 3. Retorno Visual e Sonoro
- LED RGB alterna as cores enquanto ativo.
- Buzzer emite bipes enquanto ativo.
- Ao suspender uma tarefa, o respectivo periférico para de funcionar até ser retomado.

## Fluxo de Funcionamento

1. **Sistema em Execução:** LED RGB alterna as cores e buzzer emite bipes.
2. **Suspensão:** Ao pressionar Botão A ou B, a tarefa correspondente é suspensa e o periférico para.
3. **Retomada:** Ao pressionar novamente, a tarefa é retomada e o periférico volta a funcionar.

## Arquivos

- `main.c`: Arquivo principal que configura e gerencia as tarefas do sistema.
- `bibliotecas/led.c` & `include/led.h`: Controle do LED RGB.
- `bibliotecas/buzzer.c` & `include/buzzer.h`: Controle do buzzer via PWM.
- `bibliotecas/button.c` & `include/button.h`: Controle dos botões A e B, incluindo debounce e interrupção.
- `CMakeLists.txt`: Configuração do projeto para build com pico-sdk e FreeRTOS.

---

## 📜 Licença
GNU GPL-3.0.