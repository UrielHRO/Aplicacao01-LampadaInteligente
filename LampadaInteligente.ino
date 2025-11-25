#include "thingProperties.h"
#include "DHT.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

// HARDWARE
#define DHTTYPE DHT11
#define DHTpin  4
#define LDRpin 36
#define PotPin 34
#define Button 5

#define BuzzPin 2
#define LedR 23
#define LedG 22
#define LedB 21

// Variáveis Locais
bool LED_LOCK = false;
bool TEMP_ENABLED = true;
bool LDR_ENABLED = true;

// --- MEMÓRIA DO BOTÃO FÍSICO ---
int ultimoValorFisico = 0; 
const int THRESHOLD = 100; // Sensibilidade

volatile unsigned long lastButtonPress = 0;
const unsigned long debounceDelay = 250;

DHT dht(DHTpin, DHTTYPE);

void IRAM_ATTR rotina_botao() {
  unsigned long currentTime = millis();
  if ((currentTime - lastButtonPress) > debounceDelay) {
    sistema_ativo = !sistema_ativo; 
    lastButtonPress = currentTime;
  }
}

void setColor(int r, int g, int b) {
  analogWrite(LedR, r);
  analogWrite(LedG, g);
  analogWrite(LedB, b);
}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); 
  Serial.begin(115200);
  delay(1500); 

  // Inicializa a memória do botão
  ultimoValorFisico = analogRead(PotPin);

  dht.begin();
  pinMode(BuzzPin, OUTPUT);
  pinMode(LedR, OUTPUT);
  pinMode(LedG, OUTPUT);
  pinMode(LedB, OUTPUT);
  pinMode(Button, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(Button), rotina_botao, FALLING);

  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  ArduinoCloud.update(); 

  // --- LEITURAS  ---
  float tempLida = dht.readTemperature(); 
  float humLida  = dht.readHumidity();
  int ldrLido = analogRead(LDRpin);
  
  // --- LÓGICA DO LED HÍBRIDO ---
  int leituraAtual = analogRead(PotPin);

  if (abs(leituraAtual - ultimoValorFisico) > THRESHOLD) {
    cores = leituraAtual;           
    ultimoValorFisico = leituraAtual; 
  }

  if (isnan(tempLida)) tempLida = 0.0;
  if (isnan(humLida))  humLida = 0.0;

  temperatura = tempLida;
  umidade = humLida;
  luminosidade = ldrLido;

  // --- SAÍDAS ---
  if (!sistema_ativo) {
    noTone(BuzzPin);
    setColor(0,0,0);
  } 
  else {
    // P1: TEMPERATURA
    if (TEMP_ENABLED && (temperatura < 0 || temperatura > 31)) { 
      LED_LOCK = true;
      setColor(0,0,0);
      tone(BuzzPin, 500);
    } 
    else {
      noTone(BuzzPin);

      // P2: LUMINOSIDADE
      if (LDR_ENABLED) {
        if (luminosidade < 1500) { 
           LED_LOCK = true;
           setColor(0,0,0);
        } else {
           LED_LOCK = false;
        }
      }

      // P3: CORES
      if (!LED_LOCK) {
        if (cores < 682) setColor(255, 0, 0);
        else if (cores < 1365) setColor(255, 100, 0);
        else if (cores < 2048) setColor(255, 255, 0);
        else if (cores < 2731) setColor(255, 255, 255);
        else if (cores < 3072) setColor(0, 255, 255);
        else setColor(0, 0, 255);
      }
    }
  }
}

void onCoresChange()         {} 
void onSistemaAtivoChange()  {}
void onTemperaturaChange()   {}
void onUmidadeChange()       {}
void onLuminosidadeChange()  {}