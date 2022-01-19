#ifndef ESP32_CTRL_H_
#define ESP32_CTRL_H_

#include <Preferences.h> // WiFi storage
#include "WiFi.h"
#include "driver/adc.h"
#include <esp_wifi.h>
#include <esp_bt.h>
#include <ArduinoOTA.h>

#define LED GPIO_NUM_19
#define ALT_BUTTON GPIO_NUM_39

void ESP32CtrlStart();
void BeginSleep(bool notimer, int sleepTime);
void print_wakeup_reason();

extern String PrefSSID, PrefPassword;

bool WiFiSmartConfig();
uint8_t StartWiFi();
void StopWiFi();

void SetupOTA();
void HandleOTA();

#endif