#ifndef CONFIG_H_
#define CONFIG_H_

#define otaName "ESP32-DHT"

#define otaPassword "otapassword" // OTA password

#define statsdTag "myhome"

// #define useStaticIP // uncomment to use static IP instead of DHCP

#define ip IPAddress(10, 0, 0, 71)
#define gateway IPAddress(10, 0, 0, 1)
#define subnet IPAddress(255, 255, 255, 0)

// #define drawLess // uncomment to draw regardless of night and weekday, need to setup following to get correct time

#define ntpServer "cn.pool.ntp.org" // NTP server
#define gmtOffset 8 // Timezone: GMT+8
#define timezone "CST-8"


#endif