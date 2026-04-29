#include <avr/io.h>
#include <avr/interrupt.h>

/* Definição do pino do LED vermelho */
#define PIN_LED_RED   PB5

/* Definição do pino do LED verde */
#define PIN_LED_GREEN PB4

/* Definição do pino do LED azul */
#define PIN_LED_BLUE  PB3

/* Variável global compartilhada com a interrupção */
/* "volatile" evita otimizações indevidas do compilador */
volatile int counter = 0;

void setup()
{
  /* Configura os pinos dos LEDs como saída */
  DDRB |= (1 << PIN_LED_RED)| (1 << PIN_LED_GREEN) | (1 << PIN_LED_BLUE);
  
  /* Configura PD2 como entrada */
  DDRD &= ~(1 << PD2);
  
  /* Configura INT0 para interrupção na borda de descida */
  EICRA |= (1 << ISC01);

  /* Complementa configuração da borda de descida */
  EICRA &= ~(1 << ISC00);
  
  /* Habilita a interrupção externa INT0 */
  EIMSK |= (1 << INT0);
  
  /* Habilita interrupções globais */
  sei();
}

void loop()
{
  /* Verifica o valor atual do contador */
  switch(counter)
  {
    case 1:
    	/* Representa o valor binário 001 */
    	PORTB |= (1 << PIN_LED_BLUE);

    	/* Garante RED e GREEN desligados */
    	PORTB &= ~((1 << PIN_LED_GREEN)|(1 << PIN_LED_RED));
    	break;

    case 2:
    	/* Representa o valor binário 010 */
    	PORTB |= (1 << PIN_LED_GREEN);

    	/* Garante RED e BLUE desligados */
    	PORTB &= ~((1 << PIN_LED_RED)|(1 << PIN_LED_BLUE));
    	break;

    case 3:
    	/* Representa o valor binário 011 */
    	PORTB |= (1 << PIN_LED_BLUE)| (1 << PIN_LED_GREEN);

    	/* Garante RED desligado */
    	PORTB &= ~((1 << PIN_LED_RED));
    	break;

    case 4:
    	/* Representa o valor binário 100 */
    	PORTB |= (1 << PIN_LED_RED);

    	/* Garante GREEN e BLUE desligados */
    	PORTB &= ~((1 << PIN_LED_GREEN)|(1 << PIN_LED_BLUE));
    	break;

    case 5:
    	/* Representa o valor binário 101 */
    	PORTB |= (1 << PIN_LED_RED)| (1 << PIN_LED_BLUE);

    	/* Garante GREEN desligado */
    	PORTB &= ~((1 << PIN_LED_GREEN));
    	break;

    case 6:
    	/* Representa o valor binário 110 */
    	PORTB |= (1 << PIN_LED_RED)| (1 << PIN_LED_GREEN);

    	/* Garante BLUE desligado */
    	PORTB &= ~((1 << PIN_LED_BLUE));
    	break;

    case 7:
    	/* Representa o valor binário 111 */
    	PORTB |= (1 << PIN_LED_RED)|(1 << PIN_LED_GREEN)|(1 << PIN_LED_BLUE);
    	break;

    default:
    	/* Representa o valor binário 000 */
    	PORTB &= ~((1 << PIN_LED_GREEN)|(1 << PIN_LED_RED)|(1 << PIN_LED_BLUE));  	
  }
}

/* Rotina de interrupção externa INT0 */
ISR(INT0_vect)
{
  /* Incrementa o contador */
  counter ++;

  /* Verifica se o contador ultrapassou 7 */
  if (counter >=8)
  {
    /* Reinicia a contagem */
    counter = 0;
  }
}