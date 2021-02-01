#include <IRremote.h>

int Echo = A1;
int Trig = A0;

int RECV_PIN = 2;
IRrecv irrecv(RECV_PIN);
decode_results results;

#define LeftMotorGo 7
#define LeftMotorBack 6

#define RightMotorGo 9
#define RightMotorBack 8

#define Right_motor_en 10
#define Left_motor_en 5

#if defined(ARDUINO_ARCH_SAMD)

#define Serial SerialUSB
#endif

long duration;
int distance = 0;

int spin_speed = 50;
int turn_speed = 150;

long LastClick = 0;
long LastHoldDown = 0;

int count = 0;

enum RemoteCode {
  Go = 0xFF30CF, //1
  Back = 0xFF18E7, //2
  TurnLeft = 0xFF7A85, //3
  RightFront = 0xFF10EF, //4
  LeftBack = 0xFF38C7, //5
  RightBack = 0xFF5AA5, //6
  PowerOn = 0xFFA25D, //power button
  PowerOff = 0xFF629D, //Vol+
  HoldDown = 0xFFFFFFFF 
};

RemoteCode remoteCode = PowerOff;
int readRemoteCode = 0;

void setup() {
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  pinMode(LeftMotorGo, OUTPUT);
  pinMode(LeftMotorBack, OUTPUT);
  pinMode(RightMotorGo, OUTPUT);
  pinMode(RightMotorBack, OUTPUT);
  pinMode(Right_motor_en, OUTPUT);
  pinMode(Left_motor_en, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  irrecv.enableIRIn();
  Serial.begin(115200);
}

long RemoteControl() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    irrecv.resume();
  }
  delay(100);
  return results.value;
}

void DistanceSensor() {
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  duration = pulseIn(Echo, HIGH);
  distance = duration * 0.034 / 2;
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
}

void GoBack() {
  digitalWrite(Left_motor_en, HIGH);
  analogWrite(Left_motor_en, 255);
  digitalWrite(Right_motor_en, HIGH);
  analogWrite(Right_motor_en, 255);
  digitalWrite(LeftMotorGo, LOW);
  analogWrite(LeftMotorGo, 0);
  digitalWrite(LeftMotorBack, HIGH);
  analogWrite(LeftMotorBack, 255);
  digitalWrite(RightMotorGo, LOW);
  analogWrite(RightMotorGo, 0);
  digitalWrite(RightMotorBack, HIGH);
  analogWrite(RightMotorBack, 255);
}

void GoForwards() {
  digitalWrite(Left_motor_en, HIGH);
  analogWrite(Left_motor_en, 255);
  digitalWrite(Right_motor_en, HIGH);
  analogWrite(Right_motor_en, 255);
  digitalWrite(LeftMotorGo, HIGH);
  analogWrite(LeftMotorGo, 255);
  digitalWrite(LeftMotorBack, LOW);
  analogWrite(LeftMotorBack, 0);
  digitalWrite(RightMotorGo, HIGH);
  analogWrite(RightMotorGo, 255);
  digitalWrite(RightMotorBack, LOW);
  analogWrite(RightMotorBack, 0);
}

void turnLeft(int spin_speed, int turn_speed) {
  digitalWrite(Left_motor_en, HIGH);
  analogWrite(Left_motor_en, 255);
  digitalWrite(Right_motor_en, HIGH);
  analogWrite(Right_motor_en, 255);
  digitalWrite(RightMotorGo, HIGH);
  analogWrite(RightMotorGo, turn_speed);
  digitalWrite(LeftMotorGo, HIGH);
  analogWrite(LeftMotorGo, spin_speed);
  digitalWrite(RightMotorBack, LOW);
  analogWrite(RightMotorBack, 0);
  digitalWrite(LeftMotorBack, LOW);
  analogWrite(LeftMotorBack, 0);
}

void StopForward() {
  digitalWrite(Left_motor_en, HIGH);
  analogWrite(Left_motor_en, 255);
  digitalWrite(Right_motor_en, HIGH);
  analogWrite(Left_motor_en, 255);
  digitalWrite(LeftMotorGo, LOW);
  analogWrite(LeftMotorGo, 0);
  digitalWrite(RightMotorGo, LOW);
  analogWrite(RightMotorGo, 0);
}

void StopAll() {
  digitalWrite(Left_motor_en, HIGH);
  analogWrite(Left_motor_en, 255);
  digitalWrite(Right_motor_en, HIGH);
  analogWrite(Left_motor_en, 255);
  digitalWrite(LeftMotorGo, LOW);
  analogWrite(LeftMotorGo, 0);
  digitalWrite(LeftMotorBack, LOW);
  analogWrite(LeftMotorBack, 0);
  digitalWrite(RightMotorGo, LOW);
  analogWrite(RightMotorGo, 0);
  digitalWrite(RightMotorBack, LOW);
  analogWrite(RightMotorBack, 0);
}

void loop() {
  LastHoldDown = 0;
   switch (RemoteControl()) {
    case Go:
      LastClick = Go;
      GoForwards();
      break;
    case Back:
      LastClick = Back;
      GoBack();
      break;
    case PowerOn:
      LastClick = PowerOn;
      GoForwards();
      break;
    case PowerOff:
      LastClick = PowerOff;
      StopAll();
      break;
    case HoldDown:
      count = 0;
      Serial.println("HoldDown");
      Serial.println(count);
      LastHoldDown = HoldDown;
      if (LastClick = Go) {
        GoForwards();
      }
      else if (LastClick == Back) {
        GoBack();
      }
      else if (LastClick == PowerOn) {
        GoForwards();
      }
      else if (LastClick == PowerOff) {
        StopAll();
      }
      break;
   }
   if (LastHoldDown == 0 && (LastClick != PowerOn || LastClick != PowerOff || LastClick != Go || LastClick != Back)) {
    StopAll();
    count++;
    Serial.println(count);
   }
   
   if (distance <= 40) {
      StopForward();
   }
   DistanceSensor();
  //turnLeft(spin_speed, turn_speed);
}
