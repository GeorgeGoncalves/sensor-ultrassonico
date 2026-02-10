// =============================
// Variaveis globais
// =============================
// Sensor
unsigned long tempoSensor = 0;
const unsigned long intervaloSensor = 100;  // Tempo em milessegundos
float distancia = 0;                        // Variavel para amarzenar a conversão em centimetros(cm)

// Buzzer
unsigned long tempoBuzzer = 0;  // Tempo que o buzzer fica ligado
bool buzzerLigado = false;      // Condição para ligar o buzzer


// Variavel para mostrar o estado atual do buzzer
enum ModoAlerta {
  DESLIGADO,     // Sirene inativa
  BAIXO,         // Intensidade mínima
  MODERADO,      // Intensidade intermediária
  ELEVADO,       // Atenção maior
  VIGILANTE,     // Estado de prontidão
  CRITICO,       // Alerta forte
  EMERGENCIA     // Máxima urgência
};

ModoAlerta modoAtual = DESLIGADO;


// =============================
// Pinos utilizados
// =============================
// Sensor
#define TRIG_PIN D5  // Sensor
#define ECHO_PIN D6  // Sensor

// LEDs
#define LED0 D0  // LED verde
#define LED1 D1  // LED verde
#define LED2 D2  // LED verde
#define LED3 D3  // LED amarelo
#define LED4 D4  // LED vermelho
#define LED5 D7  // LED vermelho

// Buzzer
#define pinBuzzer D8  // Buzzer


// =============================
// Função para acionar o buzzer
// =============================
void atualizarBuzzer() {
  unsigned long agora = millis();

  unsigned long tempoON = 0;    // Valor inicial seguro
  unsigned long tempoOFF = 0;   // Evita lixo de memória

  switch (modoAtual) {
    case DESLIGADO:
      digitalWrite(pinBuzzer, LOW);
      buzzerLigado = false;
      return;

    case BAIXO:
      tempoON = 100;
      tempoOFF = 1000;
      break;

    case MODERADO:
      tempoON = 200;
      tempoOFF = 700;
      break;

    case ELEVADO:
      tempoON = 300;
      tempoOFF = 500;
      break;

    case VIGILANTE:
      tempoON = 200;
      tempoOFF = 200;
      break;

    case CRITICO:
      tempoON = 100;
      tempoOFF = 100;
      break;

    case EMERGENCIA:
      tempoON = 100;
      tempoOFF = 0;
      break;
  }

  // Máquina de estados o buzzer
  if (buzzerLigado) {
    if (agora - tempoBuzzer >= tempoON) {
      digitalWrite(pinBuzzer, LOW);
      buzzerLigado = false;
      tempoBuzzer = agora;
    }
  } else {
    if (agora - tempoBuzzer >= tempoOFF) {
      digitalWrite(pinBuzzer, HIGH);
      buzzerLigado = true;
      tempoBuzzer = agora;
    }
  }
}


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
  // Apaga todos os LEDs no início (estado limpo)
  digitalWrite(LED0, LOW);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED4, LOW);
  digitalWrite(LED5, LOW);

  // Verificação da distância
  if (distancia > 31) {
    modoAtual = DESLIGADO;

  } else if (distancia > 30) {
    digitalWrite(LED0, HIGH);
    modoAtual = BAIXO;

  } else if (distancia > 25) {
    digitalWrite(LED0, HIGH);
    digitalWrite(LED1, HIGH);
    modoAtual = MODERADO;

  } else if (distancia > 20) {
    digitalWrite(LED0, HIGH);
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    modoAtual = ELEVADO;

  } else if (distancia > 15) {
    digitalWrite(LED0, HIGH);
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    modoAtual = VIGILANTE;

  } else if (distancia > 10) {
    digitalWrite(LED0, HIGH);
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, HIGH);
    modoAtual = CRITICO;

  } else {
    digitalWrite(LED0, HIGH);
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
    digitalWrite(LED4, HIGH);
    digitalWrite(LED5, HIGH);
    modoAtual = EMERGENCIA;

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

  pinMode(pinBuzzer, OUTPUT);  // Buzzer

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

  // === TAREFA 2: LEDs ===
  atualizarLEDs();

  // === TAREFA : BUZZER ===
  atualizarBuzzer();
}