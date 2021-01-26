#include <SimpleDHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

// codice che ho preso dal tutorial lavfin
// che utilizza la libreria simpledgt11. La quale ho installato unsando "Manage Libraries"
// Nel tutorial non sono sicuro che i fili corrispondano al sensore del mio kit.
// Però dopo aver provato, il collegamento come nel file pdf sembra corretto. Mentre l'indicazione sul circuito no.
// 1 - GND
// 2 - Vcc
// 3 - Data

// for DHT11,
//      VCC: 5V or 3V
//      GND: GND
//      DATA: 2
const int pinDHT11 = 2;
const int buttonApin = 9;
SimpleDHT11 dht11;

String g_strLine1;
String g_strLine2;
char buff1[32];
char buff2[32];
int g_prevTemp = 0;
int g_prevHum = 0;
int g_lightloop = 0;
int g_readLoop = 0;
const int g_defDelay = 100;
const int sec_with_light = 5;
const int sec_between_read = 3;
const int turnOffLoop = sec_with_light * 1000 / g_defDelay;
const int readLoop = sec_between_read * 1000 / g_defDelay;

#define DEBUG

void setup()
{
  lcd.init();

  pinMode(buttonApin, INPUT_PULLUP);

#ifdef DEBUG
  Serial.begin(9600);
#endif
}

void loop()
{
  int currTemp = g_prevTemp;
  int currHumidity = g_prevHum;

  if (g_readLoop >= readLoop)
  {
#ifdef DEBUG
    Serial.println("=================================");
    Serial.println("Sample DHT11...");
#endif
    g_readLoop = 0;
    // read with raw sample data.
    byte temperature = 0;
    byte humidity = 0;
    byte data[40] = {0};
    if (dht11.read(pinDHT11, &temperature, &humidity, data))
    {
#ifdef DEBUG
      Serial.print("Read DHT11 failed\n");
#endif
      delay(g_defDelay);
      g_readLoop++;
      return;
    }
    currTemp = (int)temperature;
    currHumidity = (int)humidity;

#ifdef DEBUG
    Serial.print("Read OK, Sample RAW Bits: ");
    for (int i = 0; i < 40; i++)
    {
      Serial.print((int)data[i]);
      if (i > 0 && ((i + 1) % 4) == 0)
      {
        Serial.print(' ');
      }
    }
    Serial.println("");
    Serial.print("Sample OK: ");
    Serial.print(currTemp);
    Serial.print(" *C, ");
    Serial.print(currHumidity);
    Serial.println(" %");
#endif

    snprintf(buff1, sizeof(buff1), "%d C Temperature", currTemp);
    g_strLine1 = buff1;
    snprintf(buff2, sizeof(buff2), "%d %% Humidity", currHumidity);
    g_strLine2 = buff2;
  }
  else
  {
    g_readLoop++;
  }

  if (g_prevTemp != currTemp || g_prevHum != currHumidity)
  {
#ifdef DEBUG
    Serial.println("Tmperature or humidiy has been changed");
#endif
    turnONDisplay();

    lcd.setCursor(0, 0);
    lcd.print(g_strLine1);
    lcd.setCursor(0, 1);
    lcd.print(g_strLine2);
    g_prevTemp = currTemp;
    g_prevHum = currHumidity;
  }
  else
  {
    if (g_lightloop == turnOffLoop)
    {
      turnOFFDisplay();
    }
    else if (g_lightloop < turnOffLoop)
    {
      g_lightloop++;
    }

    if (digitalRead(buttonApin) == LOW)
    {
#ifdef DEBUG
      Serial.println("Button pressed");
#endif
      turnONDisplay();
    }
  }

  // DHT11 sampling rate is 1HZ.
  delay(g_defDelay);
}

void turnONDisplay()
{
  lcd.backlight();
  g_lightloop = 0;
#ifdef DEBUG
  Serial.println("Turn on light");
#endif
}

void turnOFFDisplay()
{
  lcd.noBacklight();
  g_lightloop++;
#ifdef DEBUG
  Serial.print(g_lightloop);
  Serial.print(" turnOffLoop ");
  Serial.print(turnOffLoop);
  Serial.println("Turn off display");
#endif
}
