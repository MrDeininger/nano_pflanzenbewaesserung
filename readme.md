# Pflanzenbewässerungssystem

Dieses Projekt ist ein Pflanzenbewässerungssystem, das in C++ für Arduino implementiert wurde, wie in der [main.cpp](src/main.cpp) Datei zu sehen ist.

## Platine


Die Gerber Dateien, mit denen die Platine zum Projekt bestellt werden kann sind im Verzeichnis zu finden. (Gerber_Files) einfach herunterladen und z. B. auf jlcpcb.com, ... bestellen

![Hier sollte das Foto sein](platine.png)

### Beispielfoto

So könnte das Projekt am ende aussehen.

![Hier sollte das Foto sein](pflanzenfoto.jpg)

## Steuerung
Im Code sind die folgenden Befehle auf den Buttons:

* linker Button klicken: Senke die Zielfeuchtigkeit in der Erde um 5% (Erde im Blumentopf muss also 5% trockener sein, bevor die Pumpe anfängt zu pumpen) 
* rechter Button klicken: Erhöhe die Zielfeuchtigkeit in der Erde 5% (Pumpe fängt also schon bei 5% mehr Wasser im Boden an zu pumpen)
* beide Buttons gleichzeitig (kurz): setze Zielfeuchtigkeit 10% unter momentaner Feuchtigkeit
* beide Buttons gleichzeitig (> 3 Sekunden): setze Sensorkalibrierung zurück

## Teileliste

* Arduino nano
* doppelte Lüsterklemme 
* dreifache Lüsterklemme
* TIP120 Leistungstransistor
* kapazitiver Feuchtesensor
* 2x Tactile Switch button
* 5v Wasserpumpe
* Wasserspeicher
* Platine

-> ca 15€ kosten


## Überblick

Das System misst den Feuchtigkeitsgehalt des Bodens und bewässert die Pflanze, wenn der Feuchtigkeitsgehalt unter einen bestimmten Schwellenwert fällt. Es beinhaltet auch Funktionen zum Speichern und Lesen von Daten aus dem EEPROM, zum Verarbeiten von Tastendrücken und zur Steuerung von LEDs.

## Schlüsselfunktionen

- `setup()`: Stellt den Anfangszustand des Systems ein.
- `loop()`: Hauptzyklus, in dem das System den Feuchtigkeitsgehalt überprüft und entscheidet, ob die Pflanze bewässert werden soll.
- `saveToEEPROM()`: Speichert Daten im EEPROM.
- `readFromEEPROM()`: Liest Daten aus dem EEPROM.
- `printAllValues()`: Druckt alle Werte für Debugging-Zwecke.
- `getMoistureReading(int iterations_to_average, int delay_per_iteration)`: Erhält die Feuchtigkeitsmessung vom Sensor.
- `isMoistureBelowThreshold(uint16_t moisture)`: Überprüft, ob der Feuchtigkeitsgehalt unter dem Schwellenwert liegt.
- `handleButtons()`: Verarbeitet Tastendrücke.
- `button1_onClick()`, `button2_onClick()`, `both_buttons_onClick()`, `both_buttons_longPress()`: Funktionen zur Verarbeitung spezifischer Tastendruckereignisse.
- `stateMachine(int watering_time, int pause_after_watering, int delay_per_iteration, int iterations_to_average)`: Steuert den Zustand des Systems.
- `handleLEDs()`: Steuert die LEDs.
- `interpolate_color(uint8_t r1, uint8_t g1, uint8_t b1, uint8_t r2, uint8_t g2, uint8_t b2, uint8_t &r, uint8_t &g, uint8_t &b, float t)`: Interpoliert zwischen zwei Farben.

## Abhängigkeiten

Das Projekt hängt von der Adafruit NeoPixel-Bibliothek ab, wie in der [platformio.ini](platformio.ini) Datei angegeben.

## Bau und Betrieb

Dieses Projekt wird mit PlatformIO gebaut und betrieben. Bitte beziehen Sie sich auf die [PlatformIO-Dokumentation](https://docs.platformio.org/page/projectconf.html) für weitere Informationen zum Bau und Betrieb des Projekts.

## LED-Anzeigen

Die obere LED zeigt die Feuchtigkeit im Blumentopf an (von trocken: rot zu okay: grün zu feucht: blau), und die untere LED zeigt den Schwellenwert (gleicher Farbverlauf).

