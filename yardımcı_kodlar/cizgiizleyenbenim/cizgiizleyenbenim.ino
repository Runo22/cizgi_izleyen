#include <QTRSensors.h>

#define SAG1 3 // A SAĞ MOTOR OLSUN
#define SAG2 4
#define PWMA 5

#define SOL1 6 // B SAĞ MOTOR
#define SOL2 7
#define PWMB 8
#define STBY 9

#define sensorsayisi 8
#define sensororneksayisi 4
#define emiterpini 10
#define LED 11
#define MZ80 12
int hiz=70;


boolean zemin=1; // 1 siyah zemin , 0 beyaz zemin olsun

int hata=0;
int turev=0;

float KP=0.01;
float KD=0.1;

int pozisyon=0;

int fark=0; // motorların fark değişkeni

int son_hata; // Orantılı son değer (hataların türevini hesaplamak için kullanılır.)

int hedef=2500; // ayar noktası (her sensor arası 2500)

int sensor[sensorsayisi];

QTRSensorsAnalog qtra((unsigned char[]){A0 , A1 , A2 , A3 , A4 , A5},sensorsayisi);

void setup()
{
  pinMode(SAG1,OUTPUT);
  pinMode(SAG2,OUTPUT);
  pinMode(PWMA,OUTPUT);
  pinMode(SOL1,OUTPUT);
  pinMode(SOL2,OUTPUT);
  pinMode(PWMB,OUTPUT);
  pinMode(LED,OUTPUT);
  pinMode(STBY,OUTPUT);
  pinMode(SAG1,OUTPUT);

// LED YANIP SÖNERKEN YANARKEN KALİBRASYON



  for ( int i-0; i<70; i++)
  {
    digitalWrite(LED, HIGH); delay(20);
    qtra.calibrate();
    digitalWrite(LED, LOW); delay(20);   
  }
  while(digitalRead(MZ80)==0) fren(1,1);
}

void loop ()
{
  pozisyon=qtra.readLine(sensor, true , zemin);
  hata=pozisyon-hedef;
  turev=hata-son_hata;
  son_hata = hata;
  fark=(hata*KP)+(turev*KD);

  if (fark<0) 
    motor(hiz,hiz+fark);
  else 
    motor(hiz-fark,hiz);
      
}
// sağ motor sürücü fonksiyonu
void sagmotor(int deger)
{
  if (deger>=0)
  {
     digitalWrite(SAG1,HIGH);

     digitalWrite(SAG2,LOW);
  }
  else
  {
   digitalWrite(SAG1,LOW);
    
   digitalWrite(SAG2,HIGH);
    deger*= -1;
  }
 analogWrite(PWMA,deger);  
} 
// sol motor sürücü fonksiyonu
void solmotor(int deger)
{
  if (deger>=0)
  {
    digitalWrite(SOL1,HIGH);
    digitalWrite(SOL2,LOW);
    
  }
  else
  {
    digitalWrite(SOL1,LOW);
    digitalWrite(SOL2,HIGH);
    deger *= -1;
  }
  analogWrite(PWMB,deger)
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
    digitalWrite(SOL1,HIGH);
    digitalWrite(SOL2,HIGH);
  }
  if ( sag )
  {
    digitalWrite(SAG1,HIGH);
    digitalWrite(SAG2,HIGH);
    
  }
}
