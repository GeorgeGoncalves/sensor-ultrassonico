## Sistema de Sensor de Estacionamento com ESP8266

Sistema embarcado inspirado em sensores automotivos de estacionamento, utilizando ESP8266 + HC-SR04 + LEDs + Buzzer, 
com arquitetura não bloqueante baseada em millis() e máquina de estados.

---

### Sobre o Projeto

Este projeto implementa um sistema progressivo de alerta de proximidade utilizando sensor ultrassônico.
O comportamento simula sensores automotivos reais, incluindo:

- Histerese (anti-oscilação)

- Filtro de média móvel

- Progressão sonora proporcional à distância

- Indicação visual por LEDs

- Arquitetura não bloqueante (sem delay())

O objetivo foi aplicar conceitos reais de firmware e organização de código em sistemas embarcados.

---

### Arquitetura do Sistema

```
Sensor → Filtro → Máquina de Estados → LEDs + Buzzer
```

#### Componentes Lógicos

- Leitura temporizada do sensor

- Filtro de média móvel (suavização)

- Máquina de estados com histerese

- Controle não bloqueante com millis()

- Separação de responsabilidades

---

### Hardware Utilizado

- ESP8266 (NodeMCU)

- Sensor Ultrassônico HC-SR04

- 6 LEDs

- 1 Buzzer

- Resistores para LEDs

- Transistor para o Buzzer

- Protoboard

---

### Funcionamento

O sistema classifica a distância em faixas progressivas:

| Distância  | Comportamento |
| ------------- |:-------------:|
|    > 80 cm    | Sistema desligado |
|    60–80 cm   | Beep lento     |
|    40–60 cm   |    Beep médio  |
|    25–40 cm   |   Beep rápido  |
|    10–25 cm   |  Beep muito rápido |
|    < 10 cm    | Alerta contínuo |
	
A histerese impede alternância instável quando a leitura oscila próximo aos limites.

---

### Principais Conceitos Aplicados

- Máquina de Estados

- Histerese

- Filtro de Média Móvel

- Programação Não Bloqueante

- Organização modular de firmware

- Separação entre lógica e hardware

---

### Estrutura do Código

`medirDistancia()` → leitura do HC-SR04

`filtrarDistancia()` → média móvel

`determinarModo()` → lógica de estados

`atualizarLEDs()` → controle visual

`atualizarBuzzer()` → controle sonoro não bloqueante

---

### Como Executar

1. Clone o repositório:
 ```
git clone https://github.com/seu-usuario/sensor-estacionamento-esp8266.git
```
2. Abra no Arduino IDE

3. Selecione a placa ESP8266

4. Faça o upload

---

### Objetivo Educacional

Este projeto foi desenvolvido com foco em:

- Aprimorar entendimento de sistemas embarcados

- Evoluir da programação básica para arquitetura estruturada

- Simular comportamento real de sistemas automotivos

---

### Autor

George Gonçalves Miranda
Estudante de Tecnologia com forte interesse em sistemas embarcados, firmware e arquitetura de software.
