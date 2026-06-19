// Vinicius Trindade 15491900
// Mapeamento dos pinos do LCD conforme padrao do Kit EasyPIC
sbit LCD_RS at RB4_bit;
sbit LCD_EN at RB5_bit;
sbit LCD_D4 at RB0_bit;
sbit LCD_D5 at RB1_bit;
sbit LCD_D6 at RB2_bit;
sbit LCD_D7 at RB3_bit;

sbit LCD_RS_Direction at TRISB4_bit;
sbit LCD_EN_Direction at TRISB5_bit;
sbit LCD_D4_Direction at TRISB0_bit;
sbit LCD_D5_Direction at TRISB1_bit;
sbit LCD_D6_Direction at TRISB2_bit;
sbit LCD_D7_Direction at TRISB3_bit;

// Definicao dos pinos de I/O
sbit BTN_1 at RD0_bit;      // Inicia a contagem e leitura
sbit BTN_2 at RD1_bit;      // Seleciona entre 60s e 10s (e pausa o sistema)
sbit LED_TEMP at RD2_bit;   // Acende se Temp > 50 °C

// Variaveis globais de controle de tempo e estado
volatile unsigned int tempo_atual = 60; // Inicia com 60s
volatile bit flag_1s;                   // Flag unificada de passagem de 1s
volatile unsigned short cont_250ms = 0; // Contador auxiliar para o Timer1
bit iniciou_sistema;                    // Flag de controle de partida
bit modo_curto;                     // 0 = 60s (Timer0), 1 = 10s (Timer1)

// Rotina de Interrupcao do PIC18
void interrupt() {
    // Tratamento da interrupcao do Timer0 (Base de 1s para contagem longa)
    if (INTCON.TMR0IF) {
        TMR0H = 0x0B;            // Recarrega byte alto (1 segundo a 8MHz)
        TMR0L = 0xDC;            // Recarrega byte baixo

        // So aciona a flag se estiver no modo de 60s
        if (modo_curto == 0) {
            flag_1s = 1;
        }
        INTCON.TMR0IF = 0;       // Limpa flag do Timer0
    }

    // Tratamento da interrupcao do Timer1 (Base de 250ms para contagem curta)
    if (PIR1.TMR1IF) {
        TMR1H = 0x0B;            // Recarrega byte alto (250ms a 8MHz c/ Prescaler 1:8)
        TMR1L = 0xDC;            // Recarrega byte baixo

        cont_250ms++;
        if (cont_250ms >= 4) {   // 4 x 250ms = 1 segundo
            cont_250ms = 0;
            // So aciona a flag se estiver no modo de 10s
            if (modo_curto == 1) {
                flag_1s = 1;
            }
        }
        PIR1.TMR1IF = 0;         // Limpa flag do Timer1
    }
}

// Funcao para exibir o tempo no LCD
void Exibir_Tempo(unsigned int valor) {
    char txt_tempo[4];
    ByteToStr(valor, txt_tempo); // Converte para string
    Lcd_Out(1, 12, txt_tempo);   // Exibe na linha 1
}

// Funcao para converter e exibir a Temperatura sem usar float
void Exibir_Temperatura(unsigned int valor_adc) {
    unsigned long temp_x10 = ((unsigned long)valor_adc * 1000) / 1023;
    unsigned int parte_inteira = temp_x10 / 10;
    unsigned int parte_decimal = temp_x10 % 10;
    char txt_temp[10];

    if (parte_inteira > 50) {
        LED_TEMP = 1;
    } else {
        LED_TEMP = 0;
    }

    if (parte_inteira >= 100) {
        txt_temp[0] = '1';
        txt_temp[1] = '0';
        txt_temp[2] = '0';
    } else {
        txt_temp[0] = ' ';
        txt_temp[1] = (parte_inteira / 10) + '0';
        txt_temp[2] = (parte_inteira % 10) + '0';
        if (txt_temp[1] == '0') txt_temp[1] = ' ';
    }

    txt_temp[3] = '.';
    txt_temp[4] = parte_decimal + '0';
    txt_temp[5] = ' ';
    txt_temp[6] = 223; // Simbolo de grau (°)
    txt_temp[7] = 'C';
    txt_temp[8] = ' '; // Espaco apagador
    txt_temp[9] = '\0';

    Lcd_Out(2, 8, txt_temp);
}

