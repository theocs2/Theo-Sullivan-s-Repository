/*
Below is a personal project I worked on during the 2024-2025 school year. I utilized an Arduino One, ESP 32 Module, KY-026 Flame Sensor, and made use of several 
C++ libraries to give status updates to the user on their winter fire's status.
*/
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

#define FLAME_SENSOR_PIN 4   // Digital pin connected to the flame sensor
#define LED_PIN 2            // LED to indicate flame detection

//WiFi Information
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Telegram Bot details
#define BOT_TOKEN "YOUR_TELEGRAM_BOT_TOKEN"
#define CHAT_ID "YOUR_CHAT_ID"   // Telegram's chat ID

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

bool flameDetected = false;
unsigned long lastMsgTime = 0;
const unsigned long notifyInterval = 60000; // 1 minute between notifications

void setup() {
  Serial.begin(115200);
  pinMode(FLAME_SENSOR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Connect to Wi-Fi
  Serial.println("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Initialize secure client for Telegram
  secured_client.setInsecure();  

  bot.sendMessage(CHAT_ID, "Flame detection system initialized and online.", "");
}

void loop() {
  int sensorValue = digitalRead(FLAME_SENSOR_PIN);

  if (sensorValue == LOW) {  // LOW = Flame detected (depends on sensor)
    if (!flameDetected || millis() - lastMsgTime > notifyInterval) {
      flameDetected = true;
      lastMsgTime = millis();

      Serial.println("Flame detected!");
      digitalWrite(LED_PIN, HIGH);

      String message = "*Flame detected!* at your Arduino sensor.\n";
      message += "Time: " + String(millis() / 1000) + "s since startup.";
      bot.sendMessage(CHAT_ID, message, "Markdown");
    }
  } else {
    flameDetected = false;
    digitalWrite(LED_PIN, LOW);
  }

  delay(200); // debounce delay
}
