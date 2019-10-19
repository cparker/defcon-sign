#include <FastLED.h>
#include <EEPROM.h>

#define FASTLED_ALLOW_INTERRUPTS 1
#define FASTLED_INTERRUPT_RETRY_COUNT 1


#define LEDS_PIN            2
#define LED_COUNT           9
#define BUTTON_PIN          4
#define TOTAL_DEFCON_MODES  5
#define MODE_ADDRESS        0


CRGB leds[LED_COUNT];

const CRGB red = CRGB(0, 255, 0);
const CRGB blue = CRGB(0, 0, 255);
const CRGB green = CRGB(255, 0, 0);
const CRGB purple = CRGB(0, 255, 255);
const CRGB yellow = CRGB(255, 255, 0);
const CRGB white = CRGB(255, 255, 255);

const CRGB off = CRGB(0,0,0);

int defcon_mode = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Shall we play a game?");

  FastLED.addLeds<WS2812, LEDS_PIN, GRB>(leds, LED_COUNT);

  
  /*
  int i = 0;
  while (i < NUMBER_LEDS) {
    leds[i] = red;
    FastLED.show();
    delay(250);
    i++;
  }
  */

  // turn on white lights for defcon
  leds[0] = white;
  leds[1] = white;
  leds[2] = white;
  leds[3] = off;  
  leds[4] = white;
  leds[5] = off;
  leds[6] = off;
  leds[7] = off;
  leds[8] = off;

  pinMode(BUTTON_PIN, INPUT);

  defcon_mode = EEPROM.read(MODE_ADDRESS);
  if (defcon_mode > 4 || defcon_mode < 0) {
    // probably reading EEPROM for the first time
    EEPROM.write(MODE_ADDRESS, 0);
  }
}


void numbers_off() {
  leds[3] = off;
  leds[5] = off;
  leds[6] = off;
  leds[7] = off;
  leds[8] = off;
}

void defcon5() {
  numbers_off();
  leds[3] = blue;
}

void defcon4() {
  numbers_off();
  leds[5] = green;
}

void defcon3() {
  numbers_off();
  leds[6] = yellow;
}

void defcon2() {
  numbers_off();
  leds[7] = red;
}

void defcon1() {
  numbers_off();
  leds[8] = white;
}

int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers



void loop() {

  int reading = digitalRead(BUTTON_PIN);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        Serial.println("PUSH!!");
        defcon_mode = (defcon_mode + 1) % TOTAL_DEFCON_MODES;
        EEPROM.write(MODE_ADDRESS, defcon_mode);
      }
    }
  }

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;  

  
  switch (defcon_mode) {

    case 0:
      defcon5();
      break;

    case 1:
      defcon4();
      break;

    case 2:
      defcon3();
      break;

    case 3:
      defcon2();
      break;

    default:
      defcon1();
      break;
  }

  FastLED.show();

  delay(10);  
}
