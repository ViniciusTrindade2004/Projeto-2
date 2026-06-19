# Projeto 2: Aferidor de Temperatura de Forno Industrial

Disciplina: SEL0433 - Aplicação de Microprocessadores   

Aluno: Vinícius Rafael Trindade, nº USP 15491900

## Introdução e Descrição do Projeto

Este projeto foi desenvolvido como solução para uma indústria metalúrgica, com o objetivo de projetar um dispositivo de aferição de temperatura e tempo de um forno industrial. Fornos industriais são essenciais em processos como fabricação metálica e manipulação de químicos, exigindo monitoramento constante e preciso.

O sistema baseia-se no microcontrolador PIC18F4550 e simula a leitura de um sensor de temperatura LM35 (por meio de um potenciômetro) na faixa de 0°C a 100°C. Através de botões com tratamento de debounce, o usuário pode alternar entre dois modos de tempo de aferição (60 segundos ou 10 segundos) e iniciar a contagem regressiva. Durante o funcionamento, o sistema exibe os dados atualizados em um display LCD e aciona um LED de alerta caso a temperatura ultrapasse a marca de 50°C, representando o acionamento da resistência do forno.

Dentre os recursos utilizados, destaca-se os seguintes:

* Timers e Interrupções: Utilização simultânea de múltiplos temporizadores de hardware. O Timer0 atua gerando uma base de tempo precisa de 1 segundo (para a contagem longa de 60s), enquanto o Timer1 gera uma base de 250ms (para a contagem curta de 10s).
* Conversor Analógico-Digital (ADC) com resolução de 10 bits.
* Display LCD (Modo 4 bits).
* Otimização de Memória: O processamento matemático da temperatura foi feito através de manipulação de inteiros (unsigned long e módulo/divisão), evitando o uso de variáveis do tipo float, que ocupam grande quantidade de memória RAM de dados no PIC.

## Resultados e Discussão

O firmware foi compilado com sucesso e o arquivo hex foi carregado no simulador SimulIDE. Seguem abaixo imagens que mostram a compilação, a contagem curta e a contagem longa, respectivamente:

![Compilação sucedida](https://github.com/ViniciusTrindade2004/Projeto-2/blob/main/compilacao.png)
![Contagem curta](https://github.com/ViniciusTrindade2004/Projeto-2/blob/main/contagem_curta.png)
![Contagem longa](https://github.com/ViniciusTrindade2004/Projeto-2/blob/main/contagem_longa.png)

O botão inferior atua como chave seletora e pausa. Ao ser pressionado, ele interrompe a aferição e contagem e alterna o cronômetro entre as configurações de 60 segundos e 10 segundos, limpando e atualizando o LCD.

O botão superior aciona o início da contagem regressiva, gerida pelos timers via interrupções, além de iniciar a aferição da temperatura.

O potênciometro está ligado a 1V e permite variações de temperatura de 0°C a 100°C.

Nota-se que na imagem da contagem curta, a temperatura é menor que 50°C e portanto o LED está apagado. No caso da contagem longa, ela é superior a 50°C e o LED está aceso.
