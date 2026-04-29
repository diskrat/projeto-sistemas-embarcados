/* 
 * Definição dos pinos dos LEDs do semáforo:
 * Verde, amarelo e vermelho dos carros,
 * além do verde e vermelho dos pedestres.
 */
#define PIN_LED_GREEN PB5
#define PIN_LED_YELLOW PB4
#define PIN_LED_RED PB3
#define PIN_LED_P_GREEN PB1
#define PIN_LED_P_RED PB0

/*
 * Define o pino utilizado pelo botão
 * de solicitação de travessia.
 */
#define PIN_BOTAO PD2

/*
 * Define os tempos de funcionamento
 * dos sinais do semáforo em milissegundos.
 */
#define TIME_RED_GREEN 4000
#define TIME_YELLOW 1500

/* Flag alterada pela interrupção */
volatile bool botao_pressionado = 0;

void setup()
{
  /* Configura pino do LED verde como saída */
  /* Configura pino do LED amarelo como saída */
  /* Configura pino do LED vermelho como saída */
  /* Configura LEDs do pedestre como saída */
  DDRB |= (1 << PIN_LED_GREEN |
           1 << PIN_LED_YELLOW | 
           1 << PIN_LED_RED |
           1 << PIN_LED_P_GREEN |
           1 << PIN_LED_P_RED); 
  
  /* Configura o botão como entrada digital */
  DDRD &= ~(1 << PIN_BOTAO);
  
  /* Define ISC01 = 1 */
  /* Configura INT0 para interrupção na borda de descida */
  EICRA |= (1 << ISC01);

  /* Define ISC00 = 0 */
  /* Complementa configuração da borda de descida */
  EICRA &= ~(1 << ISC00);
  
  /* Habilita a interrupção externa INT0 */
  EIMSK |= (1 << INT0);
  
  /* Habilita interrupções globais */
  sei();
}

void loop()
{
  /* Verifica estado da flag do botão */
  switch (botao_pressionado)
  {
    case 1:
    	/* Executa rotina de travessia do pedestre */
    	vPedestre();
    
    	/* Limpa flag após finalizar travessia */
    	botao_pressionado = 0;
    
    	break;
    default:
    	vSemaforo();
    	
  }  
}

////////////////////////////////////////////
/*               INTERRUPCAO              */
////////////////////////////////////////////

ISR(INT0_vect)
{
  /* Sinaliza que o botão foi pressionado */
  botao_pressionado = 1;
}


////////////////////////////////////////////
/*             FUNCOES GERAIS             */
////////////////////////////////////////////

void vSemaforo()
{
  /* Verde carros | Vermelho pedestres */
  PORTB |= (1 << PIN_LED_GREEN);
  PORTB &= ~(1 << PIN_LED_YELLOW);
  PORTB &= ~(1 << PIN_LED_RED);

  PORTB &= ~(1 << PIN_LED_P_GREEN);
  PORTB |= (1 << PIN_LED_P_RED);

  delay(TIME_RED_GREEN);

  /* Amarelo carros */
  PORTB &= ~(1 << PIN_LED_GREEN);
  PORTB |= (1 << PIN_LED_YELLOW);

  delay(TIME_YELLOW);

  /* Vermelho carros */
  PORTB &= ~(1 << PIN_LED_YELLOW);
  PORTB |= (1 << PIN_LED_RED);

  delay(TIME_RED_GREEN);
}

void vPedestre()
{
  /* Fecha trânsito dos carros */
  PORTB &= ~(1 << PIN_LED_GREEN);
  PORTB &= ~(1 << PIN_LED_YELLOW);
  PORTB |= (1 << PIN_LED_RED);

  /* Libera pedestres */
  PORTB &= ~(1 << PIN_LED_P_RED);
  PORTB |= (1 << PIN_LED_P_GREEN);

  delay(5000);

  /* Pisca verde pedestre */
  for(int i = 0; i < 5; i++)
  {
    /* Alterna estado do LED verde */
    /* XOR inverte o bit atual do pino */
    PORTB ^= (1 << PIN_LED_P_GREEN);
    delay(300);
  }

  /* Fecha pedestres */
  PORTB &= ~(1 << PIN_LED_P_GREEN);
  PORTB |= (1 << PIN_LED_P_RED);

  delay(500);

  /* Libera carros novamente */
  PORTB &= ~(1 << PIN_LED_RED);
  PORTB |= (1 << PIN_LED_GREEN);
}
