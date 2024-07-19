#include <ps5Controller.h>
#include <Arduino.h>
#include <ESP32_Servo.h>

//////////////////////Base controll//////////////////////
Servo PA, PB, PC, PD;
float theta = 0;
int ay = 0, ay1 = 0;
float magnitude = 0;
int pa = 0, pb = 0, pc = 0, pd = 0;
float a = 0, b = 0, c = 0, d = 0, abs_a = 0, abs_b = 0, abs_c = 0, abs_d = 0;
int x = 0, y = 0;
int rotational_pwm = 0;
float Sin = 0 , Cos = 0, Max = 0;
int P;

/////////////////////Viper motor///////////////
int in1 = 13, pwm = 14, in2 = 12;

///////////////////////Pneumatics//////////////
int IN1 = 26;

/////////////////////Gripper Servo////////
Servo grip;

//////////////////////BLDC////////////////
Servo BLDC1, BLDC2;



void setup()
{
  ps5.begin("E8:47:3A:55:1F:5A");
  Serial.println("Ready.");
  Serial.begin(921600);

 PA.attach(18);
 PB.attach(5);
 PC.attach(15);
 PD.attach(2);

  ////////////////////////////Gripper Servo///////////
  grip.attach(27);

  ////////////////////////BLDC Throwing////////////////////
  BLDC1.attach(25);
  BLDC1.write(30);
  delay(3000);
  BLDC2.attach(33);
  BLDC2.write(30);
  delay(3000);

  ///////////////////////Viper motor////////////////
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(pwm, OUTPUT);

  ///////////////////Pneumatics////////////////
  pinMode(IN1, OUTPUT);

}

