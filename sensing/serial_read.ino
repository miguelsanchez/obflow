#include <math.h>
const int pinAdc = A0;
int noise = 0;
const int B = 4250;               
const int R0 = 100000;            
const int pinTempSensor = A1;     
const int pinRel = 4;

void setup()
{
  Serial.begin(115200);
  pinMode(pinRel, OUTPUT);
}

void loop()
{
  long sum = 0;
    for(int i=0; i<32; i++)
    {
        sum += analogRead(pinAdc);
    }

    sum >>= 5;

    noise = map(sum, 0, 796, 1, 6);
    
    

    int a = analogRead(pinTempSensor);

    float R = 1023.0/a-1.0;
    R = R0*R;

    float temperature = 1.0/(log(R/R0)/B+1/298.15)-273.15;
     if (temperature >= 22) {
      digitalWrite(pinRel, HIGH);
     }
     else {
      digitalWrite(pinRel, LOW);
     }

    Serial.println(noise);
    delay(300);
}
