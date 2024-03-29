#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

const int sensorPinAnalog = A0; 
const int sensorPinDigital = 2;

volatile bool changed = false;
volatile int count = 0; 
volatile int valAnolog = 0;
volatile int valDigital = 1;
volatile int prevDigital = 1;
volatile unsigned long time = 0;

unsigned long quantity = 0;

void setup() {
  Serial.begin(9600);
  pinMode(sensorPinDigital, INPUT_PULLUP); // Set the pin as an input and enable the internal pull-up resistor
  pinMode(sensorPinAnalog, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), beamBreakChanged, CHANGE); // Triggers the beamBreakChanged ISR on a state change on pin 2

  // Setting the LCD display
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Quantity: 0");
  lcd.setCursor(0, 1);
  lcd.print("BreakTime: 0us"); // microsecond
}

void loop() {
  valAnolog = analogRead(sensorPinAnalog);
  // Serial.println("valAnolog: " + String(valAnolog) + "(" + String(float(valAnolog)/1023*5) + "V)");

  if (changed) {
    // If status CHANGE is detected on digitalPinToInterrupt(2), print message
    Serial.println(String(count) + " times" + " valAnolog: " + String(valAnolog) + "(" + String(float(valAnolog)/1023*5) + "V)" + " valDigital: " + String(valDigital) + " Quantity: " + String(quantity));
    

    // bean leave, get leave time and culculate the break time
    if (valDigital == 1 && prevDigital == 0) {
      quantity++;
      Serial.println("break time: " + String(time) + "us");

      // Update the LCD display
      // first line
      lcd.setCursor(10, 0);
      lcd.print("          "); // 10 spaces
      lcd.setCursor(10, 0);
      lcd.print(quantity);

      // second line
      lcd.setCursor(11, 1);
      lcd.print("        "); // 8 spaces
      lcd.setCursor(11, 1);
      lcd.print(String(time) + "us"); // microsecond
    }
    changed = false; // Reset the changed flag
  }
}

// ISR for beam break
void beamBreakChanged() {
  changed = true; // Set the changed flag
  valAnolog = analogRead(sensorPinAnalog);
  prevDigital = valDigital;
  valDigital = digitalRead(sensorPinDigital);
  count++;
  if (valDigital == 0 && prevDigital == 1) {
    time = micros();
  }

  if (valDigital == 1 && prevDigital == 0) {
    time = micros() - time;
  }
}