void loop()
{
  // Below has all accessible outputs from the controller
  if (ps5.isConnected()) {


    if ( ps5.LStickX() || ps5.LStickY() || ps5.RStickX() || ps5.RStickY() ) {
      if ( (ps5.LStickX() > 25)) {
        x = ps5.LStickX();
        x = map(ps5.LStickX(), 25, 127, 0, 90);
      }
      else if ( (ps5.LStickX() < -25)) {
        x = ps5.LStickX();
        x = map(ps5.LStickX(), -25, -127, 0, -90);
      }
      if ( (ps5.LStickY() > 25)) {
        y = ps5.LStickY();
        y = map(ps5.LStickY(), 25, 127, 0, 90);
      }
      else if ( (ps5.LStickY() < -25)) {
        y = ps5.LStickY();
        y = map(ps5.LStickY(), -25, -127, 0, -90);
      }



      if ( (ps5.RStickX() > 15)) {
        rotational_pwm = ps5.RStickX();
        rotational_pwm = map(ps5.RStickX(), 15, 120, 0, 30);
      }
      else if ( (ps5.RStickX() < -15)) {
        rotational_pwm = ps5.RStickX();
        rotational_pwm = map(ps5.RStickX(), -15, -120, 0, -30);
      }
      else {
        rotational_pwm = 0;
      }
      if (( (ps5.RStickX() < 15)) && (ps5.RStickX() > -15) )
      {
        rotational_pwm = 0;
      }
      if (( (ps5.LStickX() < 25)) && ( (ps5.LStickX() > -25)) )
      {
        x = 0;
      }
      if (( (ps5.LStickY() < 25)) && ( (ps5.LStickY() > -25)) )
      {
        y = 0;
      }

      Serial.print("Left Stick x at ");
      Serial.println(x);
      Serial.print("Left Stick y at ");
      Serial.println(y);
      magnitude = sqrt(sq(x) + sq(y));
      theta = atan2(y, x);
      b = magnitude * (cos(theta + (PI / 4)));
      a = magnitude * (sin(theta + (PI / 4)));
      Serial.println(y);
      pa = map(a, -128, 128, 180, 0) - rotational_pwm;
      pc = map(a, -128, 128, 0, 180) - rotational_pwm;
      pb = map(b, -128, 128, 0, 180) + rotational_pwm;
      pd = map(b, -128, 128, 180, 0) + rotational_pwm;


      Serial.print("\rpa :");
      Serial.println(pa);
      Serial.print("pb :");
      Serial.println(pb);
      Serial.print("pc :");
      Serial.println(pc);
      Serial.print("pd :");
      Serial.println(pd);

      myservo.write(pa);
      myservo1.write(pb);
      myservo2.write(pc);
      myservo3.write(pd);


      if ( ps5.Triangle() )
      { /////////////////////////////BLDC SPEED 1
        Serial.println("Triangle Button");
        BLDC1.write(100);
        BLDC2.write(100);
        delay(100);

      }
      if ( ps5.Circle())
      { ///////////////////////////////BLDC STOP
        Serial.println("Circle Button");
        BLDC1.write(30);
        BLDC2.write(30);
        delay(10);
      }
      if ( ps5.Cross() )
      { //////////////////////////////BLDC SPEED 2
        Serial.println("Cross Button");
        BLDC1.write(115);
        BLDC2.write(115);
        delay(10);
      }
      if ( ps5.Square() )
      { ////////////////////////////////BLDC SPEED 3
        Serial.println("Square Button");
        BLDC1.write(80);
        BLDC2.write(80);
        delay(300);
        digitalWrite(IN1, 1);
        delay(400);
        digitalWrite(IN1, 0);
        delay(40);
      }
      if ( ps5.Left() )
      {
        Serial.println("Left Button");
        BLDC1.write(78);///up bldc
        BLDC2.write(78);
        delay(300);
        digitalWrite(IN1, 1);
        delay(400);
        digitalWrite(IN1, 0);
        delay(40);

      }
      if ( ps5.Right()) {
        Serial.println("right  Button");
        BLDC1.write(75);
        BLDC2.write(75);
        delay(230);
        digitalWrite(IN1, 1);
        delay(400);
        digitalWrite(IN1, 0);
        delay(40);


      }
      if ( ps5.Down() ) {
        Serial.println("down Button");
        BLDC1.write(71);
        BLDC2.write(71);
        delay(150);
        digitalWrite(IN1, 1);
        delay(400);
        digitalWrite(IN1, 0);
        delay(40);


      }

      if ( ps5.L1()) {////////////////Servo gripper open
        Serial.println("l1 Button");
        grip.write(30);
        delay(10);
      }
      if ( ps5.R1() ) {/////////////////Servo gripper Closed
        Serial.println("r1 Button");
        grip.write(0);
        delay(10);
      }

      if ( ps5.L2() ) {///////////////////Viper motor down//////////////
        Serial.printf("L2 button at %d\n", ps5.L2Value());
        if ((ps5.L2Value()) < 30) {
          ay = 0;
        }
        else {
          ay = map(ps5.L2Value(), 30, 255, 0, 255);
        }
        analogWrite(pwm, ay);
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
        Serial.println(ay);
      }
      if ( ps5.R2()) {////////////////////////Viper motor Up///////////////

        if ((ps5.R2Value()) < 30)
        {
          ay1 = 0;

        }
        else {
          ay1 = map((ps5.R2Value()), 30, 255, 0, 255);

        }
        Serial.printf("R2 button at %d\n", ps5.R2Value());
        analogWrite(pwm, ay1);
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
        Serial.print("ay1 pwm ");
        Serial.println(ay1);
      }

      if ( ps5.Up()) { ////////////////////Pneumatics Feed///////////////
        Serial.println("Up Button");
        digitalWrite(IN1, 1);
        delay(400);
        digitalWrite(IN1, 0);
        delay(40);
      }

      if (ps5.Share())
      { Serial.println("Share Button");
        digitalWrite(IN1, 1);
        delay(40);
      }
      if (ps5.Options()) 
      {Serial.println("Options Button");
        digitalWrite(IN1, 0);
        delay(40);
      }

    }
  }
}
