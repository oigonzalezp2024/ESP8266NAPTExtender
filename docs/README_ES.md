# ESP8266NAPTExtender 🚀

**ESP8266: Extensor NAT dual (STA/SoftAP) con NAPT simplificado.**

---
### ⚠️ Nota Importante: Dependencia de LwIP
Esta librería requiere que la configuración del core ESP8266 Arduino habilite la característica **LwIP NAPT**. Consulta las opciones de la placa en el IDE de Arduino (o la configuración de PlatformIO) para asegurar la compatibilidad.
---

<a name="release"></a>
[![Release](https://img.shields.io/github/v/release/oigonzalezp2024/ESP8266NAPTExtender?include_prereleases)](https://github.com/oigonzalezp2024/ESP8266NAPTExtender/releases)
[![GitHub license](https://img.shields.io/github/license/oigonzalezp2024/ESP8266NAPTExtender)](https://github.com/oigonzalezp2024/ESP8266NAPTExtender/blob/main/LICENSE)
[![GitHub issues](https://img.shields.io/github/issues/oigonzalezp2024/ESP8266NAPTExtender)](https://github.com/oigonzalezp2024/ESP8266NAPTExtender/issues)

[![ESP8266](https://img.shields.io/badge/ESP-8266-000000.svg?longCache=true&style=flat&colorA=CC101F)](https://www.espressif.com/en/products/socs/esp8266)
[![PlatformIO Registry](https://img.shields.io/badge/PlatformIO-Library-orange?longCache=true&style=flat)](https://platformio.org/lib/show/YOUR_LIB_ID_HERE/ESP8266NAPTExtender)

---

## Contenido
- [Visión General](#visión-general)
- [Cómo Funciona el NAPT](#cómo-funciona-el-napt)
- [Inicio Rápido](#inicio-rápido)
  - [Instalación](#instalación)
  - [Uso Básico](#uso-básico)
- [Documentación de la API](#documentación-de-la-api)
- [Contribuciones](#contribuciones)

---

## Visión General

Esta librería simplifica el proceso de configurar un módulo **ESP8266** para funcionar como un **Extensor de Rango (Range Extender)** completo y funcional utilizando **Network Address Translation (NAPT)**, una característica avanzada del *core* LwIP del ESP8266.

El módulo se conecta a una red principal (modo **Station - STA**) y simultáneamente crea su propio **Punto de Acceso (SoftAP)**, compartiendo la conexión a Internet a través del NAPT.

### ¿Por qué NAPT?

Sin NAPT, los dispositivos conectados al SoftAP no podrían acceder a Internet o a la red STA principal debido a conflictos de direcciones y falta de enrutamiento. **NAPT** resuelve esto, traduciendo las direcciones IP del SoftAP a la IP del STA, permitiendo que el tráfico fluya de manera transparente (similar a cómo funciona un router doméstico).

---

## Cómo Funciona el NAPT

El flujo de trabajo implementado por la función `begin()` es el siguiente:

1.  **Conexión STA:** El ESP8266 intenta conectarse a la red WiFi principal (`sta_ssid`).
2.  **Configuración del SoftAP:** Si la conexión STA es exitosa, se configura y levanta el Access Point secundario.
3.  **DHCP y DNS:** Se habilita el servidor DHCP del SoftAP y se configura para usar el **DNS obtenido de la red STA** (garantizando el acceso a Internet por nombre de dominio).
4.  **Inicialización y Habilitación del NAPT:**
    * Se inicializa la capa NAPT de LwIP.
    * Se habilita la traducción de direcciones en la interfaz del **SoftAP** (`SOFTAP_IF`), permitiendo que los paquetes de la red local (SoftAP) se enruten a través de la interfaz **STA** utilizando NAT.



---

## Inicio Rápido

### Instalación

La forma recomendada es a través del Gestor de Librerías de Arduino o PlatformIO.

#### Instalación en Arduino (Próximamente)

1.  En el IDE de Arduino, ve a **Sketch > Include Library > Manage Libraries...**
2.  Busca `ESP8266NAPTExtender` y haz clic en **Instalar**.

#### Instalación desde GitHub

1.  Descarga o clona este repositorio en tu carpeta de librerías de Arduino (`.../Arduino/libraries/`).
2.  Asegúrate de que la carpeta se llame **`ESP8266NAPTExtender`**.

### Uso Básico

El uso es simple: solo necesitas inicializar la clase y llamar a `begin()` en `setup()` con las credenciales de la red principal.

```cpp
#include <ESP8266NAPTExtender.h>

// Credenciales de la red principal (a la que se conecta el ESP)
#define WIFI_SSID "TuRedPrincipal"
#define WIFI_PASS "TuContraseña"

// La red que creará el ESP se llamará: "TuRedPrincipal_extender"

ESP8266NAPTExtender extender;

void setup() {
    Serial.begin(115200);
    Serial.println("\n\nIniciando NAPT Extender...");
    
    // El método begin() realiza toda la configuración y activación de NAPT
    if (extender.begin(WIFI_SSID, WIFI_PASS)) {
        Serial.println("Extender de rango NAPT activo.");
        Serial.printf("STA IP (WAN): %s\n", extender.getSTAIP().toString().c_str());
        Serial.printf("AP IP (LAN): %s\n", extender.getAPIP().toString().c_str());
    } else {
        Serial.println("Fallo al inicializar NAPT Extender.");
    }
}

void loop() {
    // La función loop no requiere implementación activa, NAPT opera en segundo plano.
    extender.loop(); 
    delay(10);
}
````

-----

## Documentación de la API

La clase principal es **`ESP8266NAPTExtender`**.

### `begin()`

| Declaración | `bool begin(const char* sta_ssid, const char* sta_psk, const char* ap_ssid_suffix = "_extender", const char* ap_psk = nullptr)` |
| :--- | :--- |
| **Descripción** | Conecta la STA, configura SoftAP, activa DHCP/DNS y habilita NAPT. |
| **`sta_ssid`** | SSID de la red a la que se conecta el ESP. |
| **`sta_psk`** | Contraseña de la red STA. |
| **`ap_ssid_suffix`** | *(Opcional)* Sufijo para el SSID del Access Point. Por defecto: `_extender`. |
| **`ap_psk`** | *(Opcional)* Contraseña del SoftAP. Si se omite (`nullptr`), usa `sta_psk`. |
| **Retorno** | `true` si la conexión STA y la activación de NAPT son exitosas. |

### Métodos de Estado

| Método | Declaración | Descripción |
| :--- | :--- | :--- |
| **`getSTAIP()`** | `IPAddress getSTAIP()` | Devuelve la IP obtenida del router principal (STA). |
| **`getAPIP()`** | `IPAddress getAPIP()` | Devuelve la IP del SoftAP (por defecto: `172.217.28.254` en el ejemplo). |
| **`loop()`** | `void loop()` | Función dummy. Llamada necesaria en `loop()` de Arduino. |

-----

## Contribuciones

Este proyecto fue desarrollado por **Oscar Gonzalez**. ¡Las contribuciones, reportes de *bugs* y sugerencias son bienvenidas\!