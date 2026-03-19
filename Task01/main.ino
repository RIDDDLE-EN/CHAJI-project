#include <WiFiManager.h>
#include <EEPROM.h>
#include <WiFi.h>

WiFiManager wm;

int button = 13;

bool shouldSaveConfig = false;

char ssid[32];
char pass[32];

void saveConfigCallback() {
  Serial.println("Should Save Config");
  shouldSaveConfig = true;
}

void saveCredentials(const char* newSSID, const char* newPass) {
  Serial.println("Saving WiFil credentials to EEPROM ...");

  for (int i = 0; i < 32; i++) {
    EEPROM.write(0 + i, newSSID[i]);
  }

  for (int i = 0; i < 32; i++){
    EEPROM.write(100 + i, newPass[i]);
  }

  EEPROM.commit();
}

void readCredentials() {
  Serial.println("Reading WiFi credentials from EEPROM...");
  for (int i = 0; i < 32; i++) {
    ssid[i] = EEPROM.read(0 + i);
  }
  ssid[31] = '\0';

  for (int i = 0; i < 32; i++) {
    pass[i] = EEPROM.read(100 + i);
  }
  pass[31] = '\0';

  Serial.println("SSID: ");
  Serial.println(ssid);
  Serial.println("Password: ");
  Serial.println(pass);

  delay(5000);
}

void setup_wifi(){
  readCredentials();
  Serial.println("Connecting to: ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
}

void setup(){
  Serial.begin(115200);
  EEPROM.begin(512);
  
  pinMode(button, INPUT_PULLUP);

  wm.setSaveConfigCallback(saveConfigCallback);

  if (digitalRead(button) == LOW) {
    Serial.println("Button pressed, starting WiFiManager...");
    wm.startConfigPortal("ESP32_Config");

    if (shouldSaveConfig){
      saveCredentials(wm.getWiFiSSID().c_str(), wm.getWiFiPass().c_str());
      Serial.println("Credentials saved.");
      ESP.restart();
    }
  } else {
    setup_wifi();
  }
}

void loop(){
  
}
