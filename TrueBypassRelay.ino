#include <EEPROM.h>

// PINOUT
constexpr uint8_t SWITCH_PIN    = 4;  // Momentary Normally Open Push Button 
constexpr uint8_t RELAY_SET     = 2;
constexpr uint8_t RELAY_RESET   = 3;
constexpr uint8_t LED_BUILT_IN  = 1;   // board supplied LED, no external resistor needed
constexpr uint8_t LED_EXTERNAL  = 0;   // external board LED, must have a resistor connected to LED

// TIMING
constexpr uint16_t DEBOUNCE_MS     = 30;
constexpr uint16_t LONG_PRESS_MS   = 800;
constexpr uint16_t RELAY_PULSE_MS  = 20;
constexpr uint16_t LED_BLINK_MS    = 250;

// EEPROM
constexpr uint8_t EEPROM_BASE_ADDR = 0;
constexpr uint8_t EEPROM_SLOTS     = 64;
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

bool buttonState = HIGH;
bool lastButtonReading = HIGH;

bool longPressActive = false;
bool ledBlinkState = false;

unsigned long debounceTime = 0;
unsigned long pressStartTime = 0;
unsigned long ledBlinkTime = 0;



// LED functions (handles active-HIGH and active-LOW)

void setStatusLED(bool on) {
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

// Non-blocking relay pulse start

void startRelayPulse(bool state) {
  if (relayPulseState != RelayPulseState::IDLE) return;

  relayPulsePin = state ? RELAY_SET : RELAY_RESET;
  digitalWrite(relayPulsePin, HIGH);

  relayPulseStart = millis();
  relayPulseState = RelayPulseState::PULSING;
}

// Relay pulse update (call every loop)

void updateRelayPulse() {
  if (relayPulseState == RelayPulseState::PULSING) {
    if (millis() - relayPulseStart >= RELAY_PULSE_MS) {
      digitalWrite(relayPulsePin, LOW);
      relayPulseState = RelayPulseState::IDLE;
    }
  }
}


// EEPROM wear-level read

bool loadRelayState() {
  bool lastValid = false;
  bool found = false;

  for (uint8_t i = 0; i < EEPROM_SLOTS; i++) {
    uint8_t v = EEPROM.read(EEPROM_BASE_ADDR + i);
    if (v == 0 || v == 1) {
      lastValid = v;
      eepromIndex = (i + 1) % EEPROM_SLOTS;
      found = true;
    }
  }

  if (!found) {
    eepromIndex = 0;
    return false;
  }
  return lastValid;
}


// EEPROM wear-level write

void saveRelayState(bool state) {
  EEPROM.update(EEPROM_BASE_ADDR + eepromIndex, state ? 1 : 0);
  eepromIndex = (eepromIndex + 1) % EEPROM_SLOTS;
}

void setup() {
  pinMode(SWITCH_PIN, INPUT_PULLUP);

  pinMode(RELAY_SET, OUTPUT);
  pinMode(RELAY_RESET, OUTPUT);

  pinMode(LED_BUILT_IN, OUTPUT);
  pinMode(LED_EXTERNAL, OUTPUT);

  digitalWrite(RELAY_SET, LOW);
  digitalWrite(RELAY_RESET, LOW);

  setStatusLED(false);

  relayState = loadRelayState();
  startRelayPulse(relayState);
  setStatusLED(relayState);
}

void loop() {
  updateRelayPulse();

  bool newButtonReading = digitalRead(SWITCH_PIN);

  // Debouncing
  if (newButtonReading != lastButtonReading) {
    debounceTime = millis();
  }

  if ((millis() - debounceTime) > DEBOUNCE_MS) {
    if (newButtonReading != buttonState) {
      buttonState = newButtonReading;

      // Button pressed
      if (buttonState == LOW) {
        pressStartTime = millis();
        longPressActive = false;
      }
      // Button released
      else {
        if (longPressActive) {
          startRelayPulse(relayState);
          setStatusLED(relayState);
          longPressActive = false;
        } else {
          relayState = !relayState;
          startRelayPulse(relayState);
          saveRelayState(relayState);
          setStatusLED(relayState);
        }
      }
    }
  }

  // Long press detection
  if (buttonState == LOW && !longPressActive &&
      (millis() - pressStartTime) > LONG_PRESS_MS) {

    longPressActive = true;
    tempRelayState = !relayState;
    startRelayPulse(tempRelayState);

    if (!tempRelayState) {
      setStatusLED(false);
    } else {
      ledBlinkTime = millis();
      ledBlinkState = true;
      setStatusLED(true);
    }
  }

  // LED behavior during long press
  if (longPressActive && tempRelayState) {
    updateBlinkLED();
  }

  lastButtonReading = newButtonReading;
}
