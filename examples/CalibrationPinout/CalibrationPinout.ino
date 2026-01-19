#include <ESP32Servo.h>

Servo left_drive;
Servo left_ankle;
Servo right_drive;
Servo right_ankle;

void setup() {
  Serial.begin(115200);

  left_drive.attach(5);
  right_drive.attach(23);
  left_ankle.attach(19);
  right_ankle.attach(18);
}

void loop() {
  Serial.println("moving left drive");
  left_drive.write(45);
  delay(1000);
  left_drive.write(125);
  delay(1000);
  left_drive.write(0);

  Serial.println("moving right drive");
  right_drive.write(45);
  delay(1000);
  right_drive.write(125);
  delay(1000);
  right_drive.write(0);

  Serial.println("moving left ankle");
  left_ankle.write(45);
  delay(1000);
  left_ankle.write(125);
  delay(1000);
  left_ankle.write(0);

  Serial.println("moving right ankle");
  right_ankle.write(45);
  delay(1000);
  right_ankle.write(125);
  delay(1000);
  right_ankle.write(0);
}
