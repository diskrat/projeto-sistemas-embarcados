

/* Variáveis globais para armazenar leitura do ADC e valor de saída */
int sensorValue = 0;
int outputValue = 0;

/* Configura o ADC e PWM  */
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
  outputValue = (sensorValue >> 2);
  OCR1A = outputValue;
}
