// ---- Config ----
#define IO_USERNAME  "YOUR_AIO_USERNAME"
#define IO_KEY       "YOUR_AIO_KEY"
#define WIFI_SSID    "YOUR_WIFI_NAME"
#define WIFI_PASS    "YOUR_WIFI_PASSWORD"

#include "AdafruitIO_WiFi.h"
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

// Feed "LED" = MQTT topic IO_USERNAME/feeds/led
AdafruitIO_Feed *ledFeed = io.feed("LED");

const int RELAY_PIN = 26;     // relay IN pin
const int LED_PIN   = 2;      // onboard LED

// Called every time a new value arrives on the feed
void handleRelay(AdafruitIO_Data *data) {
  int value = data->toInt();
  Serial.print("Adafruit IO value: ");
  Serial.println(value);

  if (value == 1) {
    digitalWrite(RELAY_PIN, HIGH);
    digitalWrite(LED_PIN, HIGH);
    Serial.println("Relay ON -> Bulb ON");
  } else {
    digitalWrite(RELAY_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
    Serial.println("Relay OFF -> Bulb OFF");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);   // start with bulb OFF

  Serial.print("Connecting to Adafruit IO");
  io.connect();                              // Wi-Fi + MQTT connection
  ledFeed->onMessage(handleRelay);           // subscribe + register callback
  while (io.status() < AIO_CONNECTED) { Serial.print("."); delay(500); }
  Serial.println();
  Serial.println(io.statusText());
  ledFeed->get();                            // ask for the last stored value
}

void loop() {
  io.run();   // keeps MQTT alive and processes incoming messages
}
