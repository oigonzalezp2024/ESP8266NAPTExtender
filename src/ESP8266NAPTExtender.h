#ifndef ESP8266NAPTExtender_H
#define ESP8266NAPTExtender_H

#include <ESP8266WiFi.h>
#include <lwip/napt.h>
#include <lwip/dns.h>

// Definiciones para NAPT
#define NAPT_MAX_CONNECTIONS 1000
#define NAPT_PORT_POOL_SIZE 10

class ESP8266NAPTExtender {
public:
    /**
     * @brief Inicializa y configura el extensor de rango NAPT.
     * * Conecta al WiFi STA, configura el SoftAP, habilita el DHCP con el DNS 
     * real y activa el NAT para compartir la conexión.
     * * @param sta_ssid El SSID de la red a la que se conectará (STA).
     * @param sta_psk La contraseña de la red STA.
     * @param ap_ssid_suffix Sufijo para el SSID del Access Point (por defecto "_ext"). 
     * El AP SSID final será <sta_ssid><ap_ssid_suffix>.
     * @param ap_psk La contraseña del Access Point (si es nulo, usará sta_psk).
     * @return true si la configuración y el NAPT se inicializaron correctamente, false en caso contrario.
     */
    bool begin(const char* sta_ssid, const char* sta_psk, 
               const char* ap_ssid_suffix = "_extender", const char* ap_psk = nullptr);

    /**
     * @brief Función dummy para cumplir con el loop de Arduino. 
     * En este caso, no hace nada, ya que NAPT funciona en segundo plano.
     */
    void loop() {}

    /**
     * @brief Obtiene la IP local del dispositivo en modo STA.
     * @return La dirección IP del STA.
     */
    IPAddress getSTAIP() {
        return WiFi.localIP();
    }
    
    /**
     * @brief Obtiene la IP del SoftAP.
     * @return La dirección IP del SoftAP.
     */
    IPAddress getAPIP() {
        return WiFi.softAPIP();
    }

private:
    // Puedes poner aquí cualquier variable o método interno que necesites
};

#endif // ESP8266NAPTExtender_H
