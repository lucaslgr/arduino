#include <Arduino.h>

//--- WIFI ---
#include <ESP8266WiFi.h>
const char *ssid = "WOLF_2Ghz";
const char *password = "wheyebatatadoce";
WiFiClient nodemcuClient;

//--- MQTT CLIENT ---
#include <MQTT.h>
const char *mqtt_server = "pid-controller.cloud.shiftr.io";
// const char *mqtt_server = "test.mosquitto.org";
const char *client_id = "nodemcu";
const char *topicTemperature = "/controlproject/temperature";
MQTTClient client;
const char *mqttUser = "pid-controller";
const char *mqttPassword = "YipriUzio5iDmCar";
#define MQTT_BROKER_PORT 1883 // insecure port TCP
unsigned long lastMillis = 0;

//--- LM35 ---
#define LM35_ANALOG A0 //Setando o pino de leitura analógica do LM35

//--- SETUP ---
float temperature = 0.0;
void wifiSettingsConnectWifi();
void mqttConnectServer();
void mqttReconnect();
void measureTemperature();
void publishAtTopicTemperature();
void showMeasurements();

void setup()
{
  Serial.begin(115200);
  wifiSettingsConnectWifi();
  client.begin(mqtt_server, nodemcuClient);
  // client.onMessage(messageReceived);
  mqttConnectServer();
}

//--- LOOP ---
void loop()
{
  client.loop();
  delay(10); // <- fixes some issues with WiFi stability

  if (!client.connected())
    mqttReconnect();

  measureTemperature();
}

//--- CONECTA AO WIFI ---
void wifiSettingsConnectWifi()
{
  Serial.print("Conectando o Wifi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi conectado com sucesso!");
}

//--- CONECTA AO MQTT ---
void mqttConnectServer()
{
  Serial.print("Checando o wifi...");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\n Conectando...");
  // client.connect(client_id, "public", "public");
  while (!client.connect(client_id, mqttUser, mqttPassword))
  {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nMQTT Server Connected!");
  client.subscribe(topicTemperature);
  // client.unsubscribe("/hello");
}

//--- RECONECTA O MQTT ---
void mqttReconnect()
{
  while (!client.connected())
  {
    Serial.println("\n\rServidor mqtt não conectado, tentando a reconexão...");
    mqttConnectServer();
  }
}

//--- PUBLICA A TEMP. ---
void publishAtTopicTemperature()
{
  if (millis() - lastMillis > 1000)
  {
    lastMillis = millis();
    char valueNum[20];
    sprintf(valueNum, "%f", temperature);
    client.publish(
        topicTemperature,
        valueNum);
  }
}

//--- MEDIÇÃO DE TEMPERATURA ---
void measureTemperature()
{
  int analogicValue = analogRead(LM35_ANALOG);
  float milivolts = analogicValue * (3300.0 / 1024);
  temperature = milivolts / 10; // Cada 10mV = 1Cº [Em Celsius]

  //Tratamento para valores inválidos
  if (!isnan(temperature) && temperature != 2147483647)
  {
    publishAtTopicTemperature();
    showMeasurements();
    delay(2500);
  }
  else
  {
    Serial.println("Valor incorreto mensurado");
    delay(2500);
  }
}

//--- MOSTRA A TEMPERATURA E A UMIDADE ---
void showMeasurements()
{
  Serial.print("Temperatura: ");
  Serial.print(abs(temperature));
  Serial.println(" *C");
}