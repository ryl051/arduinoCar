#include "SR04.h"
#include <Servo.h>
#define TRIG_PIN A5 
#define ECHO_PIN A4 
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
Servo myservo;  
int pos = 0;    
int frontDist = 0;
int leftDist = 0;
int rightDist = 0;
int leftMotor = 5;
int rightMotor = 6;
int leftMotorForwards = 3;
int rightMotorForwards = 4;
int leftMotorBackwards = 2;
int rightMotorBackwards = 7;
int buzzer = A3;

//left and right leds for avoiding obstacles
int leftLedAvoid = 12; //left led for avoiding obstacles
int rightLedFollowLine = 13;

//left and right leds for following black line racetrack
int leftLedFollowLine = 11;
int rightLedAvoid = 8;

int leftLight  = 11;
int Light_Front = 12;
int leftRight = 13;
int headlight   = A1;
int motor       = A2;

unsigned char control; //stores control value
unsigned char valueL; //stores left motor speed
unsigned char valueR; //stores right motor speed

void setup() {
  pinMode(leftMotor,OUTPUT);
  pinMode(rightMotor,OUTPUT);
  pinMode(leftMotorForwards,OUTPUT);
  pinMode(rightMotorForwards,OUTPUT);
  pinMode(leftMotorBackwards,OUTPUT);
  pinMode(rightMotorBackwards,OUTPUT);
  pinMode(buzzer,OUTPUT);
  digitalWrite(buzzer,HIGH); 
  pinMode(headlight,OUTPUT);
  pinMode(motor,OUTPUT);
  pinMode(leftLedFollowLine,INPUT);
  pinMode(rightLedAvoid,INPUT);
  pinMode(leftLedAvoid,INPUT);
  pinMode(rightLedFollowLine,INPUT);

 analogWrite(leftMotor,130);
 analogWrite(rightMotor,130);
 
 myservo.attach(9);  
 myservo.write(90);
 delay(65); //wait for servo to reach specified angle

 Serial.begin(9600); 
}


void forward()
{
   digitalWrite(leftMotorForwards,HIGH);
   digitalWrite(leftMotorBackwards,LOW); 
   digitalWrite(rightMotorForwards,HIGH);
   digitalWrite(rightMotorBackwards,LOW);  
  }

void reverse()
{
   digitalWrite(leftMotorForwards,LOW); 
   digitalWrite(leftMotorBackwards,HIGH); 
   digitalWrite(rightMotorForwards,LOW); 
   digitalWrite(rightMotorBackwards,HIGH); 
  }

void left()
{
   digitalWrite(leftMotorForwards,LOW); 
   digitalWrite(leftMotorBackwards,LOW); 
   digitalWrite(rightMotorForwards,HIGH);  
   digitalWrite(rightMotorBackwards,LOW);  
  }


void right()
{
   digitalWrite(leftMotorForwards,HIGH);  
   digitalWrite(leftMotorBackwards,LOW); 
   digitalWrite(rightMotorForwards,LOW); 
   digitalWrite(rightMotorBackwards,LOW);     
  }

void stop()
{
   digitalWrite(leftMotorForwards,LOW); 
   digitalWrite(leftMotorBackwards,LOW); 
   digitalWrite(rightMotorForwards,LOW);  
   digitalWrite(rightMotorBackwards,LOW);
  }


void leftFast()
{
   digitalWrite(leftMotorForwards,LOW);  
   digitalWrite(leftMotorBackwards,HIGH); 
   digitalWrite(rightMotorForwards,HIGH);  
   digitalWrite(rightMotorBackwards,LOW); 
  }

void rightFast()
{
   digitalWrite(leftMotorForwards,HIGH);  
   digitalWrite(leftMotorBackwards,LOW); 
   digitalWrite(rightMotorForwards,LOW);  
   digitalWrite(rightMotorBackwards,HIGH); 
  }

//detection front, left, and right with ultrasonic sensor
void detectFront()
{
  myservo.write(90);
  delay(65);
  frontDist = sr04.Distance();
  }
void detectLeft()
{
  myservo.write(180);
  delay(300);
  leftDist = sr04.Distance();
  }
void detectRight()
{
  myservo.write(0);
  delay(300);
  rightDist = sr04.Distance();
  }

//following feature with ultrasonic sensor
void ultrasonic()
{
    analogWrite(leftMotor,130);
    analogWrite(rightMotor,130);
   detectFront();
   if(frontDist < 35)
   {
      stop(); delay(100);
      reverse(); delay(300);
      stop(); delay(100);
      detectLeft();
      detectRight();
      if(leftDist < 35 && rightDist < 35) {
          //turns around 180 degrees
          leftFast();
          delay(600);
        }
        else if(leftDist > rightDist){
            left(); delay(500);
            stop(); delay(100);
          }
          else{
             right(); delay(500);
             stop(); delay(100);
            }
    }
    else{
        forward();
      } 
  }
