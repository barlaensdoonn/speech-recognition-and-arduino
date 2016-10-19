// THE DOOR
// 10/19/16

// open the grate when ultrasonic sensor is triggered
// leave open 30 seconds
// close grate and pause 3 seconds
// open door for 15 seconds
// close door, wait 15 seconds before grate can be triggered again

#include <NewPing.h>

#define TRIGGER_PIN   12 // Arduino pin tied to trigger pin on ping sensor.
#define ECHO_PIN      11 // Arduino pin tied to echo pin on ping sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

unsigned long pingTimer;
unsigned long grateTimer;
unsigned long gratePauseTimer;
unsigned long doorTimer;
unsigned long doorPauseTimer;

// all times here are in milliseconds //
unsigned int pingSpeed = 50;      // ping frequency. 50ms would be 20 times a second.
const long doorPause = 3000;      // amount of time after grate closing that door pauses before opening
const long doorClose = 18000;     // doorPause - doorClose = amount of time door stays open
const long grateInterval = 30000; // time grate is open
const long gratePause = 15000;    // amount of time after door closing before grate can be triggered again

int distance = 0;
int gratePin = 7;
int doorPin = 8;

bool grateState = false;
bool doorState = false;
bool doorOpen = false;


void setup() {
  Serial.begin(115200);
  
  pinMode(gratePin, OUTPUT);
  pinMode(doorPin, OUTPUT);
  digitalWrite(gratePin, LOW);
  digitalWrite(doorPin, LOW);

  pingTimer = millis();
  grateTimer = millis();
  gratePauseTimer = millis();
  doorTimer = millis();
  doorPauseTimer = millis();
}

void loop() {
  if (millis() >= pingTimer) {   // pingSpeed milliseconds since last ping, do another ping.
    pingTimer += pingSpeed;      // Set the next ping time.
    sonar.ping_timer(echoCheck); // Send out the ping, calls "echoCheck" function every 24uS where you can check the ping status.
  }

  if (distance <= 50 && !grateState && !doorState && millis() >= gratePauseTimer) {
    grateTimer = millis() + grateInterval;
    digitalWrite(gratePin, HIGH);
    grateState = true;
    // Serial.println("grate triggered");
  }
  else if (millis() >= grateTimer && grateState) {
    digitalWrite(gratePin, LOW);
    grateState = false;
    doorState = true;
    // Serial.println("grate off/door on");
  }
  else if (doorState) {
    if (millis() >= doorTimer && !doorOpen) {
      // doorTimer = millis() + doorInterval;
      doorPauseTimer = millis() + doorPause;
      doorTimer = millis() + doorClose;
      // Serial.println("door timers set");
    }
    else if (millis() >= doorPauseTimer && !doorOpen) {
      digitalWrite(doorPin, HIGH);
      doorOpen = true;
      // Serial.println("door open");
    }
    else if (millis() >= doorTimer && doorOpen) {
      digitalWrite(doorPin, LOW);
      doorOpen = false;
      doorState = false;
      gratePauseTimer = millis() + gratePause;
      // Serial.println("door closed");
    }
  }
  
}

void echoCheck() { // Timer2 interrupt calls this function every 24uS where you can check the ping status.
  // Don't do anything here!
  if (sonar.check_timer()) { // This is how you check to see if the ping was received.
    // Here's where you can add code.
    distance = sonar.ping_result / US_ROUNDTRIP_CM;
    
//    Serial.print("Ping: ");
//    Serial.print(sonar.ping_result / US_ROUNDTRIP_CM); // Ping returned, uS result in ping_result, convert to cm with US_ROUNDTRIP_CM.
//    Serial.println("cm");
  }
  // Don't do anything here!
}
