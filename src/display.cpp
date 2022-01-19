#include <display.h>

// #define DRAW_GRID 1   //Help debug layout changes
#define SCREEN_WIDTH 250
#define SCREEN_HEIGHT 122

enum alignmentType
{
  LEFT,
  RIGHT,
  CENTER
};

// Connections for Lilygo TTGO T5 V2.3_2.13 from
// https://github.com/lewisxhe/TTGO-EPaper-Series#board-pins
static const uint8_t EPD_BUSY = 4;
static const uint8_t EPD_CS = 5;
static const uint8_t EPD_RST = 16;
static const uint8_t EPD_DC = 17;   //Data/Command
static const uint8_t EPD_SCK = 18;  //CLK on pinout?
static const uint8_t EPD_MISO = -1; // Master-In Slave-Out not used, as no data from display
static const uint8_t EPD_MOSI = 23;

GxEPD2_BW<GxEPD2_213_B73, GxEPD2_213_B73::HEIGHT> display(GxEPD2_213_B73(/*CS=D8*/ EPD_CS, /*DC=D3*/ EPD_DC, /*RST=D4*/ EPD_RST, /*BUSY=D2*/ EPD_BUSY));
U8G2_FOR_ADAFRUIT_GFX u8g2Fonts; // Select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall

RTC_DATA_ATTR int bootCount = 0;

void InitialiseDisplay()
{
  bootCount = bootCount % 20; // full update after 20 partial updates
  Serial.println("Begin InitialiseDisplay ...");
  display.init(115200, bootCount++ == 0, 2, false);
  Serial.printf("boot count %d\n", bootCount);
  // display.init(); for older Waveshare HAT's
  SPI.end();
  SPI.begin(EPD_SCK, EPD_MISO, EPD_MOSI, EPD_CS);
  display.setRotation(3);                    // Use 1 or 3 for landscape modes
  u8g2Fonts.begin(display);                  // connect u8g2 procedures to Adafruit GFX
  u8g2Fonts.setFontMode(1);                  // use u8g2 transparent mode (this is default)
  u8g2Fonts.setFontDirection(0);             // left to right (this is default)
  u8g2Fonts.setForegroundColor(GxEPD_BLACK); // apply Adafruit GFX color
  u8g2Fonts.setBackgroundColor(GxEPD_WHITE); // apply Adafruit GFX color
  // u8g2Fonts.setFont(u8g2_font_helvB10_tf);   // Explore u8g2 fonts from here: https://github.com/olikraus/u8g2/wiki/fntlistall
  //display.fillScreen(GxEPD_WHITE);
  display.setFullWindow();
  Serial.println("... End InitialiseDisplay");
}
//#########################################################################################
void drawString(int x, int y, String text, alignmentType alignment)
{
  int16_t x1, y1; //the bounds of x,y and w and h of the variable 'text' in pixels.
  uint16_t w, h;
  display.setTextWrap(false);
  display.getTextBounds(text, x, y, &x1, &y1, &w, &h);
  if (alignment == RIGHT)
    x = x - w;
  if (alignment == CENTER)
    x = x - w / 2;
  u8g2Fonts.setCursor(x, y + h);
  u8g2Fonts.print(text);
}
//#########################################################################################
void DrawBattery(int x, int y)
{
  uint8_t percentage = 100;
  float voltage = analogRead(35) / 4096.0 * 7.46;
  if (voltage > 1)
  { // Only display if there is a valid reading
    Serial.println("Voltage = " + String(voltage));
    percentage = 2836.9625 * pow(voltage, 4) - 43987.4889 * pow(voltage, 3) + 255233.8134 * pow(voltage, 2) - 656689.7123 * voltage + 632041.7303;
    if (voltage >= 4.20)
      percentage = 100;
    if (voltage <= 3.50)
      percentage = 0;
    display.drawRect(x + 15, y - 12, 19, 10, GxEPD_BLACK);
    display.fillRect(x + 34, y - 10, 2, 5, GxEPD_BLACK);
    display.fillRect(x + 17, y - 10, 15 * percentage / 100.0, 6, GxEPD_BLACK);
    drawString(x + 60, y - 11, String(percentage) + "%", RIGHT);
    //drawString(x + 13, y + 5,  String(voltage, 2) + "v", CENTER);
  }
}
//#########################################################################################
void DrawRSSI(int x, int y, int rssi)
{
  int WIFIsignal = 0;
  int xpos = 1;
  for (int _rssi = -100; _rssi <= rssi; _rssi = _rssi + 20)
  {
    if (_rssi <= -20)
      WIFIsignal = 20; //            <-20dbm displays 5-bars
    if (_rssi <= -40)
      WIFIsignal = 16; //  -40dbm to  -21dbm displays 4-bars
    if (_rssi <= -60)
      WIFIsignal = 12; //  -60dbm to  -41dbm displays 3-bars
    if (_rssi <= -80)
      WIFIsignal = 8; //  -80dbm to  -61dbm displays 2-bars
    if (_rssi <= -100)
      WIFIsignal = 4; // -100dbm to  -81dbm displays 1-bar
    display.fillRect(x + xpos * 5, y - WIFIsignal, 4, WIFIsignal, GxEPD_BLACK);
    xpos++;
  }
  display.fillRect(x, y - 1, 4, 1, GxEPD_BLACK);
  //drawString(x + 5,  y + 5, String(rssi) + "dBm", CENTER);
}

