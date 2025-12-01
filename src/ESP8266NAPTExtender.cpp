#include "ESP8266NAPTExtender.h"

// Define un rango de IP fijo para el SoftAP para Android-compatibility
// IP: 172.217.28.254, Gateway: 172.217.28.254, Subnet: 255.255.255.0
const IPAddress AP_IP(172, 217, 28, 254);
const IPAddress AP_GW(172, 217, 28, 254);
const IPAddress AP_SN(255, 255, 255, 0);

bool ESP8266NAPTExtender::begin(const char* sta_ssid, const char* sta_psk, 
                                  const char* ap_ssid_suffix, const char* ap_psk) {
#if LWIP_FEATURES && !LWIP_IPV6

    // 1. Conectar a STA
    Serial.printf("\nConectando a STA: %s...\n", sta_ssid);
    WiFi.mode(WIFI_STA);
    WiFi.begin(sta_ssid, sta_psk);
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 40) { // Esperar hasta 20 segundos
        Serial.print('.');
        delay(500);
        attempts++;
    }
    
    if (WiFi.status() != WL_CONNECTED) {
        Serial.printf("\nFallo en la conexión STA.\n");
        return false;
    }
    Serial.printf("\nSTA conectada. IP: %s\n", WiFi.localIP().toString().c_str());

    // 2. Configurar SoftAP DHCP Server para usar el DNS del STA
    // El código original ya maneja la obtención del DNS.
    auto& server = WiFi.softAPDhcpServer();
    IPAddress dns_ip = WiFi.dnsIP(0);
    if (dns_ip.isSet()) {
        server.setDns(dns_ip);
        Serial.printf("SoftAP DNS configurado a: %s\n", dns_ip.toString().c_str());
    } else {
        Serial.printf("Advertencia: No se pudo obtener DNS del STA.\n");
    }

    // 3. Configurar y levantar SoftAP
    String apSSID = String(sta_ssid) + ap_ssid_suffix;
    const char* final_ap_psk = (ap_psk != nullptr) ? ap_psk : sta_psk;
    
    WiFi.softAPConfig(AP_IP, AP_GW, AP_SN);
    WiFi.softAP(apSSID.c_str(), final_ap_psk);
    Serial.printf("AP configurado. SSID: %s, IP: %s\n", apSSID.c_str(), WiFi.softAPIP().toString().c_str());

    // 4. Inicializar NAPT
    err_t ret = ip_napt_init(NAPT_MAX_CONNECTIONS, NAPT_PORT_POOL_SIZE);
    
    if (ret != ERR_OK) {
        Serial.printf("Error: ip_napt_init falló (ret=%d)\n", (int)ret);
        return false;
    }

    // 5. Habilitar NAPT en la interfaz SoftAP
    ret = ip_napt_enable_no(SOFTAP_IF, 1);
    
    if (ret != ERR_OK) {
        Serial.printf("Error: ip_napt_enable_no(SOFTAP_IF) falló (ret=%d)\n", (int)ret);
        return false;
    }

    Serial.printf("NAPT habilitado exitosamente.\n");
    return true;

#else
    Serial.printf("\nNAPT no soportado en la configuración actual de LwIP.\n");
    return false;
#endif
}

// La función loop() de la clase está vacía, ya que NAPT se encarga del routing en segundo plano.
