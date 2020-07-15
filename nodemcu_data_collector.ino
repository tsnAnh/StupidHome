#include <NTPClient.h>

#include <DHT.h>

#include <FirebaseArduino.h>
#include <FirebaseObject.h>
#include <Firebase.h>
#include <FirebaseCloudMessaging.h>
#include <FirebaseError.h>
#include <FirebaseHttpClient.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <WiFiUdp.h>

#define SSID "hello123"
#define PASSWORD "00000000"
#define FIREBASE_HOST "nguhome-a3afe.firebaseio.com"
#define FIREBASE_SECRET ""

#define DHTTYPE DHT11
#define DHTPIN D4

DHT dht(DHTPIN, DHTTYPE);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.print("Wifi connecting to ");
  Serial.println(SSID);
  
  dht.begin();
  WiFi.begin(SSID, PASSWORD);

  Serial.println();
  Serial.print("Connecting...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  timeClient.begin();
  timeClient.setTimeOffset(25200);

  Serial.println();
  Serial.print("Connected!");
  

  Firebase.begin(FIREBASE_HOST, FIREBASE_SECRET);
}

void loop() {
  DynamicJsonBuffer jsonBuffer;
  // UPDATE TIME
  timeClient.update();
  unsigned long epochTime = timeClient.getEpochTime();

  unsigned int lightValue = analogRead(A0);
  Serial.println(lightValue);

  // READ DHT11 DATA
  float temperature = dht.readTemperature();
  float temperatureF = dht.readTemperature(true);
  float humidity = dht.readHumidity();

  // Create JSON Object
  JsonObject& tempObj = jsonBuffer.parseObject("{}");
  tempObj[String("time")] = epochTime;
  tempObj[String("temperature")] = temperature;
  tempObj[String("temperatureF")] = temperatureF;
  
  JsonObject& humidityObj = jsonBuffer.parseObject("{}");
  humidityObj[String("time")] = epochTime;
  humidityObj[String("humidity")] = humidity;

  JsonObject& lightObj = jsonBuffer.parseObject("{}");
  lightObj[String("time")] = epochTime;
  lightObj[String("light")] = lightValue;
  
  Firebase.set("temperature", tempObj);
  Firebase.set("humidity", humidityObj);
  Firebase.set("lightData", lightObj);
  
  delay(10000);
}
