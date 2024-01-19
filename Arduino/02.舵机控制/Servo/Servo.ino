
#include <Servo.h>
Servo myservo;  // create servo object to control a servo

void setup() {
  Serial.begin(9600);
  Serial.println("启动了");

  myservo.attach(14, 500, 2500);

}

void loop() {
  for (int i = 0 ; i < 180 ; i = i + 5) {
    myservo.write(i);
    delay(100);
  }
  for (int i = 0 ; i < 180 ; i = i + 5) {
    myservo.write(180 - i);
    delay(100);
  }
}
