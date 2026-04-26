/* Definicao da variavel bool do modo de operação */
bool modo  = 0;

void setup()
{
  /* Configuracao do pino PB5 como saida digital, */
  /* DDRB = Data Direction Register B */
  DDRB |= (1 << PB5);
}

void loop()
{
  /* Uso de switch case por eficiencia de codigo*/ 
  switch(modo)
    {
      /* Se "modo" == 1 (true), cai nesse caso*/
      case 1:
    	/* Poe o pino PB5 em nivel logico alto */
      	PORTB |= (1 << PB5);
    
    	/* Aguarda o tempo definido */
  		delay(200); 
    	
    	/* Poe o pino PB5 em nivel logico baixo */
  		PORTB &= ~(1 << PB5);
    
    	/* Aguarda o tempo definido */
  		delay(200); 
    	
    	/* Necessario para switch case, faz sair da funcao */
      	break;
    
      /* Se modo != 1, vem para esse caso. */
      /* Funcionamento semelhante ao "case == 1", porem */
      /* com delay de 1s ao invés de 0.2s */
      default:
      	PORTB |= (1 << PB5);
  		delay(1000); 
  		PORTB &= ~(1 << PB5);
  		delay(1000);
    }
}