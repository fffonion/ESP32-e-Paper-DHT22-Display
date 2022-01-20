#ifndef CONFIG_H_
#define CONFIG_H_

#define otaName "ESP32-DHT"

#define otaPassword "otapassword" // OTA password

#define statsdTag "myhome"

// #define useStaticIP // uncomment to use static IP instead of DHCP

#define ip IPAddress(10, 0, 0, 71)
#define gateway IPAddress(10, 0, 0, 1)
#define subnet IPAddress(255, 255, 255, 0)

#endif