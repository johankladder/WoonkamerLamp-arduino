#include <SoftwareSerial.h>
#include <FastLED.h>

// Pin setup:
SoftwareSerial HM10(0, 1); 
#define NUM_LEDS 12
#define DATA_PIN 3

// Storage for the LED information - like color
CRGB leds[NUM_LEDS];

// Maximum number of input chars. As otherwise
// the Arduino reads max String length. This improves speed. 
const byte numChars = 32; 
char receivedChars[numChars];
boolean newData = false;

// The main color of the leds:
CRGB color;

void setup()
{
  // Add all the leds to the program. Please be aware of powerconsumption: 60ma per pixel!
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS); 

  // Begin serial and BT communication:
  Serial.begin(9600);
  HM10.begin(9600);
  Serial.println("HM10 serial started at 9600");

  // Default color of the leds is GREEN:
  color = CRGB::Green;

  // Turn all leds off to clear previous states:
  FastLED.clear();
  FastLED.show();
}

void loop()
{
    HM10.listen();
    readData();
    showData();
    processData();
    newData = false;
}

void readData() {
    static byte ndx = 0;
    char endMarker = '\n';
    char rc;
    
    while (HM10.available() > 0 && newData == false) {
        rc = HM10.read();

        if (rc != endMarker) {
            receivedChars[ndx] = rc;
            ndx++;
            if (ndx >= numChars) {
                ndx = numChars - 1;
            }
        }
        else {
            receivedChars[ndx] = '\0'; // terminate the string
            ndx = 0;
            newData = true;
        }
    }
}

void showData() {
    if (newData == true) {
        Serial.println(receivedChars);
    }
}

void processData() {
    if (newData == true) {
      String command = receivedChars; 
      String preCommand = command.substring(0,3);

      if (preCommand == "CML") { 
          lightsState(command.charAt(3));
      }

      if (preCommand == "CMD") {
           String brightness = command.substring(3,6);
           setBrightness(brightness.toInt());
      }
   }
}

void lightsState(char value) {
   if (value == '1') {
      lightsOn();
   } else {
      lightsOff();
   }
}


void lightsOn() {
    setAllLedsColor(color);
    FastLED.show(); 
}

void setAllLedsColor(CRGB color) {
    for (int led = 0; led < NUM_LEDS; led++) {
        setLedColor(led, color);
    }
}

void setLedColor(int led, CRGB color) {
    leds[led] = color;
}

void lightsOff() {
    FastLED.clear();
    FastLED.show();
}

void setBrightness(int brightness) {
    FastLED.setBrightness(brightness);
    FastLED.show();
}
