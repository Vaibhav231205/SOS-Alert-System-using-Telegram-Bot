#include <WiFi.h>
#include <TinyGPS++.h>
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>

const char* ssid = "Vaibhav";
const char* password = "12345678";

#define BOT_TOKEN "8377483272:AAGYPDoZZUVeB9WrtEw180D5Sp6-64Fol6s"
#define CHAT_ID "5678854226"

WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);

TinyGPSPlus gps;

HardwareSerial gpsSerial(2);

#define BUTTON 4

void setup() {
  Serial.begin(115200);
  gpsSerial.begin(9600, SERIAL_8N1, 16, 17);

  pinMode(BUTTON, INPUT_PULLUP);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }

  client.setInsecure(); // Important
  Serial.println("Connected to WiFi");
}

void loop() {

  while (gpsSerial.available()) {
    gps.encode(gpsSerial.read());
  }

  if (digitalRead(BUTTON) == LOW) {
    if (gps.location.isValid()) {

      float lat = gps.location.lat();
      float lon = gps.location.lng();

      String message = "🚨 Emergency Location:\n";
      message += "https://maps.google.com/?q=";
      message += String(lat, 6) + "," + String(lon, 6);

      bot.sendMessage(CHAT_ID, message, "");

      delay(5000); // avoid spam
    }
  }
}
