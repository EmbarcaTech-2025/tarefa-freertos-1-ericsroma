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

## Resposta das questões

1. O que acontece se todas as tarefas tiverem a mesma prioridade?

R: Se as tarefas tiverem a mesma prioridade, o escalonador do FreeRTOS irá alternar entre elas. Cada tarefa executa até chamar uma função de bloqueio (como vTaskDelay, vTaskSuspend, ou ficar esperando por um evento), ou até que seu tempo de execução termine (quantum). O sistema garante que todas as tarefas de mesma prioridade recebam tempo de CPU de forma justa. Nenhuma tarefa "preempte" a outra, pois todas têm o mesmo nível de importância para o escalonador. No código,  como todas as tarefas usam vTaskDelay, elas vão alternar normalmente e o sistema funcionará como esperado.

2. Qual tarefa consome mais tempo da CPU?

A tarefa button_task é chamada com mais frequência (a cada 100ms), enquanto as outras ficam a maior parte do tempo em espera (vTaskDelay). Portanto, button_task tende a consumir mais tempo de CPU em relação às outras.

3. Quais seriam os riscos de usar polling sem prioridades?

O uso de polling sem prioridades em um sistema multitarefa pode causar atrasos na resposta a eventos, desperdício de processamento e risco de perda de eventos rápidos, já que todas as tarefas competem igualmente pelo tempo de CPU e só verificam eventos em intervalos fixos. Isso reduz a eficiência e a responsividade do sistema, especialmente quando há necessidade de resposta rápida ou múltiplas tarefas concorrentes. O ideal é combinar polling apenas para tarefas menos críticas e usar interrupções e prioridades adequadas para eventos importantes.

## Arquivos

- `main.c`: Arquivo principal que configura e gerencia as tarefas do sistema.
- `bibliotecas/led.c` & `include/led.h`: Controle do LED RGB.
- `bibliotecas/buzzer.c` & `include/buzzer.h`: Controle do buzzer via PWM.
- `bibliotecas/button.c` & `include/button.h`: Controle dos botões A e B, incluindo debounce e interrupção.
- `CMakeLists.txt`: Configuração do projeto para build com pico-sdk e FreeRTOS.

---

## 📜 Licença
GNU GPL-3.0.