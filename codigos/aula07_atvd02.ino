/* Definicoes de variáveis utilizadas na atividade. */
/* fez-se uso do led em PB0 (pino 8) e, após cál-   */
/* culos (descritos acima da função "vLed"), defi-  */
/* niu-se como limiar do led, a tensão 4.2 V        */

#define PIN_LED PB0
#define V_LIMIAR 4.2

void setup()
{
  	/* Configura o pino PB0 como saída */
  	DDRB |= (1 << PIN_LED);
  
	/* Inicializa a comunicação serial em 9600 bps */
	Serial.begin(9600);
  
  	/* 
     * Configuração do ADC:
     * REFS0 = 1 -> utiliza AVcc (5V) como referência
     * Canal ADC0 selecionado por padrão
     */
  	ADMUX = (1 << REFS0);

  	/*
     * Configuração do registrador ADCSRA:
     * ADEN  -> habilita o ADC
     * ADPS2:0 -> define prescaler em 128
     * Frequência do ADC:
     * 16 MHz / 128 = 125 kHz
     * Valor dentro da faixa recomendada do datasheet
     */
  	ADCSRA = (1 << ADEN)|(1 << ADPS2)|(1 << ADPS1)|(1 << ADPS0);
}

void loop()
{
  	/* Inicia uma nova conversão ADC */
  	ADCSRA |= (1 << ADSC);

  	/* 
     * Aguarda o término da conversão
     * Enquanto ADSC = 1, a conversão ainda está em andamento
     */
  	while(ADCSRA & (1 << ADSC));
  	
  	/* Armazena o valor digital convertido */
  	int leitura = ADC;

  	/* 
     * Converte o valor digital para tensão:
     * resolução = 10 bits -> 0 a 1023
     * referência = 5V
     */
  	float ten_ADC = (leitura * 5.0) / 1023.0;
  
  	/* Verifica se a tensão é suficiente para acionar o LED */
  	vLed(ten_ADC);

  	/* Exibe o valor digital no monitor serial */
  	Serial.println("Valor digital: ");
  	Serial.println(leitura);
  
  	/* Aguarda 200 ms antes da próxima leitura */
  	delay(200);
}

/* FUNÇÕES DA ATIVIDADE */

/* 
 * Julgou-se 2.0V como tensão típica do LED
 * 
 * Cálculo da tensão mínima necessária:
 * V_total = V_LED + (R * I)
 * V_total = 2.0V + (220 * 0.01)
 * V_total = 4.2V
 */
void vLed(float tensao)
{
  /* 
   * Caso a tensão medida seja maior que o limiar,
   * o LED é acionado
   */
  if (tensao > V_LIMIAR)
  {
    PORTB |= (1 << PIN_LED);
  }
  else
  {
    /* Caso contrário, o LED permanece desligado */
    PORTB &= ~(1 << PIN_LED);
  }
}