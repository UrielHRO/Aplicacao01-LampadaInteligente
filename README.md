# 💡 Lâmpada Inteligente IoT - ESP32 & Arduino Cloud

Este projeto consiste em uma lâmpada inteligente baseada no **ESP32**, controlada de forma híbrida: tanto fisicamente (via potenciômetro e sensores) quanto remotamente através do dashboard do **Arduino IoT Cloud**.

O sistema implementa uma lógica de prioridades de segurança e eficiência energética, além de sincronização bidirecional entre o hardware e a nuvem.

## 🛠️ Hardware Utilizado

* **Microcontrolador:** ESP32 Dev Module
* **Sensores:**
    * DHT11 (Temperatura e Umidade)
    * LDR (Sensor de Luminosidade)
* **Atuadores:**
    * LED RGB (Cátodo/Anodo Comum)
    * Buzzer Ativo
* **Interface:**
    * Potenciômetro (Controle de Cor)
    * Push Button (Liga/Desliga Sistema)

## 🔌 Pinagem (Mapeamento)

| Componente | Pino ESP32 (GPIO) |
| :--- | :--- |
| DHT11 | 4 |
| LDR | 36 (VP) |
| Potenciômetro | 34 |
| Botão | 5 |
| Buzzer | 2 |
| LED Vermelho | 23 |
| LED Verde | 22 |
| LED Azul | 21 |

## 🧠 Lógica de Funcionamento

O código opera sob um **Sistema de Prioridades** para garantir segurança e automação:

1. **Prioridade 1: Segurança (Temperatura)** 
    * Se a temperatura ultrapassar **31ºC**, o sistema entra em modo de ALERTA.
    * O LED é desligado forçadamente e o Buzzer é acionado.
2.  **Prioridade 2: Eficiência (Luminosidade)** 
    * Se o ambiente estiver muito claro (LDR < 1500), o LED apaga automaticamente para economizar energia (Modo Diurno).
3. **Prioridade 3: Controle do Usuário (Cores)** 
    * Se as condições acima estiverem normais, o usuário pode escolher a cor do LED.

    * OBS: Tantos os valores das condições do LDR e DHT (1500 e 31°C) foram escolhidos com base no ambiente onde o circuito estava sendo testado, portanto, essas condições são passíveis de alteração variando o ambiente.

### 🎛️ Controle Híbrido (Sincronização Bidirecional)

Uma das principais funcionalidades deste projeto é a capacidade de controlar a cor tanto pelo **Potenciômetro Físico** quanto pelo **Slider na Web**, sem conflitos.

* O sistema monitora a variação do potenciômetro.
* [cite_start]Se o usuário girar o botão físico (variação > Threshold), o sistema assume o valor físico e atualiza a nuvem[cite: 29].
* Se o potenciômetro estiver parado, o sistema aceita o valor vindo do Arduino Cloud.
* Isso permite uma experiência fluida: "Quem mexeu por último, assume o controle".

## ☁️ Conexão com Arduino Cloud

A conexão é gerenciada pela biblioteca `ArduinoIoTCloud`. As variáveis são sincronizadas automaticamente entre o ESP32 e o Dashboard:

* `Botão` (Bool): Liga/Desliga o monitoramento.
* `temperatura` (Float): Leitura do DHT11.
* `umidade` (Float): Leitura do DHT11.
* `luminosidade` (Int): Leitura do LDR.
* `cores` (Int): Valor de 0 a 4095 que define a cor (Vermelho -> Amarelo -> Azul).


## 🚀 Como Executar
* Instale o Arduino IDE.

Instale as bibliotecas necessárias:

* ArduinoIoTCloud

* Arduino_ConnectionHandler

* DHT sensor library

* Configure o arquivo thingProperties.h com seu Device ID, Secret Key e credenciais Wi-Fi.

* Carregue o código LampadaInteligente.ino no ESP32.
