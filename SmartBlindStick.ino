
const int trigPin = 10;
const int echoPin = 11;
const int ldrPin = A0;
const int trigPin2 = 12;
const int echoPin2 = 13;
int motor = 7;
int digitaInput = 4;
int speaker = 5;

float duration, distance;
float duration2, distance2;


int latchPin = 9;  //Storage register clock pin
int clockPin = 3;  //Shift register clock pin
int dataPin = 2;

int numOfRegisters = 2;
byte* registerState;

long effectId = 0;
long prevEffect = 0;
long effectRepeat = 0;
long effectSpeed = 30;




void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(motor, OUTPUT);

  pinMode(speaker, OUTPUT);
  pinMode(digitaInput, INPUT);



  //Initialize for led
  registerState = new byte[numOfRegisters];
  for (size_t i = 0; i < numOfRegisters; i++) {
    registerState[i] = 0;
  }

  //set pins to output so you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  //Init LDR
  pinMode(ldrPin, INPUT);


  Serial.begin(9600);

}

void loop() {

  checkDarkness();
  firstSensor();
  secondSensor();
  soilSensor();



}

void checkDarkness() {
  int ldrStatus = analogRead(ldrPin);

  if (ldrStatus <= 300) {

    Serial.print("Its DARK value is: ");

    Serial.println(ldrStatus);

      ledOn();

  } else {
      effectF(10);


    Serial.print("Its BRIGHT, Turn off the LED : ");

    Serial.println(ldrStatus);

  }
}

void ledOn() {
  effectId = random(6);
  effectSpeed = 40;
  Serial.println("B");
  effectB(effectSpeed);
  Serial.println("D");
  effectD(effectSpeed);

}

void firstSensor() {
  digitalWrite(trigPin, LOW);

  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);

  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distance = (duration / 2) / 29.1;
  Serial.println("A is");
  Serial.println(distance);

  if (distance > 40 && distance < 70) {
    tone(speaker, 1000);
    delay(10);
    noTone(speaker);
    Serial.println("buzzer on");
  }

  if (distance < 40)// This is where checking the distanceyou can change the value

  {

    digitalWrite(motor, HIGH);


  } else

  {

    digitalWrite(motor, LOW);


  }
  delay(500);

}

void secondSensor() {
  digitalWrite(trigPin2, LOW);

  delayMicroseconds(2);

  digitalWrite(trigPin2, HIGH);

  delayMicroseconds(10);

  digitalWrite(trigPin2, LOW);

  duration2 = pulseIn(echoPin2, HIGH);

  distance2 = (duration2 / 2) / 29.1;
  Serial.println("B is");
  Serial.println(distance2);
  if (distance2 > 40 && distance2 < 70) {
    tone(speaker, 1000);
    delay(10);
    noTone(speaker);
    Serial.println("buzzer on");
  }

  if (distance2 < 40)// This is where checking the distanceyou can change the value

  {

    digitalWrite(motor, HIGH);



  }

  else

  {
    digitalWrite(motor, LOW);

  }
  delay(500);

}




void effectA(int speed) {
  for (int i = 0; i < 16; i++) {
    for (int k = i; k < 16; k++) {
      regWrite(k, HIGH);
      delay(speed);
      regWrite(k, LOW);
    }

    regWrite(i, HIGH);
  }
}

void effectB(int speed) {
  for (int i = 15; i >= 0; i--) {
    for (int k = 0; k < i; k++) {
      regWrite(k, HIGH);
      delay(speed);
      regWrite(k, LOW);
    }

    regWrite(i, HIGH);
  }
}

void effectC(int speed) {
  int prevI = 0;
  for (int i = 0; i < 16; i++) {
    regWrite(prevI, LOW);
    regWrite(i, HIGH);
    prevI = i;

    delay(speed);
  }

  for (int i = 15; i >= 0; i--) {
    regWrite(prevI, LOW);
    regWrite(i, HIGH);
    prevI = i;

    delay(speed);
  }
}

void effectD(int speed) {
  for (int i = 0; i < 8; i++) {
    for (int k = i; k < 8; k++)
    {
      regWrite(k, HIGH);
      regWrite(15 - k, HIGH);
      delay(speed);
      regWrite(k, LOW);
      regWrite(15 - k, LOW);
    }

    regWrite(i, HIGH);
    regWrite(15 - i, HIGH);
  }
}

void effectE(int speed) {
  for (int i = 7; i >= 0; i--) {
    for (int k = 0; k <= i; k++)
    {
      regWrite(k, HIGH);
      regWrite(15 - k, HIGH);
      delay(speed);
      regWrite(k, LOW);
      regWrite(15 - k, LOW);
    }

    regWrite(i, HIGH);
    regWrite(15 - i, HIGH);
  }
}


void effectF(int speed) {
  for (int i = 7; i >= 0; i--) {
    for (int k = 0; k <= i; k++)
    {
      regWrite(k, LOW);
      regWrite(15 - k, LOW);
      delay(speed);
      regWrite(k, LOW);
      regWrite(15 - k, LOW);
    }

    regWrite(i, LOW);
    regWrite(15 - i, LOW);
  }
}


void regWrite(int pin, bool state) {
  //Determines register
  int reg = pin / 8;
  //Determines pin for actual register
  int actualPin = pin - (8 * reg);

  //Begin session
  digitalWrite(latchPin, LOW);

  for (int i = 0; i < numOfRegisters; i++) {
    //Get actual states for register
    byte* states = &registerState[i];

    //Update state
    if (i == reg) {
      bitWrite(*states, actualPin, state);
    }

    //Write
    shiftOut(dataPin, clockPin, MSBFIRST, *states);
  }

  //End session
  digitalWrite(latchPin, HIGH);
}
