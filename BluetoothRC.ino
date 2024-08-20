#include <SoftwareSerial.h>
#include <Servo.h>

SoftwareSerial bluetooth(11, 12); // RX, TX
Servo Myservo;
char t;

#define MLa 4     // Left motor 1st pin
#define MLb 5     // Left motor 2nd pin
#define MRa 6     // Right motor 1st pin
#define MRb 7     // Right motor 2nd pin
#define trigPin 9 // Trig Pin Of HC-SR04
#define echoPin 8 // Echo Pin Of HC-SR04
long duration, distance;

#define L_S A3 // IR sensor Left
#define R_S A4 // IR sensor Right

void setup() {
  Serial.begin(9600);
  bluetooth.begin(9600);
  pinMode(MLa, OUTPUT);   // Left motors forward
  pinMode(MLb, OUTPUT);   // Left motors reverse
  pinMode(MRa, OUTPUT);   // Right motors forward
  pinMode(MRb, OUTPUT);   // Right motors reverse
  pinMode(trigPin, OUTPUT); // Trig Pin as output
  pinMode(echoPin, INPUT);  // Echo Pin as input
  pinMode(R_S, INPUT); // IR sensor right input
  pinMode(L_S, INPUT); // IR sensor left input
  Myservo.attach(10);  // Attach servo motor to pin 10
}

void loop() {
  if (bluetooth.available() > 0) {
    t = bluetooth.read();
    Serial.println(t);
  }

  switch (t) {
    case 'F': // Move forward
      moveForward();
      break;
    case 'B': // Move backward
      moveBackward();
      break;
    case 'L': // Turn left
      turnLeft();
      break;
    case 'R': // Turn right
      turnRight();
      break;
    case 'S': // Stop
      stopMotors();
      break;
  }

  // Obstacle detection and avoidance
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); // Transmit Waves For 10us
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); // Receive Reflected Waves
  distance = duration / 58.2;        // Get Distance
  Serial.println(distance);

  if (distance < 10 && distance > 0) { // Obstacle detected
    avoidObstacle();
  }

  delay(10);
}

void moveForward() {
  digitalWrite(MLa, LOW);
  digitalWrite(MLb, HIGH);
  digitalWrite(MRa, LOW);
  digitalWrite(MRb, HIGH);
}

void moveBackward() {
  digitalWrite(MLa, HIGH);
  digitalWrite(MLb, LOW);
  digitalWrite(MRa, HIGH);
  digitalWrite(MRb, LOW);
}

void turnLeft() {
  digitalWrite(MLa, LOW);
  digitalWrite(MLb, LOW);
  digitalWrite(MRa, LOW);
  digitalWrite(MRb, HIGH);
}

void turnRight() {
  digitalWrite(MLa, LOW);
  digitalWrite(MLb, HIGH);
  digitalWrite(MRa, LOW);
  digitalWrite(MRb, LOW);
}

void stopMotors() {
  digitalWrite(MLa, LOW);
  digitalWrite(MLb, LOW);
  digitalWrite(MRa, LOW);
  digitalWrite(MRb, LOW);
}

void avoidObstacle() {
  stopMotors();
  delay(100);
  
  Myservo.write(0);
  delay(500);
  Myservo.write(180);
  delay(500);
  Myservo.write(90);
  delay(500);

  moveBackward();
  delay(500);
  stopMotors();
  delay(100);
  
  turnLeft();
  delay(500);
  stopMotors();
}
