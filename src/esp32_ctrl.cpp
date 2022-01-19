#include "esp32_ctrl.h"

#if __has_include("myconfig.h")
    #include "myconfig.h"
#else
    #include "myconfig.example.h"
#endif

long StartTime = 0;
String PrefSSID, PrefPassword;

void ESP32CtrlStart()
{
  StartTime = millis();
  pinMode(ALT_BUTTON, INPUT);
  pinMode(LED, OUTPUT);
}

//Function that prints the reason by which ESP32 has been awaken from sleep
void print_wakeup_reason()
{
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  switch (wakeup_reason)
  {
  case 1:
    Serial.println("Wakeup caused by external signal using RTC_IO");
    break;
  case 2:
    Serial.println("Wakeup caused by external signal using RTC_CNTL");
    break;
  case 3:
    Serial.println("Wakeup caused by timer");
    break;
  case 4:
    Serial.println("Wakeup caused by touchpad");
    break;
  case 5:
    Serial.println("Wakeup caused by ULP program");
    break;
  default:
    Serial.println("Wakeup was not caused by deep sleep");
    break;
  }
}

//#########################################################################################
void BeginSleep(bool notimer, int sleepTime)
{
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  btStop();
  adc_power_off();
  esp_wifi_stop();
  esp_bt_controller_disable();
  long SleepTimer = sleepTime * 60; //Some ESP32 are too fast to maintain accurate time
  // if ( (NightSleepStart < NightSleepEnd && CurrentHour > NightSleepStart && CurrentHour < NightSleepEnd) ||
  //   (NightSleepStart > NightSleepEnd && (CurrentHour > NightSleepStart || CurrentHour < NightSleepEnd)) ) {
  //   // good night
  //   SleepTimer += (NightSleepEnd - CurrentHour + (CurrentHour > NightSleepEnd ? 24 : 0))  * 3600;
  // }

  esp_sleep_enable_ext0_wakeup(ALT_BUTTON, LOW);

  if (!notimer)
  {
    esp_sleep_enable_timer_wakeup((SleepTimer + 20) * 1000000LL); // Added 20-sec extra delay to cater for slow ESP32 RTC timers
  }
  else
  {
    Serial.println("Will not wake up on timer");
  }
#ifdef BUILTIN_LED
  pinMode(BUILTIN_LED, INPUT); // If it's On, turn it off and some boards use GPIO-5 for SPI-SS, which remains low after screen use
  digitalWrite(BUILTIN_LED, HIGH);
#endif
  Serial.println("Entering " + String(SleepTimer) + "-secs of sleep time");
  Serial.println("Awake for : " + String((millis() - StartTime) / 1000.0, 3) + "-secs");
  Serial.println("Starting deep-sleep period...");
  esp_deep_sleep_start(); // Sleep for e.g. 30 minutes
}

//#########################################################################################
// SSID storage
Preferences preferences; // declare class object
// END SSID storage
bool WiFiSmartConfig()
{
  preferences.begin("wifi", false);
  PrefSSID = preferences.getString("ssid", "none");         // NVS key ssid
  PrefPassword = preferences.getString("password", "none"); // NVS key password
  preferences.end();

  if (!(PrefSSID == "none" || PrefPassword == "none"))
  {
    if (digitalRead(ALT_BUTTON) == LOW)
    {
      Serial.println("Reset smart config");
      preferences.begin("wifi", false);
      preferences.clear();
      preferences.end();
      PrefSSID = "none";
    }
    else
    {
      return true;
    }
  }

  Serial.printf("Start smart config");
  WiFi.beginSmartConfig();
  unsigned long start = millis();
  while ((millis() < start + 60000))
  { // 1m
    Serial.print(".");
    digitalWrite(LED, LOW);
    delay(250);
    digitalWrite(LED, HIGH);
    delay(250);
    if (WiFi.smartConfigDone())
    {
      PrefSSID = WiFi.SSID();
      PrefPassword = WiFi.psk();
      Serial.printf("WiFi config: %s %s\n", PrefSSID, PrefPassword);
      preferences.begin("wifi", false);
      preferences.putString("ssid", PrefSSID);         // NVS key ssid
      preferences.putString("password", PrefPassword); // NVS key password
      preferences.end();
      for (int i = 0; i < 3; i++)
      { // blink x3
        digitalWrite(LED, LOW);
        delay(100);
        digitalWrite(LED, HIGH);
        delay(100);
      }
      return true;
    }
  }
  return false;
}

uint8_t StartWiFi()
{
  WiFi.disconnect();
  WiFi.mode(WIFI_STA); // switch off AP

  if (!WiFiSmartConfig())
  {
    return WL_NO_SSID_AVAIL;
  }
  #ifdef useStaticIP
  WiFi.config(ip, gateway, subnet);
  #endif
  //IPAddress dns(8, 8, 8, 8); // Google DNS
  WiFi.setAutoConnect(false);
  WiFi.setAutoReconnect(false);

  Serial.print("Connecting WiFI ");
  WiFi.begin(PrefSSID.c_str(), PrefPassword.c_str());
  unsigned long start = millis();
  uint8_t connectionStatus;
  bool AttemptConnection = true;
  while (AttemptConnection)
  {
    //connectionStatus = wifiMulti.run(); //5s by default
    connectionStatus = WiFi.status();
    if (millis() > start + 15000)
    { // Wait 15-secs maximum
      AttemptConnection = false;
    }
    if (connectionStatus == WL_CONNECTED || connectionStatus == WL_CONNECT_FAILED)
    {
      AttemptConnection = false;
    }
    delay(50);
  }
  if (connectionStatus == WL_CONNECTED)
  {
    int wifi_signal = WiFi.RSSI(); // Get Wifi Signal strength now, because the WiFi will be turned off to save power!
    Serial.printf("WiFi connected at: %s %s %ddBm\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str(), wifi_signal);
  }
  else
    Serial.println("WiFi connection *** FAILED ***");
  return connectionStatus;
}
//#########################################################################################
void StopWiFi()
{
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
}

void SetupOTA()
{
  // Start OTA once connected
  Serial.println("Setting up OTA");
  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);
  // Hostname defaults to esp3232-[MAC]
  ArduinoOTA.setHostname(otaName);
  // No authentication by default
  if (strlen(otaPassword) != 0)
  {
    ArduinoOTA.setPassword(otaPassword);
    Serial.printf("OTA Password: %s\n\r", otaPassword);
  }
  else
  {
    Serial.printf("\r\nNo OTA password has been set! (insecure)\r\n\r\n");
  }
  ArduinoOTA
      .onStart([]()
               {
                 String type;
                 if (ArduinoOTA.getCommand() == U_FLASH)
                   type = "sketch";
                 else // U_SPIFFS
                   type = "filesystem";
                 // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
                 Serial.println("Start updating " + type);
               })
      .onEnd([]()
             { Serial.println("\r\nEnd"); })
      .onProgress([](unsigned int progress, unsigned int total)
                  { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); })
      .onError([](ota_error_t error)
               {
                 Serial.printf("Error[%u]: ", error);
                 if (error == OTA_AUTH_ERROR)
                   Serial.println("Auth Failed");
                 else if (error == OTA_BEGIN_ERROR)
                   Serial.println("Begin Failed");
                 else if (error == OTA_CONNECT_ERROR)
                   Serial.println("Connect Failed");
                 else if (error == OTA_RECEIVE_ERROR)
                   Serial.println("Receive Failed");
                 else if (error == OTA_END_ERROR)
                   Serial.println("End Failed");
               });
  ArduinoOTA.begin();
}

void HandleOTA()
{
  ArduinoOTA.handle();
}