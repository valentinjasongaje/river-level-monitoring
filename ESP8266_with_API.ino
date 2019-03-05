#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>

const char* ssid = "LTE WIFI";
const char* password = "PasswordGaje";
SoftwareSerial ESP_Serial(3,1);

void setup () {
  ESP_Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    ESP_Serial.write(10);
  }

  ESP_Serial.write(11);
}

void parseJson(String json) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);

  if (!root.success()) {
    ESP_Serial.write(":failed");
  } else {
    ESP_Serial.write(":success");
  }

  JsonObject& data = root["data"];
  const char* data_dateTimeRead = data["dateTimeRead"]; // "2019-02-27 19:15:06+08"
  const char* data_air_pressure = data["air_pressure"]; // "1012.56"
  const char* data_rain_value = data["rain_value"]; // "0.00"
  ESP_Serial.write(":");
  ESP_Serial.write(data_air_pressure);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    HTTPClient http;  //Declare an object of class HTTPClient

    http.begin("http://dost-asti.000webhostapp.com/");  //Specify request destination
    int httpCode = http.GET();

    //Send the request
    //Serial.println(httpCode);
    if (httpCode > 0) { //Check the returning code
      String payload = http.getString();   //Get the request response payload
      //Serial.println(payload);
      parseJson(payload);
    }

    http.end();   //Close connection
  }

  delay(10000);    //Send a request every 10 seconds
}

