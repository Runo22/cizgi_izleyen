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


/* ahmetet*/
// ???????????sag sol base e baslangic ekledim????????????????

double sagMax = 100;//175; // max değer // 200
double solMax = 100;//175; // max değer // 200
double sagBase;//150; //70Motor tam olarak çizgideyken alacağı değer // 150 // 200
double solBase;//15 //70 Motor tam olarak çizgideyken alacağı değer // 150 //200
double motorHiz = 0;
double sagmotorHiz = 0;
double solmotorHiz = 0;
float Kp = 0.075;
float Kd = 0.952;
uint16_t say;
int SonHata = 0;
int hata;
double setP = 3777;
/* ahmetet*/


//1=siyah, 0=beyaz çizgi
boolean zemin = 0;

//**************mz80 pini ayarla***************
int mz80 = 9;   
int sayac = 0;
void sag_ileri (int);
void sol_ileri (int);

QTRSensorsAnalog qtra((unsigned char[]) {
  1, 2, 3, 4, 5, 6
}, NUM_SENSORS, NUM_SAMPLES_PER_SENSOR, EMITTER_PIN);
unsigned int sensor[NUM_SENSORS];

void setup() {
  pinMode(sag_i, OUTPUT);
  pinMode(sag_g, OUTPUT);
  pinMode(sol_i, OUTPUT);
  pinMode(sol_g, OUTPUT);
  pinMode(mz80, INPUT);

  for (int i = 0; i < 200; i++)
  {
    qtra.calibrate();
  }
  sag_ileri(30);
  sol_ileri(30);
}

void loop() {
  if (sayac == 0){
    engel();
  }
  PID();
  
}
void engel(){
  int engelVeri = digitalRead(mz80);
  if ((sayac == 0) && (engelVeri == 1)){
    sol_dur();
    sag_ileri(40);     // bura oyna engel icin
    delay(150);
    sag_ileri(30);
    sol_ileri(30);
    sayac++;
  }
}

void PID() {
  int pos = qtra.readLine(sensor, true, zemin);
   hata = pos - setP;
  /*Serial.begin(9600);
  Serial.print("Hata:");
  Serial.println(hata);
  Serial.print("Poz:");
  Serial.println(pos);*/
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
    rubiks();
    say = micros();
  }
}


void rubiks() {
  if (sagmotorHiz > sagMax) {
    sagmotorHiz = sagMax;
  }
  if (sagmotorHiz < 0)
  {
    sag_geri(abs(sagmotorHiz));
  }
  else
  {
    sag_ileri(sagmotorHiz);
  }


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
