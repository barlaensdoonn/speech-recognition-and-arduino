// THE DOOR
// 10/19/16

// open the grate when ultrasonic sensor is triggered
// leave open 30 seconds
// close grate and open door for 15 seconds

#include <NewPing.h>

#define TRIGGER_PIN   12 // Arduino pin tied to trigger pin on ping sensor.
#define ECHO_PIN      11 // Arduino pin tied to echo pin on ping sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

unsigned int pingSpeed = 50; // How frequently are we going to send out a ping (in milliseconds). 50ms would be 20 times a second.
unsigned long pingTimer;
unsigned long grateTimer;
const long grateInterval = 3000;

int distance = 0;

int gratePin = 7;
int doorPin = 8;

boolean grateState = false;
boolean doorState = false;


void setup() {
  Serial.begin(115200);
  
  pinMode(gratePin, OUTPUT);
  pinMode(doorPin, OUTPUT);
  digitalWrite(gratePin, LOW);
  digitalWrite(doorPin, LOW);

  pingTimer = millis();
  grateTimer = millis();
}

void loop() {
  if (millis() >= pingTimer) {   // pingSpeed milliseconds since last ping, do another ping.
    pingTimer += pingSpeed;      // Set the next ping time.
    sonar.ping_timer(echoCheck); // Send out the ping, calls "echoCheck" function every 24uS where you can check the ping status.
  }

  if (distance <= 50) {
    if (!grateState) {
      grateTimer = millis() + grateInterval;
      digitalWrite(gratePin, HIGH);
      grateState = true;
    }
  }
  else if (millis() >= grateTimer && grateState) {
    digitalWrite(gratePin, LOW);
    grateState = false;
  }

}

void echoCheck() { // Timer2 interrupt calls this function every 24uS where you can check the ping status.
  // Don't do anything here!
  if (sonar.check_timer()) { // This is how you check to see if the ping was received.
    // Here's where you can add code.
    distance = sonar.ping_result / US_ROUNDTRIP_CM;
    
    Serial.print("Ping: ");
    Serial.print(sonar.ping_result / US_ROUNDTRIP_CM); // Ping returned, uS result in ping_result, convert to cm with US_ROUNDTRIP_CM.
    Serial.println("cm");
  }
  // Don't do anything here!
}
