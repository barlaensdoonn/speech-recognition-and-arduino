// THE DOOR #2
// 10/19/16
//
// make the fan in the front door "breath" by turning on every 2 seconds, then off
// also open the exit door for 15 seconds at a random interval between 1-3 minutes

int doorPin = 8;
int fanPin = 9;

unsigned long fanTimer;
unsigned long doorTimer;
unsigned long randomInterval;
const long doorInterval = 15000;
const long fanInterval = 2500;

boolean fanState = false;
boolean doorState = false;

void setup() {
  pinMode(doorPin, OUTPUT);
  pinMode(fanPin, OUTPUT);
  digitalWrite(doorPin, LOW);
  digitalWrite(fanPin, LOW);

  Serial.begin(9600);
  randomSeed(analogRead(0));

  fanTimer = millis();
  doorTimer = millis();
}

void loop() {
  if (millis() >= fanTimer){
    if (!fanState) {
      fanTimer = millis() + fanInterval;
      digitalWrite(fanPin, HIGH);
      fanState = true;
    }
    else if (fanState) {
      fanTimer = millis() + fanInterval;
      digitalWrite(fanPin, LOW);
      fanState = false;
    }
  }

  if (millis() >= doorTimer) {
    if (!doorState) {
      doorTimer = millis() + doorInterval;
      digitalWrite(doorPin, HIGH);
      doorState = true;
    }
    else if (doorState) {
      randomInterval = random(60000, 180000);
      doorTimer = millis() + randomInterval;
      digitalWrite(doorPin, LOW);
      doorState = false;
    }
  }

}
