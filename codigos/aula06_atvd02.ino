/* Definicao dos pinos para os leds externos do semaforo.     */
/* Por serem valores constantes, pode-se utilizar #define     */

#define PIN_LED_GREEN  PB3
#define PIN_LED_YELLOW PB2
#define PIN_LED_RED    PB1

/* Definicao de variaveis constantes ao problema.             */

/* Tempo para verde e vermelho = 5s (5000ms)                  */
/* Tempo para amarelo = 2s (2000ms)                           */

const int time_green_red = 5000;
const int time_yellow    = 2000;


void setup()
{
  /* Definicao dos pinos determinados como saidas digitais    */
  DDRB |= (1 << PIN_LED_GREEN)
    	 |(1 << PIN_LED_YELLOW)
         |(1 << PIN_LED_RED);
}

void loop()
{
  vSemaphore(time_green_red, time_yellow);
}

//////////////////////////////////////////////
/*             FUNCOES GERAIS               */
//////////////////////////////////////////////

/* Funcao responsavel por realizar o comportamento do semaforo,*/
/* recebe os tempos ativos de cada posicao.                    */
/* A funcao funciona ativando o led desejado e respectivamente */
/* desativando os leds nao utilizados.                         */

void vSemaphore(int time_gr /*green_red*/, int time_y /*yellow*/)
{
  /* Ativa verde, desativa amarelo e vermelho.                 */
  PORTB |= (1 << PIN_LED_GREEN);
  PORTB &= ~((1 << PIN_LED_YELLOW)|(1 << PIN_LED_RED));
  delay(time_gr);
  
  /* Ativa amarelo, desativa verde e vermelho.                 */
  PORTB |= (1 << PIN_LED_YELLOW);
  PORTB &= ~((1 << PIN_LED_GREEN)|(1 << PIN_LED_RED));
  delay(time_y);
  
  /* Ativa vermelho, desativa verde e amarelo.                 */
  PORTB |= (1 << PIN_LED_RED);
  PORTB &= ~((1 << PIN_LED_YELLOW)|(1 << PIN_LED_GREEN));
  delay(time_gr);
   
}
