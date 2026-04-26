// C++ code
//
int sensorValue = 0;

int outputValue = 0;

void setup() {

  // pinMode(A0, INPUT);
  // Inplicito MUX3~0 clear A0
  //  ADMUX &= ~(15)

  // Seta tensao de referencia para Vcc
  // 23.9.1 ADMUX | Table 23-3 datasheet
  ADMUX |= _BV(REFS0);
  // Ativa ADC
  ADCSRA |= _BV(ADEN);

  // PWD SETUP
  //  -- MODE
  TCCR1B |= _BV(WGM12);
  TCCR1A |= _BV(WGM10);
  TCCR1A |= _BV(COM1A1);

  // pinMode(9, OUTPUT);
  DDRB |= _BV(PB1);

  Serial.begin(9600);
}

void loop() {

  // sensorValue = analogRead(A0);
  ADCSRA |= _BV(ADSC);
  while (bit_is_set(ADCSRA, ADSC))
    ;
  sensorValue = ADCL | (ADCH << 8);

  outputValue = map(sensorValue, 0, 1023, 0, 255);

  // analogWrite(9, outputValue);
  OCR1A = outputValue;

  Serial.print("ADC = ");
  Serial.print(sensorValue);
  Serial.print(" | % Motor = ");
  Serial.println(outputValue / 255.0 * 100);
  delay(100);  // Wait for 100 millisecond(s)
}