////////////////////////////////////////////////
/*     PROJETO SENSOR DE RÉ COM REGULAÇÃO     */
////////////////////////////////////////////////

/*
 * O presente projeto tem por objetivo desenvolver
 * um sensor de ré utilizando o microcontrolador
 * ATmega328P, com a plataforma Arduino.
 *
 * O sistema mede a distância entre o sensor
 * ultrassônico e possíveis obstáculos posicionados
 * atrás do veículo.
 *
 * A partir da distância detectada:
 * - O display LCD informa os valores medidos;
 * - LEDs indicam nível de proximidade;
 * - O buzzer gera alertas sonoros;
 * - O potenciômetro ajusta a distância limite
 *   para ativação do alerta.
 *
 * O acionamento do sistema é realizado através
 * de um botão conectado a uma interrupção.
 *
 * Para tal, foram utilizados os seguintes componentes:
 * - Arduino UNO R3
 * - LED vermelho
 * - LED verde
 * - LED amarelo
 * - Buzzer
 * - Display LCD 16x2
 * - Botão
 * - Sensor ultrassônico HC-SR04
 * - Potenciômetros de 10 kΩ
 * - Resistores de 330 Ω
 * - Resistor de 220 Ω
 * - Resistor de 1 kΩ
 */

/*==================================================
 *              DEFINIÇÃO DE VARIÁVEIS
 *==================================================*/

/*
 * Biblioteca responsável pelo controle
 * do display LCD 16x2 em modo de 4 bits.
 */
#include <LiquidCrystal.h>

/*
 * Biblioteca utilizada para manipulação
 * das interrupções do ATmega328P.
 */
#include <avr/interrupt.h>

/*==================================================
 *                  DEFINIÇÃO DE PINOS
 *==================================================*/

/*
 * Pinos utilizados na comunicação
 * com o display LCD.
 *
 * O display opera em modo de 4 bits,
 * utilizando apenas DB4 até DB7.
 */
#define RS             2
#define EN             3
#define DB7            10
#define DB6            9
#define DB5            8
#define DB4            7

/*
 * Pinos do sensor ultrassônico HC-SR04.
 *
 * TRIG envia o pulso ultrassônico.
 * ECHO recebe o tempo de retorno do eco.
 */
#define PIN_TRIG       5
#define PIN_ECHO       4

/*
 * Pino responsável pelo acionamento
 * do buzzer piezoelétrico.
 */
#define PIN_BUZZER     6

/*
 * Pinos conectados aos LEDs indicadores.
 *
 * Verde   -> distância segura.
 * Amarelo -> atenção.
 * Vermelho-> distância crítica.
 */
#define PIN_LED_RED    11
#define PIN_LED_GREEN  13
#define PIN_LED_YELLOW 12

/*
 * Pino conectado ao botão responsável
 * por ligar e desligar o sistema.
 */
#define PIN_BOTAO      A1

/*
 * Pino conectado ao potenciômetro.
 *
 * O valor analógico lido é utilizado
 * para ajustar a distância de alerta.
 */
#define PIN_POT        A0

/*
 * Distâncias utilizadas como referência
 * para o funcionamento do sistema.
 *
 * MIN_DIST representa a distância crítica.
 * MAX_DIST representa o limite máximo
 * ajustável pelo usuário.
 */
#define MIN_DIST 10
#define MAX_DIST 100

/*==================================================
 *                    VARIÁVEIS
 *==================================================*/

/*
 * Variável global utilizada para indicar
 * se o sistema está ativo ou desligado.
 *
 * Como é alterada dentro de interrupção,
 * deve ser declarada como volatile.
 */
volatile bool isRe = 0;

/*
 * Objeto responsável pelo controle
 * do display LCD.
 */
LiquidCrystal lcd(RS, EN, DB4, DB5, DB6, DB7);

/*==================================================
 *              PROTÓTIPOS DE FUNÇÃO
 *==================================================*/

long HC_read();

float HC_dist(long duracao);

float ADC_read();

void LED_acender(float distancia, float limite);

void BUZZER_pwm(int tempo_us, int duracao_ms);

void BUZZER_alerta(float distancia, float limite);

/*==================================================
 *                  SETUP / LOOP
 *==================================================*/

