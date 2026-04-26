/* Definicao da variavel do tempo do led ligado (ms) */
int timer_on  = 1000;

/* Definicao da variavel do tempo do led desligado (ms)  */
int timer_off = 2000;

void setup()
{
  /* Configuracao do pino PB5 como saida digital, */
  /* DDRB = Data Direction Register B */
  DDRB |= (1 << PB5);
}

void loop()
{
  /* Poe o pino PB5 em nivel logico alto */
  PORTB |= (1 << PB5);
  
  /* Faz com que o led se mantenha aceso por timer_on (ms) */
  delay(timer_on); 
  
  /* Poe o pino PB5 em nivel logico baixo */
  PORTB &= ~(1 << PB5);
  
  /* Mantem o led desligado pelo tempo timer_off (ms) */ 
  delay(timer_off);
}