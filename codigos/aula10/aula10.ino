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
    while (!(UCSR0A & _BV(UDRE0)))
      ;
    UDR0 = msg[i];
  }
}

/* Lê uma linha da UART até newline ou até encher o buffer */
/* buffer: buffer onde armazenar os caracteres lidos */
/* buffer_size: tamanho total do buffer */
/* retorna: número de caracteres lidos, excluindo o terminador nulo */
uint8_t serial_read_line(char *buffer, uint8_t buffer_size)
{
  uint8_t index = 0;
  char c;

  while (index < buffer_size - 1)
  {
    if (!(UCSR0A & (1 << RXC0)))
      break;

    c = UDR0;
    if (c == '\n')
      break;

    buffer[index++] = c;
  }

  buffer[index] = '\0';
  return index;
}

/* Tamanho do buffer para armazenar linha recebida */
#define BUFFER_SIZE 41

/* Configura o modo de PWM e inicializa a UART */
void setup()
{
  /* Configura o timer 1 para PWM com modo Fast PWM */
  TCCR1B |= (1 << WGM12);
  TCCR1A |= (1 << WGM10);
  TCCR1A |= (1 << COM1A1);

  /* Define PB1 como saída digital */
  DDRB |= (1 << PB1);
  serial_init();
}

/* Processa comandos seriais e ajusta o PWM conforme o valor recebido */
void loop()
{
  /* Verifica se há dados disponíveis na UART */
  if (UCSR0A & (1 << RXC0))
  {
    /* Lê uma linha completa do serial */
    char msg_buf[BUFFER_SIZE];
    uint8_t index = serial_read_line(msg_buf, BUFFER_SIZE);
    int brightness = 0;
    bool isInvalid = false;

    /* Converte os caracteres recebidos em valor numérico */
    for (int i = 0; i < index; i++)
    {
      if (!(msg_buf[i] >= '0' && msg_buf[i] <= '9'))
      {
        isInvalid = true;
        break;
      }
      else
      {
        brightness = brightness * 10 + (msg_buf[i] - '0');
      }
    }

    /* Valida e aplica o novo valor de PWM, ou envia mensagem de erro */
    if (!isInvalid && brightness >= 0 && brightness <= 255)
    {
      OCR1A = brightness;
      snprintf(msg_buf, sizeof(msg_buf), "Novo valor: %d\n", brightness);
      serial_send(msg_buf);
    }
    else
    {
      const char error_msg[] = "Erro: Valor fora do intervalo (0-255).\n";
      serial_send(error_msg);
    }
    delay(50);
  }
}