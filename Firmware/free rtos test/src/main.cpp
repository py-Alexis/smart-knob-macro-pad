#include <Arduino.h>
#include <SerialANSI.h>

// use only core 1 for the demo
#if CONFIG_FREERTOS_UNICORE 
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

//pins
#define buildinLed 2

int rate = 500;

void toggleLED(void *parameter) {
  while(1) {
    digitalWrite(buildinLed, HIGH);
    vTaskDelay(rate/5 / portTICK_PERIOD_MS);   // portTICK_PERIOD_MS is 1 by default so not really usefull
    digitalWrite(buildinLed, LOW);
    vTaskDelay(rate / portTICK_PERIOD_MS);
  }
}

// void readSerial(void *parameter) {
//   Serial.read()
// }


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  vTaskDelay(200);
  pinMode(buildinLed, OUTPUT);

  xTaskCreatePinnedToCore(
    toggleLED,          // Function to be called
    "Toggle LED",       // Name of task
    1024,               // Stack size (bytes)
    NULL,               // Paramter to pass to function
    1,                  // Task priority
    NULL,                // Task handle
    app_cpu);            // run on core 1 for the demo (not a parameter in the classic xTaskCreate)


   SerialLog("This is a log message");
  SerialImportant("This is an important message");
  SerialCritical("This is a critical error!");
  SerialSuccess("Operation completed successfully!");
  SerialWarning("Warning: Check your configuration.");
  SerialInfo("System is running normally.");
  SerialDebug("Debug: Variable x = 10");
  SerialTitle("=== System Log Start ===");
  SerialHighlight("Special note: this is highlighted.");


  delay(2000);
  SerialClearCurLine();
  delay(2000);
  SerialClearSerial();

  SerialLog("This is a log message");
  SerialImportant("This is an important message");
  SerialCritical("This is a critical error!");
  SerialSuccess("Operation completed successfully!");
  SerialWarning("Warning: Check your configuration.");
  SerialInfo("System is running normally.");
  SerialDebug("Debug: Variable x = 10");
  SerialTitle("=== System Log Start ===");
  SerialHighlight("Special note: this is highlighted.");


  delay(2000);
  SerialClearPrevLine();
  delay(2000);
  SerialHighlight("Special note: this is highlighted.");
  delay(2000);
  SerialClearSerial();
}


void loop() {
  // put your main code here, to run repeatedly:
}