void main() {
    // Configuracao do ADC e Pinos Analogicos
    ADCON1 = 0x1B;
    TRISA.B0 = 1;
    TRISA.B3 = 1;

    // Configuracao de Entradas/Saidas Digitais
    TRISD0_bit = 1; // RD0 (BTN_1) Entrada
    TRISD1_bit = 1; // RD1 (BTN_2) Entrada
    TRISD2_bit = 0; // RD2 (LED) Saida
    LED_TEMP = 0;   // Inicia com LED desligado

    iniciou_sistema = 0;
    modo_curto = 0;
    tempo_atual = 60;

    // Inicializacao do LCD
    Lcd_Init();
    Lcd_Cmd(_LCD_CLEAR);
    delay_ms(10);
    Lcd_Cmd(_LCD_CURSOR_OFF);
    Lcd_Out(1, 1, "Contagem:");
    Lcd_Out(2, 1, "Temp:");
    Exibir_Tempo(tempo_atual);

    ADC_Init();
    ADCON1 = 0x1B;

    // Configuracao do Timer0 (Base 1s - 16 bits, Prescaler 1:32)
    T0CON = 0x84;
    TMR0H = 0x0B;
    TMR0L = 0xDC;

    // Configuracao do Timer1 (Base 250ms - 16 bits, Prescaler 1:8)
    // 0xB1 = 10110001 -> RD16=1 (16 bits), T1CKPS=11 (1:8), TMR1ON=1
    T1CON = 0xB1;
    TMR1H = 0x0B;
    TMR1L = 0xDC;

    // Interrupcoes
    INTCON.TMR0IE = 1; // Habilita Timer0
    PIE1.TMR1IE = 1;   // Habilita Timer1
    INTCON.PEIE = 1;   // Interrupcao de Perifericos (Necessario para o Timer1)
    INTCON.GIE = 1;    // Interrupcao Global

    while(1) {
        // --- CONTROLE DOS BOTOES ---

        // Botao 1: Inicia o sistema
        if (BTN_1 == 1) {
            delay_ms(30);
            if (BTN_1 == 1) {
                iniciou_sistema = 1;
            }
        }

        // Botao 2: Alterna entre 10s e 60s (Funciona como seletor e reset/pausa)
        if (BTN_2 == 1) {
            delay_ms(30);
            if (BTN_2 == 1) {
                iniciou_sistema = 0;       // Pausa o sistema ao alterar o tempo
                LED_TEMP = 0;              // Desliga LED

                modo_curto = ~modo_curto;  // Inverte o modo (0 vira 1, 1 vira 0)

                if (modo_curto) {
                    tempo_atual = 10;
                } else {
                    tempo_atual = 60;
                }

                Lcd_Cmd(_LCD_CLEAR);
                delay_ms(10);
                Lcd_Out(1, 1, "Contagem:");
                Lcd_Out(2, 1, "Temp:");
                Exibir_Tempo(tempo_atual);    // Atualiza o LCD com o novo tempo selecionado

                while(BTN_2 == 1);            // Anti-repeticao
            }
        }

        // --- MODO DE EXECUCAO ---
        if (iniciou_sistema) {

            // Controle de decaimento do tempo
            if (flag_1s) {
                flag_1s = 0; // Reseta a flag unificada
                if (tempo_atual > 0) {
                    tempo_atual--;
                    Exibir_Tempo(tempo_atual);
                }
            }

            // Realiza a leitura e exibicao continua da Temperatura
            Exibir_Temperatura(ADC_Get_Sample(0));
            delay_ms(100);
        }
    }
}
