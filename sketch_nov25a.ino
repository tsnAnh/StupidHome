#include <FirebaseESP8266.h>
#include <FirebaseESP8266HTTPClient.h>
#include <FirebaseFS.h>
#include <FirebaseJson.h>

#include <DHT.h>
#include <ESP8266WiFi.h>

#define SSID "hello123"
#define PASSWORD "00000000"
#define FIREBASE_HOST "temp-ebab3.firebaseio.com"
#define FIREBASE_AUTH "LUIJYxbNVfEksfQXs6okE7cVpzKwii7KJ071PkVS"

#define DHTTYPE DHT11
#define DHTPIN 2

FirebaseData firebaseData;

FirebaseJson json;
DHT dht(DHTPIN, DHTTYPE);

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

  Serial.println();
  Serial.print("Connected!");

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  //Set the size of WiFi rx/tx buffers in the case where we want to work with large data.
  firebaseData.setBSSLBufferSize(1024, 1024);

  //Set the size of HTTP response buffers in the case where we want to work with large data.
  firebaseData.setResponseSize(1024);

  //Set database read timeout to 1 minute (max 15 minutes)
  Firebase.setReadTimeout(firebaseData, 1000 * 60);
  //tiny, small, medium, large and unlimited.
  //Size and its write timeout e.g. tiny (1s), small (10s), medium (30s) and large (60s).
  Firebase.setwriteSizeLimit(firebaseData, "tiny");

  //optional, set the decimal places for float and double data to be stored in database
  Firebase.setFloatDigits(2);
  Firebase.setDoubleDigits(6);
}

void loop() {
  float temperature = dht.readTemperature();
  float temperatureF = dht.readTemperature(true);
  float humidity = dht.readHumidity();
  
  // READ DHT11 DATA
  if (Firebase.set(firebaseData, "temperature", temperature)) {
    Serial.println("DOne");
  }
  if (Firebase.set(firebaseData, "temperatureF", temperatureF)) {
    Serial.println("DOne");
  }
  if (Firebase.set(firebaseData, "humidity", humidity)) {
    Serial.println("DOne");
  }
  delay(500);
}
