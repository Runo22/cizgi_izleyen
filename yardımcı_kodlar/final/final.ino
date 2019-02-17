#include <QTRSensors.h>
#define enA 2
#define in1 3
#define in2 4
#define enB 8
#define in3 6
#define in4 7
#define NUM_SENSORS             8  // number of sensors used
#define NUM_SAMPLES_PER_SENSOR  4  // average 4 analog samples per sensor reading
#define EMITTER_PIN             9  // emitter is controlled by digital pin 2


int hiz=70;
boolean zemin=1;
int hata;
int turev;

float KP = 0.01;
float KD = 0.1;

//********alt 2 sısra unsigned int olucak ******************
unsigned int pozisyon;
unsigned int sensor[NUM_SENSORS];
int fark;
int son_hata;
int hedef=3500;

int i;

// sensors 0 through 5 are connected to analog inputs 0 through 5, respectively
QTRSensorsAnalog qtra((unsigned char[]) {0, 1, 2, 3, 4, 5, 6, 7 },
  NUM_SENSORS, NUM_SAMPLES_PER_SENSOR, EMITTER_PIN);

  //-----------bu kullanılmıyor yukarıda sensor olarak aynısı var-----------
unsigned int sensorValues[NUM_SENSORS];


void setup()
{
  delay(500);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);    // turn on Arduino's LED to indicate we are in calibration mode
  for (int i = 0; i < 400; i++)  // make the calibration take about 10 seconds
  {
    qtra.calibrate();       // reads all sensors 10 times at 2.5 ms per six sensors (i.e. ~25 ms per call)
  }
  digitalWrite(13, LOW);     // turn off Arduino's LED to indicate we are through with calibration

  
}


void loop()
{
//(unsigned int *sensorValues, unsigned char readMode = QTR_EMITTERS_ON, unsigned char whiteLine = 0)
 pozisyon=qtra.readLine (sensor , true ,zemin);
 hata=pozisyon-hedef;

//****************Burda son hata tanımlı değil***************************
 turev = hata-son_hata;

 son_hata = hata;

 fark = (hata*KP) + (turev*KD);

 if (fark < 0){
 motor(hiz , hiz+fark);
 }
 else{
 motor(hiz-fark , hiz);
 }
}

void sagmotor(int deger)
{
  Serial.begin(9600);
   if(deger >= 0)
   {
    digitalWrite(in4, LOW);
    deger *= -1;
   }
   else
   {
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH); 
   }
   analogWrite(enA,deger);
}

void solmotor(int deger)
{
   if(deger <= 0)
   {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
   }
   else
   {
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    deger *= -1;
   }
   analogWrite(enB,deger);
}
void motor(int sol, int sag)
{
  solmotor(sol);
  sagmotor(sag);
}

void fren(boolean sol, boolean sag)
{

  if(sol)
  {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, HIGH);
  }
  if(sag)
  {
    digitalWrite(in3, HIGH);
    digitalWrite(in4, HIGH);
  }

  
}
