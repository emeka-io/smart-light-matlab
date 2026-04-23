// Source code for smart light system using an Arduino Uno R3 and integrated with MATLAB
const int ldrPin = A0;
const int ledPin = 13;
const int btnPin = 2;
const int bzrPin = 8;

// Hysteresis Thresholds
const int OFF_THRESHOLD = 550; // Must be brighter than this to turn off
const int ON_THRESHOLD = 400;  // Must be darker than this to turn on

bool systemEnabled = true;
bool ledState = false;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(bzrPin, OUTPUT);
  pinMode(btnPin, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  // 1. Handle Button Toggle (Manual Override)
  if (digitalRead(btnPin) == LOW) {
    systemEnabled = !systemEnabled;
    feedbackChirp(systemEnabled);
    delay(300); // Debounce to prevent multiple toggles
  }

  // 2. Read LDR Data
  int ldrValue = analogRead(ldrPin);

  // 3. Send Data to MATLAB
  // This is what allows MATLAB to "hear" the sensor values
  Serial.println(ldrValue); 

  // 4. Hysteresis Logic
  if (systemEnabled) {
    if (ldrValue < ON_THRESHOLD) {
      ledState = true;  // Trigger point: Dark enough to turn ON
    } else if (ldrValue > OFF_THRESHOLD) {
      ledState = false; // Trigger point: Bright enough to turn OFF
    }
    // If ldrValue is between thresholds, ledState doesn't change
    digitalWrite(ledPin, ledState ? HIGH : LOW);
  } else {
    // If system is toggled OFF manually, ensure LED is OFF
    digitalWrite(ledPin, LOW);
  }

  // 5. Stability Delay
  // This keeps the Serial buffer from overflowing and gives MATLAB time to process
  delay(50); 
}

void feedbackChirp(bool state) {
  if (state) {
    // Double Chirp (System ON)
    digitalWrite(bzrPin, HIGH); delay(50); digitalWrite(bzrPin, LOW); delay(50);
    digitalWrite(bzrPin, HIGH); delay(50); digitalWrite(bzrPin, LOW);
  } else {
    // Long Tone (System OFF)
    digitalWrite(bzrPin, HIGH); delay(300); digitalWrite(bzrPin, LOW);
  }
}
