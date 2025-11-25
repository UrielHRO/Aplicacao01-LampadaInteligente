#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>

const char DEVICE_LOGIN_NAME[]  = "305a2284-b5a3-410e-a77d-a38a9c814d2b";
const char DEVICE_KEY[]  = "12Akq?Hd2sW5IITC8Vliq172#";

const char SSID[]     = "Gradin Wifi";
const char PASS[]     = "1234567890";

void onSistemaAtivoChange();
void onCoresChange();

bool sistema_ativo;
float temperatura;
float umidade;
int luminosidade;
int cores;

void initProperties(){
  ArduinoCloud.setBoardId(DEVICE_LOGIN_NAME);
  ArduinoCloud.setSecretDeviceKey(DEVICE_KEY);

  ArduinoCloud.addProperty(sistema_ativo, READWRITE, ON_CHANGE, onSistemaAtivoChange);
  ArduinoCloud.addProperty(temperatura, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(umidade, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(luminosidade, READ, ON_CHANGE, NULL);
  ArduinoCloud.addProperty(cores, READWRITE, ON_CHANGE, onCoresChange);
}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);