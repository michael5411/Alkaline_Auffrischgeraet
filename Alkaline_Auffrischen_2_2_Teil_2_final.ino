//Auffrischgerät für Alkaline-Batterien
//Michael Klein 1-2026 - Betriebsanleitung bitte beachten.

#include <WiFi.h>
#include <PubSubClient.h>
#include <string.h>

// ---------- Benutzer-Konfiguration ----------
const char* ssid       = "Meine SSID";
const char* wifi_pass  = "mein supersicheres Passwort";

IPAddress local_IP(192,168,0,58);
IPAddress gateway(192,168,0,1);
IPAddress subnet(255,255,255,0);
IPAddress primaryDNS(8,8,8,8);

const char* mqtt_server = "192.168.0.112";
const uint16_t mqtt_port = 1883;
const char* mqtt_user = "ESP32";
const char* mqtt_pass = "mein supersicheres MQTT-Passwort";

const char* mqtt_topic_batt1 = "mqttuser/aklader/batt1";
const char* mqtt_topic_batt2 = "mqttuser/aklader/batt2";
const char* mqtt_topic_batt3 = "mqttuser/aklader/batt3";
const char* mqtt_topic_batt4 = "mqttuser/aklader/batt4";
const char* mqtt_topic_batt1stat = "mqttuser/aklader/batt1stat";
const char* mqtt_topic_batt2stat = "mqttuser/aklader/batt2stat";
const char* mqtt_topic_batt3stat = "mqttuser/aklader/batt3stat";
const char* mqtt_topic_batt4stat = "mqttuser/aklader/batt4stat";

WiFiClient espClient;
PubSubClient client(espClient);

void connectMQTT();
void setupWiFiStatic();
void mqttPublishAll();
void messungen();
void printTable();
void statusmeldungen();

// ---------- Mess- / Lade-Variablen ----------
float battU[4] = {0.0, 0.0, 0.0, 0.0}; // Batteriespannungen 1 - 4
int battrawU[4] = {0, 0, 0, 0}; // Rohwerte der Batteriespannungen 1 - 4
int battload[4] = {10, 10, 10, 10}; // Anzahl der > Ulimit-Zustände
char battstat[4][16] = {
  "Keine Batt.",
  "Keine Batt.",
  "Keine Batt.",
  "Keine Batt."
  };
float Ulimit = 1.65; // Batteriespannung bis zu der geladen wird
float corr[4] ={1.0, 1.0, 1.0, 1.0}; //Korrekturwerte

int pwrPins[] = {21,22,25,27}; // Pins für Batterieladeimpulse
int messPins[] = {36,34,39,35}; //Pins der ADCs für Spannungsmessung

void setup() {
  Serial.begin(115200);

  for (int i=0; i<4; i++){ // I/O Ports für Pulser einstellen
    pinMode(pwrPins[i], OUTPUT);// I/O für Batterie [i]
    digitalWrite(pwrPins[i], HIGH); // Ladestrom aus
  }

  setupWiFiStatic();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(nullptr); // nr ausgehende Topics in diesem Sketch
  connectMQTT();
}

void loop() {
  if (!client.connected()) {
    connectMQTT();
  }
  client.loop();

  messungen(); // Batteriespannungen messen

  for (int j=0; j<4; j++){ // Ladeimpuls, wenn Batterie vorhanden
    if(battU[j] < Ulimit && battU[j] > 0.2 && battload[j] > 0){
      digitalWrite(pwrPins[j], LOW); // Ladestrom einschalten
      delay(250); // Ladestrom 250 ms halten
    }
    digitalWrite(pwrPins[j], HIGH); // Ladestrom aus

    if(battU[j] == 0.0){
      battload[j] = 10; // Wenn Batterie fehlt oder gerade entnommen wurde
    }
    if(battU[j] >= Ulimit){
      battload[j] -= 1; // Wenn Ulimit erreicht wurde
    }
  } // Ende der for-Schleife
  
  statusmeldungen();

  // Publish nach Messungen und Status
  mqttPublishAll();

  if(battU[0]>= Ulimit || battU[1] >= Ulimit || battU[2] >= Ulimit || battU[3] >= Ulimit){
    printTable();
  }
  
  // 1 s Pause, um die Batterien zu schonen
  delay(1000);
}