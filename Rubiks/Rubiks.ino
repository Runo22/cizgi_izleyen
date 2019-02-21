#include <QTRSensors.h>
#define NUM_SENSORS 8
#define NUM_SAMPLES_PER_SENSOR 4
#define EMITTER_PIN 13

const int enSol = 2;
const int sol_i = 3;      
const int sol_g = 4;     
const int enSag = 7;
const int sag_i = 6;      
const int sag_g = 5;       

int hiz = 15;
int ortaNok=3500;
float KP = 0.01;
float KD = 0.1;
float KG = 0.1;

int lastError = 0;
unsigned long son_t;
unsigned long ilk_t = 0;
int pid;

//0=siyah, 1=beyaz
boolean zemin = 1;




QTRSensorsAnalog qtra((unsigned char[]) {0,1,2,3,4,5,6,7}, NUM_SENSORS, NUM_SAMPLES_PER_SENSOR, EMITTER_PIN);
unsigned int sensor[NUM_SENSORS];

void setup() {
  pinMode(sag_i, OUTPUT);
  pinMode(sag_g, OUTPUT);
  pinMode(sol_i, OUTPUT);
  pinMode(sol_g, OUTPUT);
  
  calibration();
  }

void loop() {
  pid = PID()
  
  if (pid < 0) 
    {motor(hiz, hiz+pid);}
  else if (pid > 0)
    {motor(hiz-pid, hiz);}
  else 
    {motor(hiz, hiz);}    
  }







void calibration(){
  for(int i = 0; i < 400; i++)
  {qtra.calibrate();
   delay(20);}
  }

int PID(void) {
  int pos = qtra.readLine(sensor, true, zemin);
  int error =ortaNok - pos;
  while (son_t - ilk_t <= 50){
    son_t = millis();
    }
  int fark = (KP*error) + (KD*((error - lastError)/(son_t - ilk_t)));
  ilk_t = son_t;
  lastError = error;
  return KG * fark;
  }

void motor(int sol,int sag){
  digitalWrite(sol_i,HIGH);
  digitalWrite(sol_g,LOW); 
  analogWrite(enSol,sol);
  digitalWrite(sag_i,HIGH);
  digitalWrite(sag_g,LOW);
  analogWrite(enSag,sag); 
  }
