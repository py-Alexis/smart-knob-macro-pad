#include <Arduino.h>
#include <string.h>

#include "Wire.h"

#include "MT6701.hpp"

#include "Adafruit_VEML7700.h"

#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>

void blinkSetup();
void blinkLoop();

// needs Wire.h
void i2cScannerSetup();
void i2cScannerLoop();


// needs MT6701 (to import in platform io)
void encoderSetup();
void encoderLoop();
  // Create an MT6701 object with with very short update interval for accurate RPM measurement.
  MT6701 mt6701(MT6701::DEFAULT_ADDRESS, 2);


// needs Adafruit_VEML7700
void lightSetup();
void lightLoop();
Adafruit_VEML7700 veml = Adafruit_VEML7700();

void screenSetup();
void screenLoop();
TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h



void setup() {
  // blinkSetup();

  // i2cScannerSetup();

  // encoderSetup();
  
  // lightSetup();

  screenSetup();
}

void loop() {
  // blinkLoop();

  // i2cScannerLoop();

  // encoderLoop();

  // lightLoop();

  screenLoop();
}


void blinkSetup() {
  pinMode(2, OUTPUT);
}

void blinkLoop() {
  digitalWrite(2, HIGH);
  delay(500);

  digitalWrite(2, LOW);
  delay(500);
}

void i2cScannerSetup() {
  // https://randomnerdtutorials.com/esp32-i2c-scanner-arduino/

  Wire.begin();
  Serial.begin(9600);
  Serial.println("\nI2C Scanner");
}

void i2cScannerLoop() {
    byte error, address;
  int nDevices;
  Serial.println("Scanning...");
  nDevices = 0;
  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
      nDevices++;
    }
    else if (error==4) {
      Serial.print("Unknow error at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  }
  else {
    Serial.println("done\n");
  }
  delay(5000);  
}

void encoderSetup() {
  Serial.begin(9600);
  mt6701.begin();
}

void encoderLoop() {
      // On ESP32, the count is updated in a background task.
    // On other platforms, the count is updated here.
    // Make sure to call updateCount() at least once per revolution.
    // mt6701.updateCount();
    Serial.print("Angle (degrees): ");
    Serial.println(mt6701.getAngleDegrees());
    Serial.print("Angle (radians): ");
    Serial.println(mt6701.getAngleRadians());
    Serial.print("Accumulated count: ");
    Serial.println(mt6701.getAccumulator());
    Serial.print("RPM: ");
    Serial.println(mt6701.getRPM());
    Serial.print("raw count value: ");
    Serial.println(mt6701.getCount());
    Serial.print("turns since startup: ");
    Serial.println(mt6701.getTurns());
    Serial.print("full turns since startup: ");
    Serial.println(mt6701.getFullTurns());
    Serial.println();
    delay(1000);
}

void lightSetup() {
  Serial.begin(9600);
  while (!Serial) { delay(10); }
  Serial.println("Adafruit VEML7700 Auto Lux Test");

  if (!veml.begin()) {
    Serial.println("Sensor not found");
    while (1);
  }
  Serial.println("Sensor found"); 
}

void lightLoop() {
   // to read lux using automatic method, specify VEML_LUX_AUTO
  float lux = veml.readLux(VEML_LUX_AUTO);

  Serial.println("------------------------------------");
  Serial.print("Lux = "); Serial.println(lux);
  Serial.println("Settings used for reading:");
  Serial.print(F("Gain: "));
  switch (veml.getGain()) {
    case VEML7700_GAIN_1: Serial.println("1"); break;
    case VEML7700_GAIN_2: Serial.println("2"); break;
    case VEML7700_GAIN_1_4: Serial.println("1/4"); break;
    case VEML7700_GAIN_1_8: Serial.println("1/8"); break;
  }
  Serial.print(F("Integration Time (ms): "));
  switch (veml.getIntegrationTime()) {
    case VEML7700_IT_25MS: Serial.println("25"); break;
    case VEML7700_IT_50MS: Serial.println("50"); break;
    case VEML7700_IT_100MS: Serial.println("100"); break;
    case VEML7700_IT_200MS: Serial.println("200"); break;
    case VEML7700_IT_400MS: Serial.println("400"); break;
    case VEML7700_IT_800MS: Serial.println("800"); break;
  }

  delay(1000);
}


void screenSetup() {
  tft.init();
  tft.setRotation(2);

  tft.fillScreen(TFT_BLUE);

  tft.fillCircle(120, 120, 120, TFT_BLUE);
  tft.fillCircle(120, 120, 110, TFT_RED);
  tft.fillCircle(120, 120, 100, TFT_BLACK);
  tft.fillCircle(120, 120, 90, TFT_YELLOW);
  tft.fillCircle(120, 120, 80, TFT_GREEN);
  tft.fillCircle(120, 120, 70, TFT_WHITE);

  tft.setTextColor(TFT_BLACK);
  tft.setCursor(67, 100);
  tft.setTextSize(10);
  tft.print("HEY");
}

int countdownValue = 100;

void screenLoop() {
  tft.fillCircle(120, 120, 70, TFT_WHITE);

  // Print the countdown value
  tft.setTextDatum(MC_DATUM);
  tft.drawString(String(countdownValue), tft.width()/2, tft.height()/2);

  // Decrement countdown value
  countdownValue--;

  // Reset countdown when it reaches -1
  if (countdownValue < 0) {
    countdownValue = 100;
  }

  delay(1000); // Wait for 1 second
}