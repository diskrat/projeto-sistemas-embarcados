#define FOSC 16000000  // Clock Speed
#define BAUD 9600      // Baud rate for serial communication
#define MYUBRR FOSC / 16 / BAUD - 1

#define BUFFER_SIZE 41  //

void setup() {
  // PWD SETUP
  //  -- MODE
  TCCR1B |= _BV(WGM12);
  TCCR1A |= _BV(WGM10);
  TCCR1A |= _BV(COM1A1);

  // pinMode(9, OUTPUT);
  DDRB |= _BV(PB1);

  UCSR0B = _BV(RXEN0) | _BV(TXEN0);    // Enable RX and TX
  UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);  // 8-bit data
}

void loop() {
  if (UCSR0A & _BV(RXC0)) {     // Check if data is available
    char msg_buf[BUFFER_SIZE];  // Buffer to store incoming characters
    uint8_t index = 0;          // Index for the msg_buf
    int brightness = 0;         // set led brightness
    bool isInvalid = false;     // set message as invalid
    char c;                     // current char being (un)serialized

    // Read characters until newline or msg_buf is full
    while ((c = UDR0) != '\n' && index < BUFFER_SIZE - 1) {
      msg_buf[index++] = c;  // Store character in msg_buf
    }
    msg_buf[index] = '\0';  // Null-terminate the string

    // Convert msg_buf to brightness value
    for (int i = 0; i < index; i++) {
      if (!(msg_buf[i] >= '0' && msg_buf[i] <= '9')) {
        isInvalid = true;
        break;
      } else {
        brightness = brightness * 10 + (msg_buf[i] - '0');
      }
    }

    // Check brightness value and set LED
    if (!isInvalid && brightness >= 0 && brightness <= 255) {
      OCR1A = brightness;  // Set PWM value
      snprintf(msg_buf, sizeof(msg_buf), "Novo valor: %d\n", brightness);
      for (int i = 0; msg_buf[i] != '\0'; i++) {
        while (!(UCSR0A & _BV(UDRE0)))
          ;
        UDR0 = msg_buf[i];
      }
    } else {
      // Send error message using registers
      const char msg_buf[] = "Erro: Valor fora do intervalo (0-255).\n";
      for (int i = 0; msg_buf[i] != '\0'; i++) {
        while (!(UCSR0A & _BV(UDRE0)))
          ;S
        UDR0 = msg_buf[i];
      }
    }
    delay(50);
  }