//function for following blackline racetrack
void blackLine()
{
   
   analogWrite(leftMotor,150);
   analogWrite(rightMotor,150);

  if(digitalRead(leftLedFollowLine) == HIGH && digitalRead(rightLedAvoid) == HIGH) {
    forward();
    }
  else {
     if(digitalRead(leftLedFollowLine) == HIGH && digitalRead(rightLedAvoid) == LOW){
       left();
      }
     if(digitalRead(leftLedFollowLine) == LOW && digitalRead(rightLedAvoid) == HIGH) {
       right();
      }

     if(digitalRead(leftLedFollowLine) == LOW && digitalRead(rightLedAvoid) == LOW){
       reverse();
      }
    }  
  }
//using infrared sensors to avoid obstacles
void IRAvoid(){
  //LOW = detects something in front, HIGH = nothing in front
  if(digitalRead(leftLedAvoid) == HIGH && digitalRead(rightLedFollowLine) == HIGH){
  
    analogWrite(leftMotor,150);
    analogWrite(rightMotor,150);
    forward();
    }
   if(digitalRead(leftLedAvoid) == HIGH && digitalRead(rightLedFollowLine) == LOW){
     analogWrite(leftMotor,120);
     left();
     delay(40);）
    }
   if(digitalRead(leftLedAvoid) == LOW && digitalRead(rightLedFollowLine) == HIGH) {
     analogWrite(rightMotor,120);
     right();
     delay(40)
    }

   if(digitalRead(leftLedAvoid) == LOW && digitalRead(rightLedFollowLine) == LOW){
     analogWrite(leftMotor,150);
     analogWrite(rightMotor,150);
     stop();
     delay(200);
     reverse();
     delay(300);
     rightFast();
     delay(250);
    }  
  }
//seeks light and follows
void lightSeek() {
    //LOW = detects light source, HIGH = does not detect light source
    if(digitalRead(Light_Front) == LOW && digitalRead(leftLight) == HIGH && digitalRead(leftRight) == HIGH) {
      analogWrite(leftMotor,170);
      analogWrite(rightMotor,170);
      forward();
    }
    else if(digitalRead(Light_Front) == LOW && digitalRead(leftLight) == LOW && digitalRead(leftRight) == HIGH) {
     
      analogWrite(leftMotor,130);
      analogWrite(rightMotor,200);
      forward();
    }
    else if(digitalRead(Light_Front) == LOW && digitalRead(leftLight) == HIGH && digitalRead(leftRight) == LOW){
  
      analogWrite(leftMotor,200);
      analogWrite(rightMotor,130);
      forward();
    }
    else if(digitalRead(Light_Front) == HIGH && digitalRead(leftLight) == HIGH && digitalRead(leftRight) == LOW){
      analogWrite(leftMotor,170);
      analogWrite(rightMotor,170);
      right();
    }
    else if(digitalRead(Light_Front) == LOW && digitalRead(leftLight) == HIGH && digitalRead(leftRight) == HIGH){
      analogWrite(leftMotor,170);
      analogWrite(rightMotor,170);
      left();
    }
    else{
      stop();
    }
  }

//infrared tracking
void IRTracking()
{
   analogWrite(leftMotor,120);
   analogWrite(rightMotor,120);
    
  if(digitalRead(leftLedAvoid) == LOW && digitalRead(rightLedFollowLine) == LOW) {
   forward();
   }
   if(digitalRead(leftLedAvoid) == HIGH && digitalRead(rightLedFollowLine) == LOW){
   right();
   }
   if(digitalRead(leftLedAvoid) == LOW && digitalRead(rightLedFollowLine) == HIGH) {
   left();
   }
   if(digitalRead(leftLedAvoid) == HIGH && digitalRead(rightLedFollowLine) == HIGH) {
   stop();
   }  
  }
  
//sideways 
void SideRun()
{
   analogWrite(leftMotor,120);
   analogWrite(rightMotor,120);
    
    if(digitalRead(leftLedAvoid) == LOW ){
     right();
    }
    else {
       forward();
    } 
  }
void loop() {
  
    switch(control)
    {
      case 0X02:  forward();  break;  
      case 0X03:  reverse(); break; 
      case 0X04:  left();     break;  
      case 0X05:  right();    break;
      case 0X01:  stop();       break;
      case 0X06:  leftFast();   break;
      case 0X07:  rightFast();  break;
      case 0X08:  digitalWrite(buzzer,LOW);    break;
      case 0X09:  digitalWrite(buzzer,HIGH);   break;
      case 0X0A:  myservo.write(180);  delay(60);   break;
      case 0X0B:  myservo.write(90);  delay(60);   break;
      case 0X0C:  myservo.write(0);  delay(60);   break;
      case 0X0D:  digitalWrite(motor,LOW);   break;
      case 0X0E:  digitalWrite(motor,HIGH);  break;
      case 0X10:  digitalWrite(headlight,LOW);   break;
      case 0X0F:  digitalWrite(headlight,HIGH);   break;
      case 0X11:  ultrasonic();   break;
      case 0X12:  blackLine();    break;
      case 0X13:  IRAvoid();      break;
      case 0X14:  lightSeek(); break;
      case 0X15:  IRTracking();   break;
      case 0X16:  SideRun();      break;
    }
}

