#include <QTRSensors.h>
#define NUM_SENSORS 8
#define NUM_SAMPLES_PER_SENSOR 4
#define EMITTER_PIN 13

const int enSol = 2;
const int sol_i = 3;
const int sol_g = 4;
const int enSag = 7;
const int sag_i = 5;
const int sag_g = 6;

int hiz = 15;
int ortaNok = 3500;



/* ahmetet*/

double sagMax = 131;//175; // max değer // 200
double solMax = 131;//175; // max değer // 200
double sagBase ;//150; //70Motor tam olarak çizgideyken alacağı değer // 150 // 200
double solBase ;//15 //70 Motor tam olarak çizgideyken alacağı değer // 150 //200
double motorHiz = 0;
double sagmotorHiz = 0;
double solmotorHiz = 0;
float Kp = 0.075;
float Kd = 0.952;
uint16_t say;
int SonHata = 0;
int hata;
double setP = 3800;
/* ahmetet*/

int lastError = 0;
unsigned long son_t;
unsigned long ilk_t = 0;
int pid;



//0=siyah, 1=beyaz
boolean zemin = 0;

void calibration(void);

void motor(int, int);

QTRSensorsAnalog qtra((unsigned char[]) {
  1, 2, 3, 4, 5, 6
}, NUM_SENSORS, NUM_SAMPLES_PER_SENSOR, EMITTER_PIN);
unsigned int sensor[NUM_SENSORS];

void setup() {
  pinMode(sag_i, OUTPUT);
  pinMode(sag_g, OUTPUT);
  pinMode(sol_i, OUTPUT);
  pinMode(sol_g, OUTPUT);


  Serial.begin(9600);
  for (int i = 0; i < 200; i++)
  {
    qtra.calibrate();
  }

}

void loop() {
  PID();

}


void PID() {
  int pos = qtra.readLine(sensor, true, zemin);
  
  hata = pos - setP;
  /*
  if (abs(hata) >= 3800) {
    Kp = 1;
    Kd = 0;
    sagBase = 0;
    solBase = 0;
  }
  *//*
  else if (hata >= -3800 && hata <= 3800)
  {
    */
    Kp = 0.17; // 0.045;
    Kd = 0.6590; // 1
    sagBase = 85; //225 170;
    solBase = 85; //225 170;
    /*
  }
  */
  if (micros() - say > 10000) {
    motorHiz = Kp * hata + Kd * (hata - SonHata);
    SonHata = hata;
    sagmotorHiz = sagBase - motorHiz;
    solmotorHiz = solBase + motorHiz;
    tamirci();
    say = micros();
  }
}


void tamirci() {
  if (sagmotorHiz > sagMax) {
    sagmotorHiz = sagMax;
  }
  if (sagmotorHiz < 0)
  {
    sag_geri(abs(sagmotorHiz)); // + lar ekstra
  }
  else
  {
    sag_ileri(sagmotorHiz);
  }


  /***/
  if (solmotorHiz > solMax) {
    solmotorHiz = solMax;
  }

  if (solmotorHiz < 0)
  {
     sol_geri(abs(solmotorHiz));
  }
  else
  {
    sol_ileri(solmotorHiz);
  }
}

void sag_ileri (int h) {
  analogWrite(enSag, h);
  digitalWrite(sag_i, 1);
  digitalWrite(sag_g, 0);
}
void sag_geri (int h) {
  analogWrite(enSag, h);
  digitalWrite(sag_i, 0);
  digitalWrite(sag_g, 1);
}

void sol_ileri(int h) {
  analogWrite(enSol, h);
  digitalWrite(sol_i, 0);
  digitalWrite(sol_g, 1);
}
void sol_geri (int h) {
  analogWrite(enSol, h);
  digitalWrite(sol_i, 1);
  digitalWrite(sol_g, 0);
}
void sol_dur() {
  analogWrite(enSol, 0);
  digitalWrite(sag_i, 0);
  digitalWrite(sag_g, 0);
}
void sag_dur() {
  analogWrite(enSag, 0);
  digitalWrite(sag_i, 0);
  digitalWrite(sag_g, 0);
}

void OKU() {
  Serial.print(" ");
  int a = 0;
  while (a < 8) {

    Serial.print(sensor[a]);
    Serial.print(" ");
    a = a + 1;
  }
  Serial.println();


}
