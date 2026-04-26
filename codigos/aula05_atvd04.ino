/* Sem necessidade de definir variaveis globais */

void setup()
{
  /* Configuracao do pino PB5 como saida digital, */
  /* DDRB = Data Direction Register B */
  DDRB |= (1 << PB5);
}

void loop()
{
  /* Chama a funcao piscaLed, que devera realizar 3 */
  /* piscadas, com intervalo de 1.5s entre acender  */
  /* e apagar.                                      */
  piscaLed(3, 1500);
  
  /* Deixa o core ocioso por 1s                     */
  delay(1000);
  
  /* Chama a funcao piscaLed, que deve realizar 2   */
  /* piscadas, com intervalo de 2s entre acender e  */
  /* apagar.                                        */
  piscaLed(2, 2000);
  
  /*Deixa o core ocioso por 2s                      */
  delay(2000);
}

//////////////////////////////////////////////
/*             FUNCOES GERAIS               */
//////////////////////////////////////////////


/* A funcao piscaLed recebe a quantidade de piscadas */
/* que o uC ira realizar em um laço, assim como o    */
/* tempo de delay entre as piscadas.                 */
void piscaLed(int num_blink, int delay_time)
{
  
  for (int i = 0; i <= num_blink; i++)
  {
    /* Define PB5 para nivel logico alto */
    PORTB |= (1 << PB5);
    
    /* Faz o core aguardar um tempo delay_time (ms) */
    delay(delay_time);
    
    /* Define PB5 para nivel logico baixo */
    PORTB &= ~(1 << PB5);
    
    /* Faz o core aguardar um tempo delay_time (ms) */
    delay(delay_time);
  }
}