# ESP8266NAPTExtender 🚀

**ESP8266: Simplified NAPT Dual (STA/SoftAP) NAT Range Extender.**

---
### ⚠️ Important Note: LwIP Dependency
This library requires the ESP8266 Arduino core configuration to enable the **LwIP NAPT** feature. Please check your board settings in the Arduino IDE (or PlatformIO configuration) to ensure compatibility.
---

<a name="release"></a>
[![Release](https://img.shields.io/github/v/release/oigonzalezp2024/ESP8266NAPTExtender?include_prereleases)](https://github.com/oigonzalezp2024/ESP8266NAPTExtender/releases)
[![GitHub license](https://img.shields.io/github/license/oigonzalezp2024/ESP8266NAPTExtender)](https://github.com/oigonzalezp2024/ESP8266NAPTExtender/blob/main/LICENSE)
[![GitHub issues](https://img.shields.io/github/issues/oigonzalezp2024/ESP8266NAPTExtender)](https://github.com/oigonzalezp2024/ESP8266NAPTExtender/issues)

[![ESP8266](https://img.shields.io/badge/ESP-8266-000000.svg?longCache=true&style=flat&colorA=CC101F)](https://www.espressif.com/en/products/socs/esp8266)
[![PlatformIO Registry](https://img.shields.io/badge/PlatformIO-Library-orange?longCache=true&style=flat)](https://platformio.org/lib/show/YOUR_LIB_ID_HERE/ESP8266NAPTExtender)

---

## 📖 Contents
- [Overview](#overview)
- [How NAPT Works](#how-napt-works)
- [Quick Start](#quick-start)
  - [Installation](#installation)
  - [Basic Usage](#basic-usage)
- [API Documentation](#api-documentation)
- [Contributions](#contributions)

---

## Overview

This library streamlines the process of configuring an **ESP8266** module to function as a complete and working **Range Extender** using **Network Address Translation (NAPT)**, an advanced feature within the ESP8266's LwIP core.

The module connects to a primary network (in **Station - STA** mode) and simultaneously creates its own **Access Point (SoftAP)**, sharing the Internet connection via NAPT.

### 🌐 Why NAPT?

Without NAPT, devices connected to the SoftAP would not be able to access the Internet or the main STA network due to address conflicts and lack of routing. **NAPT** solves this by translating the SoftAP's local IP addresses to the STA's IP, allowing traffic to flow transparently (similar to how a home router works).

## How NAPT Works

The workflow implemented by the `begin()` function is as follows:

1.  **STA Connection:** The ESP8266 attempts to connect to the main WiFi network (`sta_ssid`).
2.  **SoftAP Configuration:** If the STA connection succeeds, the secondary Access Point is configured and activated.
3.  **DHCP and DNS:** The SoftAP's DHCP server is enabled and configured to use the **DNS obtained from the STA network** (ensuring Internet access by domain name).
4.  **NAPT Initialization and Enablement:**
    * The NAPT layer of LwIP is initialized.
    * Address translation is enabled on the **SoftAP** interface (`SOFTAP_IF`), allowing packets from the local network (SoftAP) to be routed through the **STA** interface using NAT.



---

## 🚀 Quick Start

### Installation

The recommended way is through the Arduino Library Manager or PlatformIO.

#### 📦 Arduino Installation (Coming Soon)

1.  In the Arduino IDE, go to **Sketch > Include Library > Manage Libraries...**
2.  Search for `ESP8266NAPTExtender` and click **Install**.

#### 🐙 GitHub Checkout

1.  Download or clone this repository into your Arduino libraries folder (`.../Arduino/libraries/`).
2.  Ensure the folder is named **`ESP8266NAPTExtender`**.

### Basic Usage

Usage is simple: you just need to initialize the class and call `begin()` in `setup()` with the primary network credentials.

```cpp
#include <ESP8266NAPTExtender.h>

// Credentials for the main network (the one the ESP connects to)
#define WIFI_SSID "YourMainNetwork"
#define WIFI_PASS "YourPassword"

// The network created by the ESP will be named: "YourMainNetwork_extender"

ESP8266NAPTExtender extender;

void setup() {
    Serial.begin(115200);
    Serial.println("\n\nStarting NAPT Extender...");
    
    // The begin() method performs all configuration and NAPT activation
    if (extender.begin(WIFI_SSID, WIFI_PASS)) {
        Serial.println("NAPT Range Extender is active.");
        Serial.printf("STA IP (WAN): %s\n", extender.getSTAIP().toString().c_str());
        Serial.printf("AP IP (LAN): %s\n", extender.getAPIP().toString().c_str());
    } else {
        Serial.println("Failed to initialize NAPT Extender.");
    }
}

void loop() {
    // The loop function does not require active implementation; NAPT operates in the background.
    extender.loop(); 
    delay(10);
}
````

-----

## API Documentation

The main class is **`ESP8266NAPTExtender`**.

### `begin()`

| Declaration | `bool begin(const char* sta_ssid, const char* sta_psk, const char* ap_ssid_suffix = "_extender", const char* ap_psk = nullptr)` |
| :--- | :--- |
| **Description** | Connects the STA, configures SoftAP, enables DHCP/DNS, and activates NAPT to share the connection. |
| **`sta_ssid`** | SSID of the network the ESP connects to. |
| **`sta_psk`** | Password for the STA network. |
| **`ap_ssid_suffix`** | *(Optional)* Suffix for the Access Point SSID. Default: `_extender`. |
| **`ap_psk`** | *(Optional)* Password for the SoftAP. If omitted (`nullptr`), it uses `sta_psk`. |
| **Returns** | `true` if STA connection and NAPT activation are successful. |

### Status Methods

| Method | Declaration | Description |
| :--- | :--- | :--- |
| **`getSTAIP()`** | `IPAddress getSTAIP()` | Returns the IP address obtained from the main router (STA). |
| **`getAPIP()`** | `IPAddress getAPIP()` | Returns the IP address of the SoftAP (default: `172.217.28.254` in the example). |
| **`loop()`** | `void loop()` | Dummy function. Required call in the Arduino `loop()`. |

-----

## Contributions

This project was developed by **Oscar Gonzalez**. Contributions, bug reports, and suggestions are welcome\!