void DrawMainSection(float t, float h)
{

  display.setPartialWindow(0, 0, 130, SCREEN_HEIGHT);

  display.firstPage();
  do
  {
    display.fillScreen(GxEPD_WHITE);

    u8g2Fonts.setFont(u8g2_font_helvB08_tf);
    //display.drawRect(0,0,SCREEN_WIDTH,SCREEN_HEIGHT,GxEPD_BLACK);
    // - drawString(0, 2, time_str, LEFT);
    // u8g2Fonts.setFont(u8g2_font_5x7_tf);
    // drawString(95, 1, cityName, LEFT);
    // - u8g2Fonts.setFont(u8g2_font_wqy12_t_gb2312);
    // String city_and_date = String(cityName) + " " + date_str;
    // drawString(SCREEN_WIDTH, 2, city_and_date, RIGHT);
    // - drawString(SCREEN_WIDTH+5, 2, date_str, RIGHT);
    // drawString(SCREEN_WIDTH, 1, date_str, RIGHT);
    display.drawLine(0, 18, 130, 18, GxEPD_BLACK);

    DrawBattery(-10, 16);

    String tt = isnan(t) ? String("?") : (String(t, 1) + "°");
    String hh = isnan(h) ? String("?") : String(h, 1);

    u8g2Fonts.setFont(u8g2_font_streamline_weather_t);
    drawString(5, 55, String(char(48 + 6)), LEFT);
    u8g2Fonts.setFont(u8g2_font_logisoso34_tf);
    drawString(35, 60, tt, LEFT);

    display.drawLine(5, 80, 130, 80, GxEPD_BLACK);

    u8g2Fonts.setFont(u8g2_font_streamline_ecology_t);
    drawString(5, 105, String(char(48 + 10)), LEFT);
    u8g2Fonts.setFont(u8g2_font_logisoso20_tf);
    drawString(40, 105, hh, LEFT);
    u8g2Fonts.setFont(u8g2_font_helvB08_tf);
    drawString(95, 95, "%", LEFT);
  } while (display.nextPage());
}

void DrawRSSISection(int rssi)
{
  display.setPartialWindow(110, 0, 25, 18);

  display.firstPage();
  do
  {
    display.drawLine(110, 18, 130, 18, GxEPD_BLACK); // amend the line
    if (rssi == 0)
    {
      u8g2Fonts.setFont(u8g2_font_streamline_internet_network_t);
      drawString(110, 9, String(char(48 + 8)), LEFT); // connect+smile icon
    }
    else
      DrawRSSI(110, 14, rssi);
  } while (display.nextPage());
}

