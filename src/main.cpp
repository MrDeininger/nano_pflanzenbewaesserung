#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>

#define LED_PIN 10     // Pin für die LED-Anzeige
#define BUTTON_PIN_1 3 // Pin für Push Button 1 (links)
#define BUTTON_PIN_2 2 // Pin für Push Button 2 (rechts)
#define PUMP_PIN 9     // Pin für die Pumpe
#define NUMPIXELS 2    // Anzahl der LEDs


/**
 * EEPROM
*/

void readFromEEPROM();
void saveToEEPROM();

/**
 * Moisture Sensor
*/

void getMoistureReading(int iterations, int delayPerIteration);
const int sensorPin = A0;
int moisture = 100;
uint16_t tmp_moisture = 0;
unsigned long millis_moisture = 0;
int iteration_moisture = 0;
unsigned moisture_min_reading = 1023;
unsigned moisture_max_reading = 0;
bool isMoistureBelowThreshold(uint16_t moisture);
uint16_t threshhold = 0;


/**
 * Serial
*/
void printAllValues();

/**
 * State Machine
*/

void stateMachine(int watering_time, int pause_after_watering, int delay_per_iteration, int iterations_to_average);

unsigned long millis_stateMachine = 0;
int state = 0;
int watering_time = 3000;
int pause_after_watering = 10000;
int delay_per_iteration = 10;
int iterations_to_average = 10;

/**
 * Buttons
 */

void handleButtons();
void button1_onClick();
void button2_onClick();
void both_buttons_onClick();
void both_buttons_longPress();
bool button1_pressed = false;
bool button2_pressed = false;
bool both_buttons_pressed = false;
unsigned long millis_both_buttons = 0;
bool both_buttons_long_pressed = false;

/**
 *LEDs
 */
void handleLEDs();
void fadeLEDs(int upper_limit, float multiplier);
float fade_Brightness = 0;
bool fade_up = true;
unsigned long timer_leds = 0;
void interpolate_color(uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2, uint8_t &r, uint8_t &g, uint8_t &b, float t);

Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, LED_PIN, NEO_GRB + NEO_KHZ800);


/**
 * Setup
*/


void setup()
{
  Serial.begin(9600);                  // Starte die serielle Kommunikation
  strip.begin();                       // Initialisiere die NeoPixel-Bibliothek
  strip.show();                        // Aktualisiere die LED-Anzeige
  pinMode(BUTTON_PIN_1, INPUT_PULLUP); // Setze Push Button 1 als Eingang mit Pull-Up-Widerstand
  pinMode(BUTTON_PIN_2, INPUT_PULLUP); // Setze Push Button 2 als Eingang mit Pull-Up-Widerstand
  pinMode(PUMP_PIN, OUTPUT);           // Setze den Pumppin als Ausgang
  strip.setBrightness(10);             // Setze die Helligkeit der LED-Anzeige auf 50%
  readFromEEPROM();                    // Lese den Wert aus dem EEPROM
}

/**
 * Loop
*/

void loop()
{

  // variables: watering_time, pause_after_watering, delay_per_iteration, iterations_to_average
  stateMachine(watering_time, pause_after_watering, delay_per_iteration, iterations_to_average);
  handleButtons();
  handleLEDs();
  getMoistureReading(10, 10);
}

void saveToEEPROM()
{
  if (threshhold > 100)
  { 
    threshhold = 0;
  }
  threshhold = constrain(threshhold, 0, 100);
  EEPROM.put(0, threshhold);
  if (moisture_min_reading > 1023)
  { 
    moisture_min_reading = 0;
  }
  moisture_min_reading = constrain(moisture_min_reading, 0, 1023);
  EEPROM.put(2, moisture_min_reading);

  if (moisture_max_reading > 1023)
  { 
    moisture_max_reading = 0;
  }
  moisture_max_reading = constrain(moisture_max_reading, 0, 1023);
  EEPROM.put(4, moisture_max_reading);
}

void readFromEEPROM()
{
  if (EEPROM.read(0) == 255)
  {
    EEPROM.put(0, 50);
    EEPROM.put(2, 1023);
    EEPROM.put(4, 0);
  }
  else
  {
    EEPROM.get(0, threshhold);
    EEPROM.get(2, moisture_min_reading);
    EEPROM.get(4, moisture_max_reading);
  }
}

