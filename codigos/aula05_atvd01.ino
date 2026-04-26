/* Definicao da variavel do tempo do led ligado/apagado (ms) */
int timer  = 3500;

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
  
  /* Faz com que o led se mantenha aceso pelo tempo timer (ms) */
  delay(timer); 
  
  /* Poe o pino PB5 em nivel logico baixo */
  PORTB &= ~(1 << PB5);
  
  /* Faz com que o led se mantenha apagado pelo tempo timer (ms) */
  delay(timer); 
}