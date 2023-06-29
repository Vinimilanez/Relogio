// Define os pinos dos displays de 7 segmentos
const int segmentPins[4] = {2, 3, 4, 5};

// Define os pinos do potenciômetro e botões
const int potPin = A2;
const int potPinM = A3;
const int buttonPin1 = 6;
const int buttonPin2 = 7;
const int buttonPin3 = 8;
const int pinBuzzer = A5;

int hours = 23;
int minutes = 59;
int seconds = 0;
int alarmHours = 0;
int alarmMinutes = 0;
bool alarmMode = false;
bool alarmFrequencyMode = false;
bool setTimeMode = false;
int alarmFrequency = 1000;

void setup() {
  // Configura os pinos dos displays como saída
  for (int i = 0; i < 4; i++) {
    pinMode(segmentPins[i], OUTPUT);
  }

  for (int i = 0; i < 8; i++) {
    pinMode(i + 9, OUTPUT);
  }

  pinMode(pinBuzzer, OUTPUT);
  // Configura os pinos dos botões como entrada
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);

  Serial.begin(9600);
}

void loop() {
  // Leitura do potenciômetro para ajustar as horas e minutos
  int potValue = analogRead(potPin);
  int potValueM = analogRead(potPinM);
  if (setTimeMode) {
    hours = map(potValue, 0, 1023, 0, 2300)/100;
    minutes = map(potValueM, 0, 1023, 0, 5900)/100;
    Serial.println(hours+":"+minutes);
  }  
  
  if(alarmMode) {
    alarmHours = map(potValue, 0, 1023, 0, 23);
    alarmMinutes = map(potValueM, 0, 1023, 0, 59);
  }
   
  if(alarmFrequencyMode){
    alarmFrequency = map(potValue, 0, 1023, 500, 2000);
    tone(pinBuzzer, alarmFrequency);
  }else {
  	noTone(pinBuzzer);
  }


  // Verifica se o botão 1 foi pressionado para alternar entre ajuste de hora e ajuste de alarme
  if (digitalRead(buttonPin1) == LOW) {
    delay(50); // Debounce
    if (digitalRead(buttonPin1) == LOW) {
      	alarmMode = !alarmMode;
      	setTimeMode = false;
      	alarmFrequencyMode = false;
      Serial.print("Modo de Set Alarme = ");
      Serial.print(alarmMode);
    }
  }

  // Verifica se o botão 2 foi pressionado para reiniciar os minutos e segundos do alarme
  if (digitalRead(buttonPin2) == LOW) {
    delay(50); // Debounce
    if (digitalRead(buttonPin2) == LOW) {
      	setTimeMode = !setTimeMode;
      	alarmMode = false;
      	alarmFrequencyMode = false;
      Serial.print("Modo de setTimeMode = ");
      Serial.print(setTimeMode);
    }
  }

  // Verifica se o botão 3 foi pressionado para ajustar a frequência do alarme
  if (digitalRead(buttonPin3) == LOW) {
    delay(50); // Debounce
    if (digitalRead(buttonPin3) == LOW) {
        alarmMode = false;
        setTimeMode = false;
        alarmFrequencyMode = !alarmFrequencyMode;
      Serial.print("Modo de Set Frequencia = ");
      Serial.print(alarmFrequencyMode);
    }
  }

  // Exibe as horas e minutos nos displays de 7 segmentos
  displayTime();

  // Incrementa os segundos
  static long lastAdd;
  if(millis() - lastAdd >= 10 && !setTimeMode){
  	seconds++;
    lastAdd = millis();
  }
  
  if (seconds > 59) {
    seconds = 0;
    minutes++;
    if (minutes > 59) {
      minutes = 0;
      hours++;
      if (hours > 23) {
        hours = 0;
      }
    }
  }

  // Verifica se o alarme deve ser acionado
  if (alarmMode && hours == 0 && minutes == 0 && seconds == 0) {
    tone(9, 1000, 100); // Aciona o alarme por 100 ms
    delay(900); // Aguarda 900 ms para criar um efeito de pulso
  }
 //delay(1000); // Aguarda 1 segundo
}

// Função para exibir as horas e minutos nos displays de 7 segmentos
void displayTime() {
  int digit1, digit2, digit3, digit4;

   
  digit1 = hours / 10;
  digit2 = hours % 10;
  digit3 = minutes / 10;
  digit4 = minutes % 10;
   
  if (alarmMode) {
    digit1 = alarmHours / 10;
    digit2 = alarmHours % 10;
    digit3 = alarmMinutes / 10;
    digit4 = alarmMinutes % 10;
  } 
  
  if(alarmFrequencyMode) {
    digit1 = alarmFrequency / 1000;
    digit2 = (alarmFrequency % 1000) / 100;
    digit3 = ((alarmFrequency % 1000) % 100) / 10;
    digit4 = ((alarmFrequency % 1000) % 100) % 10;
  }

  // Desliga todos os segmentos
  for (int i = 0; i < 4; i++) {
    digitalWrite(segmentPins[i], HIGH);
  }

  // Exibe o dígito 1 (dezena das horas ou frequência do alarme)
  digitalWrite(segmentPins[0], LOW);
  displayDigit(digit1);

  // Aguarda um curto intervalo de tempo
  delay(10);

  // Desliga todos os segmentos novamente
  for (int i = 0; i < 4; i++) {
    digitalWrite(segmentPins[i], HIGH);
  }

  // Exibe o dígito 2 (unidade das horas ou frequência do alarme)
  digitalWrite(segmentPins[1], LOW);
  displayDigit(digit2);

  // Aguarda um curto intervalo de tempo
  delay(10);

  // Desliga todos os segmentos novamente
  for (int i = 0; i < 4; i++) {
    digitalWrite(segmentPins[i], HIGH);
  }

  // Exibe o dígito 3 (dezena dos minutos ou frequência do alarme)
  digitalWrite(segmentPins[2], LOW);
  displayDigit(digit3);

  // Aguarda um curto intervalo de tempo
  delay(10);

  // Desliga todos os segmentos novamente
  for (int i = 0; i < 4; i++) {
    digitalWrite(segmentPins[i], HIGH);
  }

  // Exibe o dígito 4 (unidade dos minutos ou frequência do alarme)
  digitalWrite(segmentPins[3], LOW);
  displayDigit(digit4);

  // Aguarda um curto intervalo de tempo
  delay(10);
}

// Função para exibir um dígito no display de 7 segmentos
void displayDigit(int digit) {
  // Define os padrões de segmentos para cada dígito (0 a 9)
  const byte segmentPatterns[10] = {
    B11111100,  // 0
    B01100000,  // 1
    B11011010,  // 2
    B11110010,  // 3
    B01100110,  // 4
    B10110110,  // 5
    B00111110,  // 6
    B11100000,  // 7
    B11111110,  // 8
    B11110110   // 9
  };

  // Exibe o dígito no display
  for (int i = 7; i >= 0; i--) {
    digitalWrite((7-i)+9, bitRead(segmentPatterns[digit], i));
  }
}
