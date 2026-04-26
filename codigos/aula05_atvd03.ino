/* Sem necessidade de definir variaveis */

void setup()
{
  /* Configuracao do pino PB5 como saida digital, */
  /* DDRB = Data Direction Register B */
  DDRB |= (1 << PB5);
}

void loop()
{
  /* Faz com que o comando seja repetido 5 vezes*/
  for (int i = 0; i < 5; i++)
  {
    /* Faz com que o registrador ative o pino PB5 */
  	PORTB |= (1 << PB5);
    
    /* Aguarda um tempo determinado (200 ms) */
  	delay(200); 
    
    /* Faz com que o registrador desabilite o pino PB5 */
  	PORTB &= ~(1 << PB5);
    
    /* Aguarda um tempo determinado (200 ms)*/
  	delay(200);
  }
  
  /* Deixa o core ocioso pelo tempo determinado */
  /* pela atividade (2000 ms) */
  delay(2000);
}