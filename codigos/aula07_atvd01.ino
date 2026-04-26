void setup()
{
    /* Inicializa a comunicação serial com baud rate de 9600 bps                */
    Serial.begin(9600);
  
    /* Configuração do registrador ADMUX (ADC Multiplexer Selection Register)   */
    /* REFS0 = 1 → define a referência de tensão como AVcc (5V no Arduino Uno)  */
    /* Demais bits ficam em 0 → canal ADC0 selecionado por padrão               */
    ADMUX = (1 << REFS0);
  
    /* Configuração do registrador ADCSRA (ADC Control and Status Register A)   */
    /* ADEN  = 1 → habilita o ADC                                               */
    /* ADPS2, ADPS1, ADPS0 = 1 → prescaler = 128                                */
    /* (divide o clock para adequar à faixa de operação do ADC)                 */
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

void loop()
{
    /* Inicia uma conversão ADC (ADSC = 1)                         */
    ADCSRA |= (1 << ADSC);

    /* Aguarda a conversão terminar                                */
    /* Enquanto ADSC for 1 → conversão em andamento                */
    /* Quando vira 0 → conversão concluída                         */
    while(ADCSRA & (1 << ADSC));
  	
    /* Lê o valor convertido (registrador de 10 bits: ADCL + ADCH) */
    int leitura = ADC;
  
    /* Envia mensagem pela serial                                  */
    Serial.println("Valor digital: ");
    Serial.println(leitura);
  
    /* Chama função para classificar o valor lido                  */
    /* (baixo, médio ou alto)                                      */
    vChecagem(leitura);
  
    /* Pequeno atraso para evitar leituras muito rápidas           */
    delay(200);
}

void vChecagem(int valor)
{
    /* Verifica o intervalo do valor (0–1023)                      */
    /* Divide aproximadamente em 3 faixas iguais                   */

    if(valor < 341) 
        Serial.println("BAIXO");
    
    else if (valor <= 682) 
        Serial.println("MEDIO");
    
    else 
        Serial.println("ALTO");
    
    /* Quebra de linha extra para organização da saída             */
    Serial.println("\n");
}