void setup()
{
    /*
     * Inicializa o display LCD
     * configurando 16 colunas
     * e 2 linhas.
     */
    lcd.begin(16, 2);

    /*
     * Posiciona o cursor na primeira linha
     * e exibe mensagem de inicialização.
     */
  	lcd.setCursor(0,0);
  	lcd.print("INICIALIZANDO");

    /*
     * Pequena pausa para indicar
     * início do sistema.
     */
  	delay(3000);

    /*
     * Configura os pinos dos LEDs
     * como saída digital.
     */
    pinMode(PIN_LED_RED, OUTPUT);
    pinMode(PIN_LED_GREEN, OUTPUT);
    pinMode(PIN_LED_YELLOW, OUTPUT);

    /*
     * Configura o buzzer
     * como saída digital.
     */
    pinMode(PIN_BUZZER, OUTPUT);

    /*
     * Configura o sensor HC-SR04.
     *
     * TRIG -> saída.
     * ECHO -> entrada.
     */
    pinMode(PIN_TRIG, OUTPUT);
    pinMode(PIN_ECHO, INPUT);

    /*
     * Configura o botão
     * como entrada digital.
     */
    pinMode(PIN_BOTAO, INPUT);

    /*
     * Habilita interrupções do grupo
     * PCINT[14:8], correspondente
     * à PORTC do ATmega328P.
     */
	PCICR |= (1 << PCIE1);

    /*
     * Habilita interrupção especificamente
     * no pino PC1, correspondente ao A1.
     *
     * Sempre que houver mudança de estado
     * nesse pino, a ISR será executada.
     */
 	PCMSK1 |= (1 << PCINT9);

    /*
     * Limpa o display após a inicialização.
     */
    lcd.clear();

    /*
     * Exibe mensagem indicando
     * que o sistema está desligado.
     */
    lcd.setCursor(0, 0);
    lcd.print("Sistema OFF");
}

void loop()
{
    /*
     * O funcionamento principal depende
     * do estado da variável isRe.
     *
     * isRe = 1 -> sistema ativo.
     * isRe = 0 -> sistema desligado.
     */
    switch(isRe)
    {
        case 1:
        {
            long duracao;
            float distancia;
            float limite_usuario;

            /*
             * Realiza o disparo do sensor
             * ultrassônico e mede o tempo
             * de retorno do eco.
             */
            duracao = HC_read();

            /*
             * Converte o tempo medido
             * em distância em centímetros.
             */
            distancia = HC_dist(duracao);

            /*
             * Lê o potenciômetro através
             * do ADC do Arduino.
             *
             * O valor de 0 a 1023 é convertido
             * para uma faixa entre MIN_DIST
             * e MAX_DIST.
             *
             * Esse valor representa a distância
             * limite configurada pelo usuário.
             */
            limite_usuario = map(
                ADC_read(),
                0,
                1023,
                MIN_DIST,
                MAX_DIST
            );

            /*
             * Atualiza a primeira linha
             * do display com a distância
             * atual detectada.
             */
            lcd.setCursor(0, 0);
            lcd.print("Dist: ");
            lcd.print(distancia);
            lcd.print(" cm");

            /*
             * Atualiza a segunda linha
             * mostrando o limite ajustado
             * pelo potenciômetro.
             */
            lcd.setCursor(0, 1);
            lcd.print("Lim: ");
            lcd.print(limite_usuario);
            lcd.print(" cm");

            /*
             * Atualiza o estado dos LEDs
             * de acordo com a distância.
             */
            LED_acender(
                distancia,
                limite_usuario
            );

            /*
             * Caso a distância medida
             * seja menor ou igual ao limite
             * definido pelo usuário,
             * ativa o alerta sonoro.
             */
            if(distancia <= limite_usuario)
            {
                BUZZER_alerta(
                    distancia,
                    limite_usuario
                );
            }
            else
            {
                /*
                 * Garante que o buzzer
                 * permaneça desligado.
                 */
                digitalWrite(PIN_BUZZER, LOW);

                /*
                 * Pequeno atraso para evitar
                 * atualização excessivamente rápida.
                 */
                delay(150);
            }

            break;
        }

        default:
        {
            /*
             * Mantém todos os LEDs desligados
             * enquanto o sistema estiver OFF.
             */
            digitalWrite(PIN_LED_RED, LOW);
            digitalWrite(PIN_LED_GREEN, LOW);
            digitalWrite(PIN_LED_YELLOW, LOW);

            /*
             * Mantém o buzzer desligado.
             */
            digitalWrite(PIN_BUZZER, LOW);

            /*
             * Limpa o display para evitar
             * informações antigas na tela.
             */
            lcd.clear();

            /*
             * Exibe mensagem indicando
             * sistema desativado.
             */
            lcd.setCursor(0, 0);
            lcd.print("Sistema OFF");

            /*
             * Pequeno atraso para reduzir
             * repetição excessiva do loop.
             */
            delay(200);

            break;
        }
    }
}

/*==================================================
 *               ROTINAS DE INTERRUPÇÃO
 *==================================================*/