void DrawGraph(int x_pos, int y_pos, int gwidth, int gheight, float Y1Min, float Y1Max, String title, float DataArray[], int readings, boolean auto_scale, boolean barchart_mode)
{
#define auto_scale_margin 0 // Sets the autoscale increment, so axis steps up in units of e.g. 3
#define y_minor_axis 3      // 5 y-axis division markers
  float maxYscale = -10000;
  float minYscale = 10000;
  int last_x, last_y;
  float x1, y1, x2, y2;
  if (auto_scale == true)
  {
    for (int i = 1; i < readings; i++)
    {
      if (DataArray[i] >= maxYscale)
        maxYscale = DataArray[i];
      if (DataArray[i] <= minYscale)
        minYscale = DataArray[i];
    }
    maxYscale = round(maxYscale + auto_scale_margin); // Auto scale the graph and round to the nearest value defined, default was Y1Max
    Y1Max = round(maxYscale + 0.5);
    if (minYscale != 0)
      minYscale = round(minYscale - auto_scale_margin); // Auto scale the graph and round to the nearest value defined, default was Y1Min
    Y1Min = round(minYscale);
  }
  // Draw the graph
  last_x = x_pos + 1;
  last_y = y_pos + (Y1Max - constrain(DataArray[1], Y1Min, Y1Max)) / (Y1Max - Y1Min) * gheight;
  display.drawRect(x_pos, y_pos, gwidth + 3, gheight + 2, GxEPD_BLACK);
  // u8g2Fonts.setFont(u8g2_font_wqy12_t_gb2312);
  u8g2Fonts.setFont(u8g2_font_5x7_tf); // only units
  //drawString(x_pos, y_pos - 12, title, LEFT);
  // put as lower of y, only unit, align to right so longer string can poke to left
  drawString(x_pos + gwidth - 12, y_pos + gheight + 3, title, LEFT);
  u8g2Fonts.setFont(u8g2_font_5x7_tf);
  // Draw the data
  for (int gx = 1; gx < readings; gx++)
  {
    x1 = last_x;
    y1 = last_y;
    x2 = x_pos + gx * gwidth / (readings - 1) - 1; // max_readings is the global variable that sets the maximum data that can be plotted
    y2 = y_pos + (Y1Max - constrain(DataArray[gx], Y1Min, Y1Max)) / (Y1Max - Y1Min) * gheight + 1;
    if (barchart_mode)
    {
      // MOD x-8 to move left, width from + 1 to +2 to shrink
      display.fillRect(x2 - 8, y2, (gwidth / readings) - 1, y_pos + gheight - y2 + 2, GxEPD_BLACK);
    }
    else
    {
      display.drawLine(last_x, last_y, x2, y2, GxEPD_BLACK);
    }
    last_x = x2;
    last_y = y2;
  }
  //Draw the Y-axis scale
#define number_of_dashes 10
  for (int spacing = 0; spacing <= y_minor_axis; spacing++)
  {
    for (int j = 0; j < number_of_dashes; j++)
    { // Draw dashed graph grid lines
      if (spacing < y_minor_axis)
        display.drawFastHLine((x_pos + 3 + j * gwidth / number_of_dashes), y_pos + (gheight * spacing / y_minor_axis), gwidth / (2 * number_of_dashes), GxEPD_BLACK);
    }
    if ((Y1Max - (float)(Y1Max - Y1Min) / y_minor_axis * spacing) < 5)
    {
      drawString(x_pos, y_pos + gheight * spacing / y_minor_axis - 5, String((Y1Max - (float)(Y1Max - Y1Min) / y_minor_axis * spacing + 0.01), 1), RIGHT);
    }
    else
    {
      if (Y1Min < 1 && Y1Max < 10)
        drawString(x_pos - 3, y_pos + gheight * spacing / y_minor_axis - 5, String((Y1Max - (float)(Y1Max - Y1Min) / y_minor_axis * spacing + 0.01), 1), RIGHT);
      else
        drawString(x_pos - 3, y_pos + gheight * spacing / y_minor_axis - 5, String((Y1Max - (float)(Y1Max - Y1Min) / y_minor_axis * spacing + 0.01), 0), RIGHT);
    }
  }
  for (int i = 0; i < 4; i++)
  {
    drawString(x_pos + gwidth / 3 * i, y_pos + gheight + 1,
               String(i == 0 ? "T" : "") + String(i == 3 ? "" : "-") + String(24 - 8 * i), LEFT);
  }
  // drawString(x_pos + gwidth / 2, y_pos + gheight + 10, TXT_DAYS, CENTER);
}

#define max_readings 48
typedef struct
{
  float temperature_readings[max_readings] = {0};
  float humidity_readings[max_readings] = {0};
} history_t;

RTC_DATA_ATTR history_t history;
RTC_DATA_ATTR int intervalSpent = 0;
#define gInterval 30 // 30m per data point
#define gw 80
#define gh 45
Preferences prefs;

void DrawStatistics(float t, float h, int intervalMinutes)
{
  intervalSpent += intervalMinutes;
  if (intervalSpent == intervalMinutes || intervalSpent > gInterval || bootCount == 1)
  {
    intervalSpent = intervalMinutes;
    prefs.begin("main");
    prefs.getBytes("history", &history, sizeof(history));
    // shift left
    for (int i = 0; i < max_readings - 1; i++)
    {
      history.temperature_readings[i] = history.temperature_readings[i + 1];
      history.humidity_readings[i] = history.humidity_readings[i + 1];
    }
    history.temperature_readings[max_readings - 1] = isnan(t) ? 0 : t;
    history.humidity_readings[max_readings - 1] = isnan(h) ? 0 : h;
    prefs.putBytes("history", &history, sizeof(history));

    display.setPartialWindow(130, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    display.firstPage();
    do
    {
      display.fillScreen(GxEPD_WHITE);
      DrawGraph(160, 5, gw, gh, 10, 30, "°C", history.temperature_readings, max_readings, true, false);
      DrawGraph(160, SCREEN_HEIGHT - gh - 10, gw, gh, 0, 100, "%", history.humidity_readings, max_readings, true, false);
    } while (display.nextPage());
  }
}

void Display()
{
  //display.display(false); // Full screen update mode
}

void DisplaySleep()
{
  display.powerOff();
}
