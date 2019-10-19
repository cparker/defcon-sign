# 1 "/opt/projects/defcon/defcon.ino"
# 1 "/opt/projects/defcon/defcon.ino"
# 2 "/opt/projects/defcon/defcon.ino" 2
# 3 "/opt/projects/defcon/defcon.ino" 2
# 15 "/opt/projects/defcon/defcon.ino"
CRGB leds[9];

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

  FastLED.addLeds<WS2812, 2, GRB>(leds, 9);


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

  pinMode(4, 0x0);

  defcon_mode = EEPROM.read(0);
  if (defcon_mode > 4 || defcon_mode < 0) {
    // probably reading EEPROM for the first time
    EEPROM.write(0, 0);
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

int buttonState; // the current reading from the input pin
int lastButtonState = 0x0; // the previous reading from the input pin
unsigned long lastDebounceTime = 0; // the last time the output pin was toggled
unsigned long debounceDelay = 50; // the debounce time; increase if the output flickers



void loop() {

  int reading = digitalRead(4);

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
      if (buttonState == 0x1) {
        Serial.println("PUSH!!");
        defcon_mode = (defcon_mode + 1) % 5;
        EEPROM.write(0, defcon_mode);
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
