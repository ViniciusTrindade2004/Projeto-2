Projeto 2: Aferidor de Temperatura de Forno Industrial

Disciplina: SEL0433 - Aplicação de Microprocessadores   

Aluno: Vinícius Rafael Trindade, nº USP 15491900

Introdução e Descrição do Projeto

Este projeto foi desenvolvido como solução para uma indústria metalúrgica, com o objetivo de projetar um dispositivo de aferição de temperatura e tempo de um forno industrial. Fornos industriais são essenciais em processos como fabricação metálica e manipulação de químicos, exigindo monitoramento constante e preciso.  

O sistema baseia-se no microcontrolador PIC18F4550 e simula a leitura de um sensor de temperatura LM35 (por meio de um potenciômetro) na faixa de 0∘C a 100∘C. Através de botões com tratamento de debounce mecânico, o usuário pode selecionar o tempo de aferição e iniciar uma contagem regressiva. Durante o funcionamento, o sistema exibe os dados atualizados em um display LCD e aciona um LED de alerta caso a temperatura ultrapasse a marca de 50∘C, representando o acionamento da resistência do forno.  

Tecnologias e Recursos Utilizados

    Timers e Interrupções: Utilização do Timer0 para gerar bases de tempo precisas de 1 segundo para a contagem de longa duração.  

    Conversor Analógico-Digital (ADC): Leitura de sinais analógicos em resolução de 10 bits.  

    Display LCD (Modo 4 bits): Interface homem-máquina para exibir dados formatados em tempo real.  

    Otimização de Memória: O processamento matemático da temperatura foi feito através de manipulação de inteiros (unsigned long e módulo/divisão), evitando o uso de variáveis do tipo float, que ocupam grande quantidade de memória de dados no PIC.  

Destaques da Implementação

Durante o desenvolvimento, algumas particularidades de hardware e software foram superadas para garantir o pleno funcionamento do firmware:

    Tensão de Referência (Vref) Externa: Para obter melhor sensibilidade com o LM35 (que responde a 10mV/°C), utilizou-se uma tensão de referência externa de 1V no pino AN3.  

    Solução do Bug da Biblioteca ADC: A biblioteca padrão ADC_Init do MikroC sobrescreve o registrador ADCON1, zerando os bits responsáveis pela Vref externa. A solução adotada foi reconfigurar o ADCON1 (ADCON1 = 0x1B;) imediatamente após a chamada da biblioteca e utilizar a função ADC_Get_Sample no lugar de ADC_Read para efetuar as leituras de forma estável.  

    Sincronização do LCD: Foram adicionados atrasos estratégicos (delay_ms(10);) após o comando de limpar tela do LCD para evitar o corte de caracteres no envio serial.

Resultados e Discussão

O firmware foi compilado com sucesso e o arquivo .hex foi carregado no simulador SimulIDE.

Durante os testes de execução, o sistema demonstrou total estabilidade. O botão de início (BTN_1) aciona adequadamente a contagem de tempo regressiva (gerida pelo TMR0 em background via interrupção), enquanto a leitura do ADC é constantemente atualizada no display LCD.

A formatação algorítmica cumpriu o requisito de exibir a temperatura com casa decimal (no formato "XX.X °C") sem corromper a memória com dados de ponto flutuante. A lógica do LED operou conforme o esperado, ativando imediatamente a saída digital configurada ao cruzar o limiar de 50∘C. A interface com os botões demonstrou resiliência contra bouncing devido aos delays contidos nas estruturas condicionais, prevenindo o reinício ou congelamento indesejado do forno.  

Imagens
