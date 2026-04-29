/* Frequência do clock do microcontrolador (Hz) */
#define FOSC 16000000

/* Taxa de transmissão serial (bps) */
#define BAUD 9600

/* Valor do registrador UBRR para o baud rate definido */
#define MYUBRR FOSC / 16 / BAUD - 1

/* Inicializa a UART para transmissão e recepção serial */
/* Configura o USART0 para 8 bits, sem paridade e com RX/TX habilitados */
void serial_init()
{
  /* Configura o baud rate e os registradores de controle da UART */
  UBRR0H = (MYUBRR >> 8);
  UBRR0L = MYUBRR;
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

/* Envia uma string pela UART */
/* msg: ponteiro para string terminada em nulo */
void serial_send(const char *msg)
{
  for (int i = 0; msg[i] != '\0'; i++)
  {
    while (!(UCSR0A & (1 << UDRE0)))
      ;
    UDR0 = msg[i];
  }
}

/* Variáveis globais para armazenar leitura do ADC e valor de saída */
int sensorValue = 0;
int outputValue = 0;

/* Configura o ADC, PWM e inicializa a UART */
void setup()
{
  /* Define a referência de tensão como AVcc (5V) no canal ADC0 */
  ADMUX |= (1 << REFS0);

  /* Habilita o ADC com prescaler de 128 */
  ADCSRA |= (1 << ADEN);

  /* Configura o timer 1 para PWM com modo Fast PWM */
  TCCR1B |= (1 << WGM12);
  TCCR1A |= (1 << WGM10);
  TCCR1A |= (1 << COM1A1);

  /* Define PB1 como saída digital para o LED */
  DDRB |= (1 << PB1);

  serial_init();
}

/* Faz leitura do ADC, define PWM e envia a saída via serial */
void loop()
{

  /* Inicia uma conversaão ADC e aguarda seu término */
  ADCSRA |= (1 << ADSC);
  while (bit_is_set(ADCSRA, ADSC))
    ;
  sensorValue = ADCL | (ADCH << 8);

  /* Mapeia o valor lido para a faixa 0-255 e ajusta o PWM */
  outputValue = map(sensorValue, 0, 1023, 0, 255);
  OCR1A = outputValue;

  /* Formata e envia a saída via serial */
  char msg_buf[64];
  int percent = outputValue * 100 / 255;
  snprintf(msg_buf, sizeof(msg_buf), "ADC = %d | %% Motor = %d\n", sensorValue, percent);
  serial_send(msg_buf);
  delay(50);
}