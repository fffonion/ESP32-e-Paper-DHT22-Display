#ifndef CONFIG_H_
#define CONFIG_H_

#define otaName "ESP32-DHT"

#define otaPassword "otapassword" // OTA password

// #define useStaticIP // uncomment to use static IP instead of DHCP

IPAddress ip(10, 0, 0, 71);
IPAddress gateway(10, 0, 0, 1);
IPAddress subnet(255, 255, 255, 0);

#endif