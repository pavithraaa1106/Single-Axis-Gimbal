#include <Wire.h>
#include <Servo.h>

Servo MyServo;
int servopin = 9;
int servopos1;
int servopos2;
int servopos3;
int serialaskpos=0;

int16_t temp, Accx, Accy, Accz, Gyx, Gyy, Gyz;
float Ax;
float Ay;
float Az;
float Gx;
float Gy;
float Gz;
float gyroaccArray[] = {Ax,Ay,Az,Gx,Gy,Gz};
const int MPUAddy = 0x68;
int j;

float GxOffset;
float GyOffset;
float GzOffset;


int yawg = 0;

float gz;
int tstart = millis();


void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
Wire.begin();
MyServo.attach(7);

Wire.beginTransmission(MPUAddy);
Wire.write(0x6B);
Wire.write(0x00);
Wire.endTransmission(); //wakes and tests transmission

Serial.println("MPU is connected!");

long Gxsum = 0;
long Gysum = 0;
long Gzsum = 0;


for(j=0;j<=1000;j++){
Wire.beginTransmission(MPUAddy);
Wire.write(0x3B);
Wire.endTransmission(false); //keeps connection going

Wire.requestFrom(MPUAddy,14,true);
int16_t Accx = (Wire.read()<<8)|Wire.read();
int16_t Accy = (Wire.read()<<8)|Wire.read();
int16_t Accz = (Wire.read()<<8)|Wire.read();
int16_t temp = (Wire.read()<<8)|Wire.read();

int16_t Gyx = (Wire.read()<<8)|Wire.read();
int16_t Gyy = (Wire.read()<<8)|Wire.read();
int16_t Gyz = (Wire.read()<<8)|Wire.read();


Gxsum = Gxsum + Gyx;
Gysum = Gysum + Gyy;
Gzsum = Gzsum + Gyz;

delay(3);
}

GxOffset = Gxsum/1000;
GyOffset = Gysum/1000;
GzOffset = Gzsum/1000;

Serial.println("Gyro caibrated!");

tstart = millis();
}


void loop() {
  // put your main code here, to run repeatedly:

//getting data from mpu
Wire.beginTransmission(MPUAddy);
Wire.write(0x3B);
Wire.endTransmission(false);

Wire.requestFrom(MPUAddy,14,true); //requesting data from mpu
if (Wire.available()>=14){
  Accx = (Wire.read() <<8) | Wire.read(); // reads data. can only get 8 bits at a time, so it grabs 8 bit and then shifts it over and then grabs another 8 to get full 16 bit value
  Accy = (Wire.read() <<8) | Wire.read();
  Accz = (Wire.read() <<8) | Wire.read();
  temp = (Wire.read() <<8) | Wire.read();
  Gyx = (Wire.read() <<8) | Wire.read();
  Gyy = (Wire.read() <<8) | Wire.read();
  Gyz = (Wire.read() <<8) | Wire.read();
}

Ax = Accx/16384.0; //converting 16 bit value to g force; 16384 value = 1g, and sensitivity is +-2g; 1g = 9.8 m/s^2
Ay = Accy/16384.0;
Az = Accz/16384.0;

Gx = (Gyx - GxOffset)/131.0;
Gy = (Gyy - GyOffset)/131.0;
Gz = (Gyz - GzOffset)/131.0;

gz = -Gz;

//checkpoint:
//Now Ax, Ay, Az, is in g force
//Now Gx, Gy, Gz is in degrees per second
//Next step, figure out math to convert into yaw angle


yawg = yawg + (millis()-tstart)/1000.0*gz;

tstart = millis();

//servo stuff
if (Serial.available()>0){
serialaskpos = Serial.parseInt();
}

servopos1 = -yawg;
servopos2 = 90 - servopos1;
servopos3 = servopos2 + serialaskpos;

if (servopos1<0){
  servopos1 = 0;
}
if (servopos1>180){
  servopos1=180;
}

MyServo.write(servopos3);
delay(10);

while (Serial.available()>0){
  Serial.read();
}
//print stuff
Serial.print("Ax value: ");
Serial.print(Ax, 2);
Serial.print("   ");

Serial.print("Ay value: ");
Serial.print(Ay, 2);
Serial.print("   ");

Serial.print("Az value: ");
Serial.print(Az, 2);
Serial.print("   ");

Serial.print("Gx value: ");
Serial.print(Gx, 3);
Serial.print("   ");

Serial.print("Gy value: ");
Serial.print(Gy, 3);
Serial.print("   ");

Serial.print("Gz value: ");
Serial.print(gz, 3);
Serial.print("   ");

Serial.print("yaw rotation angle: ");
Serial.print(yawg);
Serial.println();


delay(25);
}
