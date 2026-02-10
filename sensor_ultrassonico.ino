unsigned long tempoSensor = 0;
const unsigned long intervaloSensor = 100;  // Tempo em milessegundos

float distancia = 0;  // Variavel para amarzenar a conversão em centimetros(cm)

// =============================
// Pinos do sensor ultrassônico
// =============================
#define TRIG_PIN D5  // Sensor
#define ECHO_PIN D6  // Sensor

// =============================
// Pinos dos LEDs
// =============================
#define LED0 D0  // LED verde
#define LED1 D1  // LED verde
#define LED2 D2  // LED verde
#define LED3 D3  // LED amarelo
#define LED4 D4  // LED vermelho
#define LED5 D7  // LED vermelho


// =============================
// Função para medir a distância
// =============================
void medirDistancia() {
  // Iniciar sensor desligado
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  // Iniciar por 10us e desliga
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Aguardar resposta e converter para "cm"
  long duracao = pulseIn(ECHO_PIN, HIGH, 30000);

  if (duracao == 0) return;

  distancia = duracao * 0.034 / 2;

  // Imprimir no serial monitor
  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");
}


// =============================
// Função que atualiza os LEDs
// =============================
void atualizarLEDs() {
  // Iniciar LEDs desligados
  digitalWrite(LED0, LOW);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  digitalWrite(LED5, LOW);

  // Verificação de qual LED deve acender
  if (distancia > 31) {
  digitalWrite(LED0, LOW);
    // Não faça nada
  }
  else if (distancia > 30) {
  digitalWrite(LED0, HIGH);
  }
  else if (distancia > 25) {
    digitalWrite(LED0, HIGH);
    digitalWrite(LED1, HIGH);
  }
  else if (distancia > 20) {
    digitalWrite(LED0, HIGH);
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
  }
  else if (distancia > 15) {
    digitalWrite(LED0, HIGH);
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
  }
  else if (distancia > 10) {
    digitalWrite(LED0, HIGH);
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, HIGH);
  }
  else {
    digitalWrite(LED0, HIGH);
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, HIGH);
    digitalWrite(LED5, HIGH);
  }
}


// =============================
// Setup
// =============================
void setup() {
  pinMode(TRIG_PIN, OUTPUT);  // Sensor
  pinMode(ECHO_PIN, INPUT);   // Sensor

  pinMode(LED0, OUTPUT);  // LED
  pinMode(LED1, OUTPUT);  // LED
  pinMode(LED2, OUTPUT);  // LED
  pinMode(LED3, OUTPUT);  // LED
  pinMode(LED4, OUTPUT);  // LED
  pinMode(LED5, OUTPUT);  // LED

  Serial.begin(74880);  // Iniciar serial monitor
}


// =============================
// Loop
// =============================
void loop() {
  unsigned long agora = millis();

  // === TAREFA 1: SENSOR ===
  if (agora - tempoSensor >= intervaloSensor) {
    tempoSensor = agora;
    medirDistancia();
  }

  // === TAREFA 1: SENSOR ===
  atualizarLEDs();
}