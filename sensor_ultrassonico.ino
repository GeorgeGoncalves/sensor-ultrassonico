// =====================================================
// SISTEMA SENSOR DE ESTACIONAMENTO - COMPLETO
// =====================================================

// =============================
// VARIAVEIS GLOBAIS
// =============================

// ========= SENSOR ============
unsigned long tempoSensor = 0;
const unsigned long intervaloSensor = 100;  // Tempo em milessegundos
float distancia = 0;                        // Variavel para amarzenar a conversão em centimetros(cm)

// ========== BUZZER ============
unsigned long tempoBuzzer = 0;  // Tempo que o buzzer fica ligado
bool buzzerLigado = false;      // Condição para ligar o buzzer

// ========= FILTRO ==============
const int NUM_AMOSTRAS = 10;
float leituras[NUM_AMOSTRAS];
int indiceLeitura = 0;
float distanciaFiltrada = 0;

// ============= ESTADO ============
enum Modo {
  DESLIGADO,
  LONGE,
  MEDIO,
  PERTO,
  MUITO_PERTO,
  CRITICO
};

Modo modoAtual = DESLIGADO;


// =============================
// PINOS
// =============================

// ========== SENSOR ===========
#define TRIG_PIN D5  // Sensor
#define ECHO_PIN D6  // Sensor

// ============ LEDs ===========
#define LED0 D0  // LED verde
#define LED1 D1  // LED verde
#define LED2 D2  // LED verde
#define LED3 D3  // LED amarelo
#define LED4 D4  // LED vermelho
#define LED5 D7  // LED vermelho

// ============ BUZZER ===========
#define pinBuzzer D8  // Buzzer


// =============================
// FILTRO - MÉDIA MÓVEL
// =============================
float filtrarDistancia(float novaLeitura) {
  leituras[indiceLeitura] = novaLeitura;
  indiceLeitura++;

  if (indiceLeitura >= NUM_AMOSTRAS)
    indiceLeitura = 0;

  float soma = 0;
  for (int i = 0; i < NUM_AMOSTRAS; i++)
    soma += leituras[i];

  return soma / NUM_AMOSTRAS;
}


// =============================
// DETERMINAR MODO COM HISTERESE
// =============================
void determinarModo() {
  switch (modoAtual) {

    case DESLIGADO:
      if (distanciaFiltrada < 80)
        modoAtual = LONGE;
      break;

    case LONGE:
      if (distanciaFiltrada > 90)
        modoAtual = DESLIGADO;
      else if (distanciaFiltrada < 60)
        modoAtual = MEDIO;
      break;

    case MEDIO:
      if (distanciaFiltrada > 70)
        modoAtual = LONGE;
      else if (distanciaFiltrada < 40)
        modoAtual = PERTO;
      break;

    case PERTO:
      if (distanciaFiltrada > 50)
        modoAtual = MEDIO;
      else if (distanciaFiltrada < 25)
        modoAtual = MUITO_PERTO;
      break;

    case MUITO_PERTO:
      if (distanciaFiltrada > 35)
        modoAtual = PERTO;
      else if (distanciaFiltrada < 12)
        modoAtual = CRITICO;
      break;

    case CRITICO:
      if (distanciaFiltrada > 18)
        modoAtual = MUITO_PERTO;
      break;
  }
}

  // =============================
  // ATUALIZAR BUZZER
  // =============================
  void atualizarBuzzer() {
    unsigned long agora = millis();

    unsigned long tempoON = 0;   // Valor inicial seguro
    unsigned long tempoOFF = 0;  // Evita lixo de memória

    switch (modoAtual) {
      case DESLIGADO:
        digitalWrite(pinBuzzer, LOW);
        buzzerLigado = false;
        return;

      case LONGE:
        tempoON = 100;
        tempoOFF = 1200;
        break;

      case MEDIO:
        tempoON = 100;
        tempoOFF = 700;
        break;

      case PERTO:
        tempoON = 120;
        tempoOFF = 400;
        break;

      case MUITO_PERTO:
        tempoON = 150;
        tempoOFF = 200;
        break;
    }

    // === MÁQUINA DE ESTADO DO BUZZER ===
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
  // MEDIR DISTÂNCIA
  // =============================
  void medirDistancia() {
    // === INICIAR SENSOR DESLIGADO ===
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);

    // === INICIAR POR 10us E DESLIGA ===
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // === AGURARDAR RESPOSTA E CONVERTER PARA 'cm ===
    long duracao = pulseIn(ECHO_PIN, HIGH, 30000);

    if (duracao == 0) {
      distancia = 200;  // Considera como longe
      return;
    }

    distancia = duracao * 0.034 / 2;
  }


  // =============================
  // ATUALIZAR LEDs
  // =============================
  void atualizarLEDs() {
    // === APAGA TODOS OS LEDs NO INÍCIO (ESTADO LIMPO) ===
    digitalWrite(LED0, LOW);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    digitalWrite(LED5, LOW);

    // === VERIFICAÇÃO DA DISTÂNCIA ===
    if (modoAtual == LONGE) {
      digitalWrite(LED0, HIGH);

    } else if (modoAtual == MEDIO) {
      digitalWrite(LED0, HIGH);
      digitalWrite(LED1, HIGH);

    } else if (modoAtual == PERTO) {
      digitalWrite(LED0, HIGH);
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);

    } else if (modoAtual == MUITO_PERTO) {
      digitalWrite(LED0, HIGH);
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, HIGH);

    } else if (modoAtual == CRITICO) {
      digitalWrite(LED0, HIGH);
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, HIGH);
      digitalWrite(LED4, HIGH);
      digitalWrite(LED5, HIGH);
    }
  }


  // =============================
  // SETUP
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
  // LOOP
  // =============================
  void loop() {
    unsigned long agora = millis();

    // 1. ATUALIZA SENSOR A CADA 100ms
    if (agora - tempoSensor >= intervaloSensor) {
      tempoSensor = agora;
      medirDistancia();

      // APLICA FILTRO
      distanciaFiltrada = filtrarDistancia(distancia);

      // ATUALIZA MODO
      determinarModo();

      // DEBUG
      Serial.print("Distancia filtrada: ");
      Serial.println(distanciaFiltrada);
    }

    // 2. ATUALIZA SAÍDAS CONTINUAMENTE
    atualizarLEDs();
    atualizarBuzzer();
  }