

int sensorValue = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("启动了");
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  pinMode(12, INPUT_PULLUP);
}

void ReadA0(){
  sensorValue = analogRead(A0);
  Serial.print("A0= ");
  Serial.print(sensorValue);
}

void InputCL(){
    if(digitalRead(12)==0){
    digitalWrite(2, LOW);
    delay(100);        // 为保证稳定性，读取之间的延迟
    while(digitalRead(12)==0){
      delay(500);
    }
  }
}

void BoardLight(){
  digitalWrite(2, HIGH);
  delay(1000);
  digitalWrite(2, LOW);
  delay(1000);
  
}

void PWMout(){
  analogWrite(ledPin, 1024);
  delay(100);
  analogWrite(ledPin, 0);
  delay(100);
}
void loop() {
  
  BoardLight();
}
