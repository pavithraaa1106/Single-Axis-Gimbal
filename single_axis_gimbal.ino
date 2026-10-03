#include <Wire.h>

int16_t temp, Accx, Accy, Accz, Gyx, Gyy, Gyz;
float Ax;
float Ay;
float Az;
float Gx;
float Gy;
float Gz;
const int MPUAddy = 0x68;


void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
Wire.begin();

int x=1;
Wire.beginTransmission(MPUAddy);
Wire.write(x);
Wire.endTransmission();


}

void loop() {
  // put your main code here, to run repeatedly:
Wire.beginTransmission(MPUAddy);
Wire.write(0x3B);
Wire.endTransmission(false);

Wire.requestFrom(MPUAddy,14);
while(Wire.available()>=14){
  Accx = (Wire.read() <<8) | Wire.read();
  Accy = (Wire.read() <<8) | Wire.read();
  Accz = (Wire.read() <<8) | Wire.read();
  temp = (Wire.read() <<8) | Wire.read();
  Gyx = (Wire.read() <<8) | Wire.read();
  Gyy = (Wire.read() <<8) | Wire.read();
  Gyz = (Wire.read() <<8) | Wire.read();
}

Ax = Accx/16384.0;
Ay = Accy/16384.0;
Az = Accz/16384.0;
Gx = Gyx/16384.0;
Gy = Gyy/16384.0;
Gz = Gyz/16384.0;

Serial.print("Ax value: ");
Serial.print(Ax, 4);
Serial.print("   ");

Serial.print("Ay value: ");
Serial.print(Ay, 4);
Serial.print("   ");

Serial.print("Az value: ");
Serial.println(Az, 4);


delay(50);
}
