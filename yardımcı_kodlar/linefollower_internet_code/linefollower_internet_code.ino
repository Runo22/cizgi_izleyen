#include <QTRSensors.h>

/******************************
 * This Code is for the robot with 2 switches
 * servo is the left motor
 * servo2 os the right motor
 *****************************/


#include <Servo.h> // library 
#include <NewPing.h>
/*
 * All variables defined below
 */


#define TRIGGER_PIN  12  
#define ECHO_PIN     11
#define MAX_DISTANCE 200 
#define button 8
#define button2 10
#define NUM_SENSORS 4      // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN  2



NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 

Servo servo;
Servo servo2;             
/**************
 * Sensor 3 = pin7, Sensor 4 = pin5, Sensor 5 = pin4, Sensor 6 = pin3
 */
QTRSensorsRC qtrrc((unsigned char[]) {7,5,4,3} ,NUM_SENSORS, TIMEOUT, EMITTER_PIN); /// Make sure to put the pin# as shown above of the arduino to the sensor within the curly bracket
unsigned int sensorValues[NUM_SENSORS]; // list of sensors

void setup()
{
 delay(500);
 pinMode(13,OUTPUT);
 digitalWrite(13,HIGH);
  Serial.begin(9600);
  servo.attach(6);
  servo2.attach(9);
  pinMode(button,OUTPUT);
  pinMode(button2, OUTPUT);
// Calibration code starts here

  for (int i = 0; i < 400; i++)  // make the calibration take about 10 seconds
  {
    qtrrc.calibrate();       // reads all sensors 10 times at 2500 us per read (i.e. ~25 ms per call)
  }
  digitalWrite(13, LOW);     // turn off Arduino's LED to indicate we are through with calibration

  // print the calibration minimum values measured when emitters were on
  Serial.begin(9600);
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtrrc.calibratedMinimumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  
  // print the calibration maximum values measured when emitters were on
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtrrc.calibratedMaximumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
  delay(1000);

  /// Ends here
 
}

void loop()
{
  digitalWrite(button,HIGH); // always have global variables button high
  digitalWrite(button2,HIGH);
 // Serial.print(digitalRead(button2));
 stop_1();
  
 while(digitalRead(button) == LOW)
  {
    Serial.print("You are inside the first button");
  delay(50);                      
  unsigned int uS = sonar.ping(); 
  //Serial.print("Ping: ");
  signed  int distance = (uS / US_ROUNDTRIP_CM);
    
    Serial.print("Ping: ");
  Serial.print(distance);
    Serial.println("cm");
  forward();
  if(distance > 8)
  {
    stop_1();
    delay(1000);
  reverse();
  delay(2000);
  turnLeft();
  delay(2000);
  
  }
  }


/*****************
 * Checking for light sensor status below
 */
/**************
 * Sensor 3 = pin7, Sensor 4 = pin5, Sensor 5 = pin4, Sensor 6 = pin3
 */
  unsigned int position = qtrrc.readLine(sensorValues);
  // down below is optional: Starts here
  unsigned char i = 0;
int line1 = sensorValues[i=1]; /// Value for sensor 4
int line2 = sensorValues[i=2]; /// Value for sensor 5
int line3 =  sensorValues[i=3]; //Value for sensor 6
int line4 = sensorValues[i]; // Value for sensor 3
// Ends here

/// 
 for (unsigned char i = 0; i < NUM_SENSORS; i++)
 {
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }
  Serial.println(); 
  /// uncommenct below if you want the position(optional)
 // Serial.println(position); 
  delay(250);
  // uncomment below if second button is not fucntioning
//  Serial.println(digitalRead(button2));

/// Down below is just code for my servos, Line follower code ends here

  while(digitalRead(button2) == LOW)
  {
 unsigned int position = qtrrc.readLine(sensorValues);
  // down below is optional: Starts here
  unsigned char i = 0;
int line1 = sensorValues[i=1]; /// Value for sensor 4
int line2 = sensorValues[i=2]; /// Value for sensor 5
int line3 =  sensorValues[i=3]; //Value for sensor 6
int line4 = sensorValues[i=0]; // Value for sensor 3


    
   /* 
    *  Insert code for the second Button
    */
    Serial.println(line1);
    Serial.println("\n");
    Serial.println(line2);
    if(line1  >= 1000 || line2 >= 1000&& digitalRead(button2)== LOW)
    {
      Serial.println("going forward");
      Serial.println("\n");
     forward();
      
    }
    else if(line4 >=1000 && digitalRead(button2) == LOW)
    {
      
     Serial.println("Going right");
    while(line1 != 1000)
    {
      unsigned int position = qtrrc.readLine(sensorValues);
  // down below is optional: Starts here
  unsigned char i = 0;
int line1 = sensorValues[i=1]; /// Value for sensor 4
int line2 = sensorValues[i=2]; /// Value for sensor 5
int line3 =  sensorValues[i=3]; //Value for sensor 6
int line4 = sensorValues[i=0]; // Value for sensor 3

Serial.println(line1);
Serial.println("\n");
     turnRight(); 
     if(line1 == 1000)
     {
      
      break;
      
      }
      
     }
      
     }
     else if(line3 == 1000 && digitalRead(button2) == LOW)
     {
      
      Serial.println("Going left");
      while(line1 != 1000)
      {
        unsigned int position = qtrrc.readLine(sensorValues);
  // down below is optional: Starts here
  unsigned char i = 0;
int line1 = sensorValues[i=1]; /// Value for sensor 4
int line2 = sensorValues[i=2]; /// Value for sensor 5
int line3 =  sensorValues[i=3]; //Value for sensor 6
int line4 = sensorValues[i=0]; // Value for sensor 3
       turnLeft(); 

  if(line1 == 1000)
  {
    
   break; 
    
   }

        
       }
      
    }
  
     




   }                                                          


}

void forward()
{
  servo.write(360); // left motor
  servo2.writeMicroseconds(360); //// right motor

 }
 void reverse()
 {
  servo.write(-360);
  servo2.write(360);
  
 }

 void stop_1()
 {
  servo.write(93); 
  servo2.write(92.36); 
 }
 void turnLeft()
 {
  servo.write(93);
  servo2.writeMicroseconds(360);
 }

 void turnRight()
 {
  servo2.write(93.36);
    servo.write(360);
 }

