#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>

// CREDENCIALES DE WIFI ( RED Y CONTRASEÑA )
#define WIFI_SSID "iPhone"
#define WIFI_PASSWORD "juanse0808"

// Telegram BOT Token obtenido de (Botfather)
#define BOT_TOKEN "6755160414:AAE-DD6BZbgtDH-mbuzXXrX8dsmNbafNdiw"

// @myidbot (IDBot) para encontrar el ID del chat
#define CHAT_ID "6684546714"

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

int baseHallValue;
const int thresholdOffset = 100; // Puede ser ajustado según sea necesario 

void setup() {
  Serial.begin(115200);
  Serial.println();

  // Inicia la conexión Wi-Fi:
  Serial.print("Conectando a la red Wi-Fi ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Agrega el certificado raíz para api.telegram.org
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // Muestra la direccion IP
  
  Serial.print("\nConexión Wi-Fi establecida. Dirección IP: ");
  Serial.println(WiFi.localIP());

  // Toma la lectura inicial del sensor de efecto Hall para establecer el valor base
  baseHallValue = hallRead();
  Serial.print("Valor base del sensor Hall: ");
  Serial.println(baseHallValue);

  Serial.println("Inicialización completada. Esperando detección del sensor Hall...");
}

void loop() {
  static unsigned long lastAlertTime = 0;
  const unsigned long alertInterval = 5000; // Intervalo de 5 segundos entre alertas

  // Lee el valor del sensor de efecto Hall
  int hallValue = hallRead();

  // Verifica si el valor leído supera el punto de cambio significativo
  if (hallValue > baseHallValue + thresholdOffset) {
    unsigned long currentTime = millis();
    // Verifica si han pasado al menos 5 segundos desde la última alerta 
    if (currentTime - lastAlertTime >= alertInterval) {
      // Si se detecta un campo magnético, envía un mensaje a través de Telegram
      bot.sendMessage(CHAT_ID, "¡Puerta ABIERTA!", "");
      Serial.println("¡Puerta ABIERTA! Se ha enviado una alerta a Telegram.");
      lastAlertTime = currentTime; // Actualiza el tiempo de la última alerta
    }
  }

  // Espera antes de volver a verificar el sensor
  delay(100);
}




