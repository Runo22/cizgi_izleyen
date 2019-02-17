#include <QTRSensors.h>
#define NUM_SENSORS 8
#define NUM_SAMPLES_PER_SENSOR 4
#define EMITTER_PIN 9

const int enSol = 2;
const int sol_i = 3;      
const int sol_g = 4;     
const int enSag = 5;
const int sag_i = 6;      
const int sag_g = 7;       



int hiz = 70;
float KP = 0.01;
float KD = 0.1;


int lastError = 0; //emin degilim bundan altta =error seklindede yazılabilir
//8 sensor icin
int ortaNok=3500;


//0=siyah cizgi, 1=beyaz cizgi
boolean zemin = 1;


//sıraası ile analog 0-7 arasi 8 tane
//(unsigned char* analogPins, unsigned char numSensors, unsigned char numSamplesPerSensor = 4, unsigned char emitterPin = QTR_NO_EMITTER_PIN)
QTRSensorsAnalog qtra((unsigned char[]) {0,1,2,3,4,5,6,7}, NUM_SENSORS, NUM_SAMPLES_PER_SENSOR, EMITTER_PIN);


void setup() {
  Serial.begin(9600); //led eklemezsek setup in sonuna koy

  pinMode(sag_i, OUTPUT);
  pinMode(sag_g, OUTPUT);
  pinMode(sol_i, OUTPUT);
  pinMode(sol_g, OUTPUT);
  
  int i;
  //Bu alt satır silinebilir ya da azaltılabilir
  delay(100);
  for(i = 0; i < 400; i++)    //burda 250 kullanmis  5sn ben 400 kullandım yaklasık 10 sn
  {
    qtra.calibrate();
    delay(20);
  }

  //TODO buraya bitince led yanmasını sagla

}

void loop() {
  unsigned int sensor[NUM_SENSORS];
  int pozisyon = qtra.readLine(sensor, true, zemin);
  int error = pozisyon - ortaNok;

  //PID
  int fark = KP*error + KD*(error - lastError);
  lastError = error;



  if (fark<0) 
    motor(hiz,hiz+fark);
  else 
    motor(hiz-fark,hiz);
      
}

void sagmotor(int deger)
{
  if (deger>=0)
  {
     digitalWrite(sag_i,HIGH);

     digitalWrite(sag_g,LOW);
  }
  else
  {
   digitalWrite(sag_i,LOW);
    
   digitalWrite(sag_g,HIGH);
    deger*= -1;
  }
 analogWrite(enSag,deger);  
} 

void solmotor(int deger)
{
  if (deger>=0)
  {
    digitalWrite(sol_i,HIGH);
    digitalWrite(sol_g,LOW);
    
  }
  else
  {
    digitalWrite(sol_i,LOW);
    digitalWrite(sol_g,HIGH);
    deger *= -1;
  }
  analogWrite(enSol,deger);
}


void motor(int sol,int sag)
{
  solmotor(sol);
  sagmotor(sag);
}

// fren fonksiyonu 
void fren(boolean sol, boolean sag)
{
  if ( sol )
  {
    digitalWrite(sol_i,HIGH);
    digitalWrite(sol_g,HIGH);
  }
  if ( sag )
  {
    digitalWrite(sag_i,HIGH);
    digitalWrite(sag_g,HIGH);
    
  }
}
