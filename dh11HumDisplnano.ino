#include <SimpleDHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2); // set the LCD address to 0x27 for a 16 chars and 2 line display

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
int pinDHT11 = 2;
SimpleDHT11 dht11;

String g_strLine1;
String g_strLine2;
char buff1[32];
char buff2[32];
int prevTemp;
int prevHum;

void setup() {
  lcd.init();
  // Print a message to the LCD.
  
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("=================================");
  Serial.println("Sample DHT11...");
  
  // read with raw sample data.
  byte temperature = 0;
  byte humidity = 0;
  byte data[40] = {0};
  if (dht11.read(pinDHT11, &temperature, &humidity, data)) {
    Serial.print("Read DHT11 failed\n");
    delay(1000);
    return;
  }
  int currTemp = (int)temperature;
  int currHumidity = (int)humidity;
  
  Serial.print("Read OK, Sample RAW Bits: ");
  for (int i = 0; i < 40; i++) {
    Serial.print((int)data[i]);
    if (i > 0 && ((i + 1) % 4) == 0) {
      Serial.print(' ');
    }
  }
  Serial.println("");
  
  Serial.print("Sample OK: ");
  snprintf(buff1, sizeof(buff1), "%d C Temperature", currTemp);
  g_strLine1 = buff1;
  snprintf(buff2, sizeof(buff2), "%d %% Humidity", currHumidity);
  g_strLine2 = buff2;
  
  Serial.print(currTemp); Serial.print(" *C, ");
  Serial.print(currHumidity); Serial.println(" %");
  if (prevTemp != currTemp || prevHum != currHumidity){
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print(g_strLine1);
    lcd.setCursor(0,1);
    lcd.print(g_strLine2);
    prevTemp = currTemp;
    prevHum = currHumidity;
  }else{
    lcd.noBacklight();
    Serial.println("No backlight");
  }
 
  // DHT11 sampling rate is 1HZ.
  delay(3000);

}