ISR(PCINT1_vect)
{
    /*
     * Verifica o estado lógico do pino PC1.
     *
     * Quando o botão é pressionado,
     * ocorre mudança de estado no pino.
     */
    if(PINC & (1 << PC1))
    {
        /*
         * Alterna o estado da variável:
         *
         * 0 -> 1
         * 1 -> 0
         *
         * Dessa forma o botão funciona
         * como liga/desliga do sistema.
         */
        isRe ^= 1;

        /*
         * Pequeno atraso utilizado
         * como debounce simples,
         * reduzindo múltiplos acionamentos
         * causados pelo ruído mecânico
         * do botão.
         */
        delay(50);
    }
}

/*==================================================
 *                 SENSOR HC-SR04
 *==================================================*/

long HC_read()
{
    /*
     * Garante que o pino TRIG
     * comece em nível lógico baixo.
     */
    digitalWrite(PIN_TRIG, LOW);

    /*
     * Pequena estabilização do sinal.
     */
    delayMicroseconds(2);

    /*
     * Envia pulso de 10 us ao TRIG.
     *
     * O HC-SR04 interpreta esse pulso
     * como comando para emitir
     * a onda ultrassônica.
     */
    digitalWrite(PIN_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_TRIG, LOW);

    /*
     * Mede o tempo em que o pino ECHO
     * permanece em nível alto.
     *
     * Esse tempo corresponde ao percurso
     * de ida e volta da onda sonora.
     */
    return pulseIn(PIN_ECHO, HIGH);
}

float HC_dist(long duracao)
{
    /*
     * Converte o tempo do eco
     * em distância.
     *
     * 0.034 cm/us corresponde
     * aproximadamente à velocidade
     * do som no ar.
     *
     * A divisão por 2 ocorre porque
     * o tempo medido considera
     * ida e volta do sinal.
     */
    return duracao * 0.034 / 2;
}

/*==================================================
 *                   POTENCIÔMETRO
 *==================================================*/

float ADC_read()
{
    /*
     * Realiza leitura analógica
     * do potenciômetro.
     *
     * O valor retornado varia
     * entre 0 e 1023.
     */
    return analogRead(PIN_POT);
}

/*==================================================
 *                       LEDS
 *==================================================*/

void LED_acender(
    float distancia,
    float limite
)
{
    /*
     * Caso a distância seja maior
     * que o limite configurado,
     * considera-se situação segura.
     *
     * Apenas o LED verde permanece ligado.
     */
    if(distancia > limite)
    {
        digitalWrite(PIN_LED_GREEN, HIGH);
        digitalWrite(PIN_LED_YELLOW, LOW);
        digitalWrite(PIN_LED_RED, LOW);
    }

    /*
     * Caso a distância esteja abaixo
     * do limite configurado, porém acima
     * da distância crítica mínima,
     * acende o LED amarelo.
     */
    else if(distancia > MIN_DIST)
    {
        digitalWrite(PIN_LED_GREEN, LOW);
        digitalWrite(PIN_LED_YELLOW, HIGH);
        digitalWrite(PIN_LED_RED, LOW);
    }

    /*
     * Caso a distância seja muito pequena,
     * considera-se situação crítica.
     *
     * Apenas o LED vermelho permanece ligado.
     */
    else
    {
        digitalWrite(PIN_LED_GREEN, LOW);
        digitalWrite(PIN_LED_YELLOW, LOW);
        digitalWrite(PIN_LED_RED, HIGH);
    }
}

/*==================================================
 *                      BUZZER
 *==================================================*/

void BUZZER_pwm(
    int tempo_us,
    int duracao_ms
)
{
    long ciclos;

    /*
     * Calcula quantas alternâncias
     * HIGH/LOW serão necessárias
     * para manter o sinal ativo
     * durante o tempo desejado.
     */
    ciclos = (
        duracao_ms * 1000L
    ) / (tempo_us * 2);

    /*
     * Gera manualmente um sinal PWM
     * alternando o pino do buzzer
     * entre nível alto e baixo.
     */
    for(long i = 0; i < ciclos; i++)
    {
        digitalWrite(PIN_BUZZER, HIGH);
        delayMicroseconds(tempo_us);

        digitalWrite(PIN_BUZZER, LOW);
        delayMicroseconds(tempo_us);
    }
}

void BUZZER_alerta(
    float distancia,
    float limite
)
{
    int intervalo;

    /*
     * Caso a distância seja crítica,
     * mantém o buzzer praticamente contínuo.
     */
    if(distancia <= MIN_DIST)
    {
        BUZZER_pwm(500, 200);

        return;
    }

    /*
     * Calcula o intervalo entre os bipes.
     *
     * Quanto menor a distância,
     * menor será o intervalo,
     * aumentando a frequência sonora
     * do alerta.
     */
    intervalo = map(
        distancia,
        MIN_DIST,
        limite,
        50,
        1000
    );

    /*
     * Emite um bip curto.
     */
    BUZZER_pwm(500, 80);

    /*
     * Aguarda antes de emitir
     * o próximo bip.
     */
    delay(intervalo);
}