const int pinAdc = A0;
int noise = 0;

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  long sum = 0;
    for(int i=0; i<32; i++)
    {
        sum += analogRead(pinAdc);
    }

    sum >>= 5;

    noise = map(sum, 0, 438, 1, 6);
    
    Serial.println(noise);
    delay(5000);
}

