# Projeto 2: Aferidor de Temperatura de Forno Industrial

Disciplina: SEL0433 - Aplicação de Microprocessadores   

Aluno: Vinícius Rafael Trindade, nº USP 15491900

## Introdução e Descrição do Projeto

Este projeto foi desenvolvido como solução para uma indústria metalúrgica, com o objetivo de projetar um dispositivo de aferição de temperatura e tempo de um forno industrial. Fornos industriais são essenciais em processos como fabricação metálica e manipulação de químicos, exigindo monitoramento constante e preciso.

O sistema baseia-se no microcontrolador PIC18F4550 e simula a leitura de um sensor de temperatura LM35 (por meio de um potenciômetro) na faixa de 0°C a 100°C. Através de botões com tratamento de debounce, o usuário pode alternar entre dois modos de tempo de aferição (60 segundos ou 10 segundos) e iniciar a contagem regressiva. Durante o funcionamento, o sistema exibe os dados atualizados em um display LCD e aciona um LED de alerta caso a temperatura ultrapasse a marca de 50°C, representando o acionamento da resistência do forno.

Dentre os recursos utilizados, destaca-se os seguintes:

* Timers e Interrupções: Utilização simultânea de múltiplos temporizadores de hardware. O Timer0 atua gerando uma base de tempo precisa de 1 segundo (para a contagem longa de 60s), enquanto o Timer1 gera uma base de 250ms (para a contagem curta de 10s).
* Conversor Analógico-Digital (ADC) com resolução de 10 bits.
* Display LCD (Modo 4 bits)
* Otimização de Memória: O processamento matemático da temperatura foi feito através de manipulação de inteiros (unsigned long e módulo/divisão), evitando o uso de variáveis do tipo float, que ocupam grande quantidade de memória RAM de dados no PIC.

## Resultados e Discussão

O firmware foi compilado com sucesso e o arquivo hex foi carregado no simulador SimulIDE. Seguem abaixo imagens que mostram a compilação, a contagem longa e a contagem curta, respectivamente



O botão seletor (**BTN_2**) atua como chave seletora (*Toggle*) e pausa. Ao ser pressionado, ele interrompe qualquer processo em andamento, reseta a lógica de segurança (desligando o LED) e alterna o cronômetro entre as configurações de **60 segundos e 10 segundos**, limpando e atualizando a interface gráfica imediatamente.

O botão de início (**BTN_1**) aciona o início da contagem regressiva, gerida pelos timers em *background* via interrupções, além de iniciar a varredura contínua da temperatura.

A formatação algorítmica cumpriu o requisito de exibir a temperatura com uma casa decimal (no formato "XX.X °C") sem a necessidade de cálculos de ponto flutuante. A lógica analógica ativou o LED digital imediatamente ao cruzar o limiar de $50^{\circ}C$. Por fim, a interface mecânica demonstrou alta resiliência contra *bouncing* (através de *delays*) e repetição indesejada de comandos (através de travas de *while* para segurar a execução enquanto o botão permanece pressionado).
