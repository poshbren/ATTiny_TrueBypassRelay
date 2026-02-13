#include <EEPROM.h>

// PINOUT
constexpr uint8_t SWITCH_PIN   = 0;  // Pin 2 - Momentary Normally Open Push Button
constexpr uint8_t RELAY_SET    = 2;  // Pin 4
constexpr uint8_t RELAY_RESET  = 4;  // Pin 7
constexpr uint8_t LED_BUILT_IN = 3;  // Pin 5 - board supplied LED, no external resistor needed
constexpr uint8_t LED_EXTERNAL = 1;  // Pin 3 - external board LED, must have a resistor connected to LED

// TIMING
constexpr uint16_t DEBOUNCE_MS    = 30;
constexpr uint16_t LONG_PRESS_MS  = 400;
constexpr uint16_t RELAY_PULSE_MS = 20;
constexpr uint16_t LED_BLINK_MS   = 150;

// EEPROM
constexpr uint8_t EEPROM_BASE_ADDR = 0;
constexpr uint8_t EEPROM_SLOTS = 64; // Change to 128 to use full eeprom of ATtiny412
uint8_t eepromIndex = 0;

// RELAY PULSE ENGINE
enum class RelayPulseState : uint8_t {
  IDLE,
  PULSING
};

RelayPulseState relayPulseState = RelayPulseState::IDLE;
uint8_t relayPulsePin = 0;
unsigned long relayPulseStart = 0;

// STATES
bool relayState = false;
bool tempRelayState = false;

bool switchState = HIGH;
bool lastSwitchReading = HIGH;

bool allowBlink = true; // Set to false to disable blink
bool longPressActive = false;
bool ledBlinkState = false;

unsigned long debounceTime = 0;
unsigned long pressStartTime = 0;
unsigned long ledBlinkTime = 0;


// LED FUNCTIONS

void setStatusLED(bool on) {
  // Comment out any LED that isn't being used
  digitalWrite(LED_BUILT_IN, on ? HIGH : LOW);
  digitalWrite(LED_EXTERNAL, on ? HIGH : LOW);
}

void updateBlinkLED() {
  if (millis() - ledBlinkTime >= LED_BLINK_MS) {
    ledBlinkTime = millis();
    ledBlinkState = !ledBlinkState;
    setStatusLED(ledBlinkState);
  }
}

// RELAY FUNCTIONS

void startRelayPulse(bool state) {
  if (relayPulseState != RelayPulseState::IDLE) return;

  relayPulsePin = state ? RELAY_SET : RELAY_RESET;
  digitalWrite(relayPulsePin, HIGH);

  relayPulseStart = millis();
  relayPulseState = RelayPulseState::PULSING;
}

void updateRelayPulse() {
  if (relayPulseState == RelayPulseState::PULSING) {
    if (millis() - relayPulseStart >= RELAY_PULSE_MS) {
      digitalWrite(relayPulsePin, LOW);
      relayPulseState = RelayPulseState::IDLE;
    }
  }
}

// EEPROM

bool loadRelayState() {
  for (uint8_t i = 0; i < EEPROM_SLOTS; i++) {
    uint8_t v = EEPROM.read(EEPROM_BASE_ADDR + i);
    if (v == 0 || v == 1) {
      return v;
    }
  }
  return false;
}

void saveRelayState(bool state) {
  EEPROM.update(EEPROM_BASE_ADDR + eepromIndex, state ? 1 : 0);
  eepromIndex = (eepromIndex + 1) % EEPROM_SLOTS;
}

// SWITCH HELPERS

void handleSwitchPressed() {
  pressStartTime = millis();
  longPressActive = false;
}

void handleSwitchReleased() {
  if (longPressActive) {
    restoreAfterLongPress();
    longPressActive = false;
    return;
  }
  toggleRelayAndStore();
}

void handleLongPress() {
  if (switchState != LOW) return;

  if (longPressActive && allowBlink && tempRelayState) { // remove tempRelayState to enable blink on bypass
    updateBlinkLED();
    return;
  }

  if (millis() - pressStartTime < LONG_PRESS_MS) return;

  longPressActive = true;
  tempRelayState = !relayState;
  startRelayPulse(tempRelayState);

  if (tempRelayState) {
    ledBlinkTime = millis();
    ledBlinkState = true;
    setStatusLED(true);
  } else {
    setStatusLED(false);
  }
}

// RELAY HELPERS

void toggleRelayAndStore() {
  relayState = !relayState;
  startRelayPulse(relayState);
  saveRelayState(relayState);
  setStatusLED(relayState);
}

void restoreAfterLongPress() {
  startRelayPulse(relayState);
  setStatusLED(relayState);
}

void setup() {
  pinMode(SWITCH_PIN, INPUT_PULLUP);

  pinMode(RELAY_SET, OUTPUT);
  pinMode(RELAY_RESET, OUTPUT);

  pinMode(LED_BUILT_IN, OUTPUT);
  pinMode(LED_EXTERNAL, OUTPUT);

  digitalWrite(RELAY_SET, LOW);
  digitalWrite(RELAY_RESET, LOW);

  relayState = loadRelayState();
  startRelayPulse(relayState); // Power-up Resync
  setStatusLED(relayState);
}

void loop() {

  updateRelayPulse();

  const bool newSwitchReading = digitalRead(SWITCH_PIN);

  if (newSwitchReading != lastSwitchReading) {
    debounceTime = millis();
    lastSwitchReading = newSwitchReading;
  }

  if ((millis() - debounceTime) <= DEBOUNCE_MS) {
    return;
  }

  if (newSwitchReading == switchState) {
    handleLongPress();
    return;
  }

  switchState = newSwitchReading;

  if (switchState == LOW) {
    handleSwitchPressed();
  } else {
    handleSwitchReleased();
  }
}
