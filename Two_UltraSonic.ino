#include <IRremote.h>

int IR_RECEIVE_PIN = 11;
IRrecv IrReceiver(IR_RECEIVE_PIN);

#define echoPinBack 5

#define trigPinBack 6

#define echoPinFront 9

#define trigPinFront 10

#define motorLeftFront 7

#define motorRightFront 8

#define motorRightBack 3

#define motorLeftBack 4

#if defined(ARDUINO_ARCH_SAMD)
#define Serial SerialUSB
#endif

long duration;

int distanceBack = 0;

int distanceFront = 0;

enum Direction {
  front,
  back
};

enum RemoteCode {
  Go = 0xFF30CF, //1
  Back = 0xFF18E7, //2
  LeftFront = 0xFF7A85, //3
  RightFront = 0xFF10EF, //4
  LeftBack = 0xFF38C7, //5
  RightBack = 0xFF5AA5, //6
  PowerOn = 0xFFA25D, //power button
  PowerOff = 0xFF629D //Vol+
};

Direction Cdirection = front;

RemoteCode remoteCode = PowerOff;
int readRemoteCode = 0;

void setup() {
  pinMode(trigPinFront, OUTPUT);
  pinMode(echoPinFront, INPUT);
  pinMode(trigPinBack, OUTPUT);
  pinMode(echoPinBack, INPUT);
  Serial.begin(115200);
  Serial.println("Ultrasonic Sensor HC-SR04 Test");
  pinMode(motorRightFront, OUTPUT);
  pinMode(motorLeftFront, OUTPUT);
  pinMode(motorRightBack, OUTPUT);
  pinMode(motorLeftBack, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  RemoteSetup();
}

void RemoteSetup() {
    #if defined(__AVR_ATmega32U4__) || defined(SERIAL_USB) || defined(SERIAL_PORT_USBVIRTUAL)
    delay(2000); // To be able to connect Serial monitor after reset and before first printout
    #endif
    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ " from " __DATE__));

    // In case the interrupt driver crashes on setup, give a clue
    // to the user what's going on.
    Serial.println("Enabling IRin");
    IrReceiver.enableIRIn();  // Start the receiver
    //IrReceiver.blink13(true); // Enable feedback LED

    Serial.print(F("Ready to receive IR signals at pin "));
    Serial.println(IR_RECEIVE_PIN);
}
/*void RemoteControl() {
  if (IrReceiver.decode()) {
        IrReceiver.printResultShort(&Serial);
        readRemoteCode = &Serial;
        Serial.println();

        IrReceiver.resume();
    }
    delay(100);
}*/

void StopFrontMotors() {
  digitalWrite(motorRightFront, LOW);
  digitalWrite(motorLeftFront, LOW);
}

void GoFrontMotors() {
  digitalWrite(motorRightFront, LOW);
  delayMicroseconds(2);
  digitalWrite(motorRightFront, 150);
  delayMicroseconds(10000);
  digitalWrite(motorLeftFront, LOW);
  delayMicroseconds(2);
  digitalWrite(motorLeftFront, 150);
  delayMicroseconds(10000);
}

void StopBackMotors() {
  digitalWrite(motorRightBack, LOW);
  digitalWrite(motorLeftBack, LOW);
}

void GoBackMotors() {
  digitalWrite(motorRightBack, LOW);
  delayMicroseconds(2);
  digitalWrite(motorRightBack, 150);
  delayMicroseconds(10000);
  digitalWrite(motorLeftBack, LOW);
  delayMicroseconds(2);
  digitalWrite(motorLeftBack, 150);
  delayMicroseconds(10000);
}

void FrontDistanceSensor() {
  digitalWrite(trigPinFront, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinFront, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinFront, LOW);
  duration = pulseIn(echoPinFront, HIGH);
  distanceFront = duration * 0.034 / 2;
  //Serial.print("DistanceFront: ");
  //Serial.print(distanceFront);
  //Serial.println(" cm");
}

void BackDistanceSensor() {
  digitalWrite(trigPinBack, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinBack, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinBack, LOW);
  duration = pulseIn(echoPinBack, HIGH);
  distanceBack = duration * 0.034 / 2;
  //Serial.print("DistanceBack: ");
  //Serial.print(distanceBack);
  //Serial.println(" cm");
}

void loop() {
   if (IrReceiver.decode()) {
       IrReceiver.printResultShort(&Serial);
       IrReceiver.printResultShort(&readRemoteCode);
       readRemoteCode.println();

       IrReceiver.resume();
   }
    delay(100);
  Serial.println("remoteCode: ");
  Serial.println(readRemoteCode);
  Serial.println("PowerOn: ");
  Serial.println(PowerOn);
  /*if (readRemoteCode == PowerOn) {
    remoteCode = PowerOn;
    GoFrontMotors();
    Serial.println(remoteCode);
  }*/
  /*if (remoteCode == PowerOff) {
    StopFrontMotors();
    StopBackMotors();
    Serial.println("yes");
  }*/
  /*if (distanceBack <= 10) {
    GoFrontMotors();
    StopBackMotors();
  }
  else if (distanceFront <= 10) {
    StopFrontMotors();
    GoBackMotors();
  }
  if ((Cdirection == front)&&(distanceFront > 10)) {
    GoFrontMotors();
    StopBackMotors();
  }
  if ((Cdirection == back)&&(distanceBack > 10)) {
    StopFrontMotors();
    GoBackMotors();
  }
  FrontDistanceSensor();
  BackDistanceSensor();
  delay(100);*/
  //GoFrontMotors();
  //GoBackMotors();
}