void printAllValues()
{
  // Serial.print("Moisture: ");
  // Serial.print(moisture);
  // Serial.print(" | Threshold: ");
  // Serial.print(threshhold);
  // Serial.print(" | Pump: ");
  // Serial.print(digitalRead(PUMP_PIN));
  // Serial.print(" | State: ");
  // Serial.print(state);
  // Serial.print(" | Moisture min: ");
  // Serial.print(moisture_min_reading);
  // Serial.print(" | Moisture max: ");
  // Serial.print(moisture_max_reading);
  // Serial.print(" | Button 1: ");
  // Serial.print(button1_pressed);
  // Serial.print(" | Button 2: ");
  // Serial.print(button2_pressed);
  // Serial.print(" | Both buttons: ");
  // Serial.print(both_buttons_pressed);
  // Serial.print(" r, g, b: ");
  // Serial.print(strip.getPixelColor(0), HEX);
  // Serial.println();
}

void getMoistureReading(int iterations_to_average, int delay_per_iteration)
{

  if (millis() - millis_moisture > delay_per_iteration)
  {
    tmp_moisture += analogRead(sensorPin);
    iteration_moisture++;
    millis_moisture = millis();
  }

  if (iteration_moisture >= iterations_to_average)
  {
    moisture = tmp_moisture / iterations_to_average;
    tmp_moisture = 0;
    iteration_moisture = 0;

    if (moisture < moisture_min_reading)
    {
      moisture_min_reading = moisture;
      saveToEEPROM();
    }

    if (moisture > moisture_max_reading)
    {
      moisture_max_reading = moisture;
      saveToEEPROM();
    }

    // map the moisture to a value between 0 and 100%
    moisture = map(moisture, moisture_min_reading, moisture_max_reading, 100, 0);
  }
}

bool isMoistureBelowThreshold(uint16_t moisture)
{
  return moisture < threshhold;
}

void handleButtons()
{
  if (digitalRead(BUTTON_PIN_1) == LOW)
  {
    if (!button1_pressed)
    {
      button1_onClick();
      button1_pressed = true;
    }
  }
  else
  {
    button1_pressed = false;
  }

  if (digitalRead(BUTTON_PIN_2) == LOW)
  {
    if (!button2_pressed)
    {
      button2_onClick();
      button2_pressed = true;
    }
  }
  else
  {
    button2_pressed = false;
  }

  if (button1_pressed && button2_pressed)
  {
    if (!both_buttons_pressed)
    {
      millis_both_buttons = millis();
      both_buttons_pressed = true;
      both_buttons_onClick();
    }
    if (millis() - millis_both_buttons > 3000 && !both_buttons_long_pressed)
    {
      if (!both_buttons_long_pressed)
      {
        both_buttons_longPress();
        both_buttons_long_pressed = true;
      }
    }
  }
  else
  {
    both_buttons_pressed = false;
    both_buttons_long_pressed = false;
  }
}

void button1_onClick()
{
  // decrease the threshold by 1%
  threshhold -= 5;
  // save the new threshold to the EEPROM
  saveToEEPROM();

  strip.setBrightness(100);

  strip.setPixelColor(0, 255, 0, 0);
  strip.setPixelColor(1, 255, 0, 0);
  strip.show();
  delay(100);
  strip.setBrightness(10);
  strip.show();
  button1_pressed = false;
  if (button1_pressed)
    button1_onClick();
}

void button2_onClick()
{
  // increase the threshold by 1%
  threshhold += 5;
  // save the new threshold to the EEPROM
  saveToEEPROM();

  strip.setBrightness(40);
  strip.setPixelColor(0, 0, 255, 0);
  strip.setPixelColor(1, 0, 255, 0);
  strip.show();
  delay(100);
  strip.show();
  button2_pressed = false;
  if (button2_pressed)
    button2_onClick();
}

void both_buttons_onClick()
{
  // put threshold to a bit lower than the current moisture
  threshhold = moisture - 5;
  // save the new threshold to the EEPROM
  saveToEEPROM();
  strip.setPixelColor(0, 0, 0, 0);
  strip.setPixelColor(1, 255, 255, 255);
  strip.show();
  delay(100);
  strip.setPixelColor(0, 255, 255, 255);
  strip.setPixelColor(1, 0, 0, 0);
  strip.show();
  delay(100);
  state = 0;
}

