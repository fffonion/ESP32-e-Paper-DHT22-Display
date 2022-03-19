

#include "DHT.h"

#include <WiFi.h> // Built-in
#include <WiFiUdp.h>

#include "esp32_ctrl.h"
#include "display.h"

#if __has_include("myconfig.h")
#include "myconfig.h"
#else
#include "myconfig.example.h"
#endif

#define STATSD "10.0.0.50"

#define DHTPIN GPIO_NUM_0 // what pin we're connected to
#define DHTTYPE DHT22     // DHT 22  (AM2302)
#define PIN3v3 GPIO_NUM_12

RTC_DATA_ATTR bool sendUDP = false;

#define SLEEP_NO_WIFI 5
#define SLEEP_HAS_WIFI 15

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

WiFiUDP Udp;

void blink()
{
  // on
  digitalWrite(LED, HIGH);
  delay(25);
  // off
  digitalWrite(LED, LOW);
  delay(50);
}

void readDHT2Statsd(const char *location, uint8_t tries)
{
  float h, t, f = 0;
  for (uint8_t i = 0; i < tries; i++)
  {
    h = dht.readHumidity(true);
    // Read temperature as Celsius
    t = dht.readTemperature(false);
    if (!isnan(h) && !isnan(t))
      break;
    blink();
    delay(500);
  }

  boolean fail = false;

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t))
  {
    fail = true;
    blink();
    blink();
    blink();
  }
  else
  {
    // Read temperature as Fahrenheit
    f = t * 1.8 + 32;
    //blink();
  }

  DrawMainSection(t, h);
  DrawStatistics(t, h, sendUDP ? SLEEP_HAS_WIFI : SLEEP_NO_WIFI);

  //  Serial.print("Humidity: ");
  //  Serial.print(h);
  //  Serial.print(" %\t");
  //  Serial.print("Temperature: ");
  //  Serial.print(t);
  //  Serial.print(" *C ");
  //  Serial.print(f);
  //  Serial.println(" *F");

  if (!sendUDP)
    return;
  //blink();
  DrawRSSISection(0);

  if (StartWiFi() == WL_CONNECTED)
  {
    int rssi = WiFi.RSSI();
    //blink();
    DrawRSSISection(rssi);
    char buf[200];
    const uint8_t *pbuf = (uint8_t *)buf;
    Udp.beginPacket(STATSD, 9125);
    if (fail)
    {
      sprintf(buf, "env.dht22_read_failure.%s.esp32:1|c\n", location);
      Udp.write(pbuf, strlen(buf));
    }
    else
    {
      sprintf(buf, "env.dht22_read_success.%s.esp32:1|c\n", location);
      Udp.write(pbuf, strlen(buf));
      sprintf(buf, "env.temperature_c.%s.esp32:%.2f|g\n", location, t);
      Udp.write(pbuf, strlen(buf));
      sprintf(buf, "env.temperature_f.%s.esp32:%.2f|g\n", location, f);
      Udp.write(pbuf, strlen(buf));
      sprintf(buf, "env.humidity.%s.esp32:%.2f|g\n", location, h);
      Udp.write(pbuf, strlen(buf));
    }

    BatteryVoltage bv = GetBatteryVoltage(true);
    sprintf(buf, "env.voltage.%s.esp32:%.2f|g\n", location, bv.voltage);
    Udp.write(pbuf, strlen(buf));
    sprintf(buf, "env.rssi.%s.esp32:%d|g\n", location, rssi);
    Udp.write(pbuf, strlen(buf));

    Udp.endPacket();
  }
  else
  {
    blink();
    blink();
    blink();
  }
}

void setup()
{
  setCpuFrequencyMhz(80);
  WiFi.mode(WIFI_OFF);
  btStop();

  Serial.begin(115200);
  ESP32CtrlStart();
  dht.begin();
  InitialiseDisplay();
  pinMode(LED, OUTPUT);

  Serial.println("started");

  if (digitalRead(ALT_BUTTON) == LOW)
  { // is long press
    digitalWrite(LED, HIGH);
    delay(2000);
    digitalWrite(LED, LOW);
    // long press shorter than 2s, setup OTA
    // otherwise we will enter smartconfig reset
    if (digitalRead(ALT_BUTTON) == HIGH)
    {
      DrawRSSISection(0);
      if (StartWiFi() == WL_CONNECTED)
      {
        digitalWrite(LED, HIGH);
        SetupOTA();
        DrawRSSISection(-1);
        return;
      } // otherwise, goto smartconfig reset
    }
  }

  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  if (wakeup_reason == 2)
  {
    sendUDP = !sendUDP;
  }

  pinMode(PIN3v3, OUTPUT);
  digitalWrite(PIN3v3, HIGH);
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  // force = true
  // in deep sleep mode, this needs to be longer
  delay(1500);

  readDHT2Statsd(statsdTag, 3);
  // make sure data is sent
  delay(100);

  digitalWrite(PIN3v3, LOW);
  DisplaySleep();
  BeginSleep(false, sendUDP ? SLEEP_HAS_WIFI : SLEEP_NO_WIFI);
}

void loop()
{
  HandleOTA();
  digitalWrite(LED, LOW);
}
