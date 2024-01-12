# Plant Watering System

This project is a plant watering system implemented in C++ for Arduino, as seen in the [main.cpp](src/main.cpp) file.

## Overview

The system measures the moisture level of the soil and waters the plant if the moisture level falls below a certain threshold. It also includes functionality for saving and reading data from EEPROM, handling button presses, and controlling LEDs.

## Key Functions

- `setup()`: Sets up the initial state of the system.
- `loop()`: Main loop where the system checks the moisture level and decides whether to water the plant.
- `saveToEEPROM()`: Saves data to EEPROM.
- `readFromEEPROM()`: Reads data from EEPROM.
- `printAllValues()`: Prints all values for debugging purposes.
- `getMoistureReading(int iterations_to_average, int delay_per_iteration)`: Gets the moisture reading from the sensor.
- `isMoistureBelowThreshold(uint16_t moisture)`: Checks if the moisture level is below the threshold.
- `handleButtons()`: Handles button presses.
- `button1_onClick()`, `button2_onClick()`, `both_buttons_onClick()`, `both_buttons_longPress()`: Functions for handling specific button press events.
- `stateMachine(int watering_time, int pause_after_watering, int delay_per_iteration, int iterations_to_average)`: Controls the state of the system.
- `handleLEDs()`: Controls the LEDs.
- `interpolate_color(uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2, uint8_t &r, uint8_t &g, uint8_t &b, float t)`: Interpolates between two colors.

## Dependencies

The project depends on the Adafruit NeoPixel library, as specified in the [platformio.ini](platformio.ini) file.

## Building and Running

This project is built and run using PlatformIO. Please refer to the [PlatformIO documentation](https://docs.platformio.org/page/projectconf.html) for more information on how to build and run the project.