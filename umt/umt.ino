#include <QTRSensors.h>
#define NUM_SENSORS 8
#define NUM_SAMPLES_PER_SENSOR 4
#define EMITTER_PIN 13

#define desired 3500
float KD = 0.1;
float KP = 0.1;
float KG = 0.1;
int pos;
int lastError;
unsigned long last;

QTRSensorsAnalog qtra((unsigned char[]) {
  0, 1, 2, 3, 4, 5, 6, 7
}, NUM_SENSORS, NUM_SAMPLES_PER_SENSOR, EMITTER_PIN);

unsigned int sensor[NUM_SENSORS];

void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  calibration();
}

void loop() {
  PID();
  driveMotors(1, 0, 10 + PID());
  driveMotors(1, 1, 10 - PID());

}

void calibration(void) {
  for (byte i = 0; i < 100; i++)   //burda 250 kullanmis  5sn ben 400 kullandım yaklasık 10 sn
  {
    qtra.calibrate();
    delay(20);
  }
}

double PID(void) {
  
  unsigned long first = millis();
  pos = qtra.readLine(sensor, true, 1);
  int error = desired - pos;
  double ErrorP = KP * error;
  double ErrorD = KD*((error - lastError) / (first - last));
  lastError = error;
  last = first;
  return (ErrorP + ErrorD) * KG;
}

void driveMotors(bool YON, bool MTR, byte HIZ) {
  if (MTR == false) {
    digitalWrite(3, !YON);
    digitalWrite(4, YON);
    analogWrite(2, HIZ);
  }
  else {
    digitalWrite(5, YON);
    digitalWrite(6, !YON);
    analogWrite(7, HIZ);
  }
}
