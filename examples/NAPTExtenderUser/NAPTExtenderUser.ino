#include <ESP8266NAPTExtender.h>

#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASS "YOUR_WIFI_PASS"

ESP8266NAPTExtender extender;

void setup() {
    Serial.begin(115200);
    Serial.println("\n\nIniciando NAPT Extender...");
    if (extender.begin(WIFI_SSID, WIFI_PASS)) {
        Serial.println("Extender de rango NAPT activo.");
        Serial.printf("STA IP: %s\n", extender.getSTAIP().toString().c_str());
        Serial.printf("AP IP: %s\n", extender.getAPIP().toString().c_str());
    } else {
        Serial.println("Fallo al inicializar NAPT Extender.");
    }
}

void loop() {
    extender.loop();
    delay(10);
}