void both_buttons_longPress()
{
  // reset the minimum and maximum moisture readings
  moisture_min_reading = 1023;
  moisture_max_reading = 0;

  // save the new values to the EEPROM
  saveToEEPROM();

  // blink the LEDs to indicate that the values have been reset
  for (int i = 0; i < 5; i++)
  {
    strip.setPixelColor(0, 0, 0, 0);
    strip.setPixelColor(1, 255, 0, 0);
    strip.show();
    delay(100);
    strip.setPixelColor(0, 255, 0, 0);
    strip.setPixelColor(1, 0, 0, 0);
    strip.show();
    delay(100);
  }
}

void stateMachine(int watering_time, int pause_after_watering, int delay_per_iteration, int iterations_to_average)
{
  switch (state)
  {
  case 0:
    if (digitalRead(PUMP_PIN))
      digitalWrite(PUMP_PIN, LOW);
    // check if the moisture is below the threshold
    if (isMoistureBelowThreshold(moisture))
    {
      // if so, go to the next state
      state = 1;
      // save the current time
      millis_stateMachine = millis();
    }
    break;
  case 1:
    digitalWrite(PUMP_PIN, HIGH);
    // check if the watering time has passed
    if (millis() - millis_stateMachine > watering_time)
    {
      // if so, go to the next state
      digitalWrite(PUMP_PIN, LOW);
      state = 2;
      // save the current time
      millis_stateMachine = millis();
      // turn on the pump
    }
    break;
  case 2:
    // check if the pause time has passed
    if (millis() - millis_stateMachine > pause_after_watering)
    {
      // if so, go to the next state
      state = 0;
      // save the current time
      millis_stateMachine = millis();
      // turn off the pump
    }
    break;
  }
}

void handleLEDs()
{

  if (!(millis() - timer_leds > 10))
  {
    return;
  }

  timer_leds = millis();

  uint8_t r, g, b;

  float t;

  // interpolate in two steps for the first LED (show moisture)
  // first step: interpolate between red and green for [0, Threshold]
  // second step: interpolate between green and blue for [Threshold, 100]
  if (moisture < threshhold)
  {
    t = (float)moisture / threshhold;
    interpolate_color(255, 0, 0, 0, 255, 0, r, g, b, t);
  }
  else
  {
    t = (float)(moisture - threshhold) / (100 - threshhold);
    interpolate_color(0, 255, 0, 0, 0, 255, r, g, b, t);
  }
  strip.setPixelColor(0, r, g, b, 0);

  // interpolate in two steps for the second LED (show variable threshhold)
  // first step: interpolate between red and green for [0, 50]
  // second step: interpolate between green and blue for [50, 100]

  if (threshhold < 50)
  {
    t = (float)threshhold / 50;
    interpolate_color(255, 0, 0, 0, 255, 0, r, g, b, t);
  }
  else
  {
    t = (float)(threshhold - 50) / 50;
    interpolate_color(0, 255, 0, 0, 0, 255, r, g, b, t);
  }
  strip.setPixelColor(1, r, g, b, 0);

  // fade LEDs based on the state

  switch (state)
  {
  case 0:
    strip.setBrightness(5);
    break;

  case 1:
    fadeLEDs(30, 2);
    break;

  case 2:
    fadeLEDs(15, 0.3);
    break;

  default:
    break;
  }

  strip.show();
}

void interpolate_color(uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2, uint8_t &r, uint8_t &g, uint8_t &b, float t)
{
  r = r1 + (r2 - r1) * t;
  g = g1 + (g2 - g1) * t;
  b = b1 + (b2 - b1) * t;
}

void fadeLEDs(int upper_limit, float multiplier)
{
  if (fade_up)
  {
    fade_Brightness += 1 * multiplier;
    if (fade_Brightness >= upper_limit)
    {
      fade_up = false;
    }
  }
  else
  {
    fade_Brightness -= 1 * multiplier;
    if (fade_Brightness <= 5)
    {
      fade_up = true;
    }
  }
  strip.setBrightness((int)fade_Brightness);
}
