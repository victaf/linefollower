
#include <QTRSensors.h>

QTRSensors qtr;

const uint8_t SensorCount = 6;
uint16_t sensorValues[SensorCount];

void setup()
{
  // configure the sensors
  qtr.setTypeAnalog();
  qtr.setSensorPins((const uint8_t[]){A0, A1, A2, A3, A4, A5}, SensorCount);

  Serial.begin(9600);
}


void loop(){
  int output[6] = {0,0,0,0,0,0};
  qtr.read(sensorValues);
  int max = 0;
  int index = 0;
  bool stop = true;
  for (uint8_t i = 0; i < SensorCount; i++)
  {
    int value = sensorValues[i];
    if(value>max){
      index = i;
      max = value;
    }
    if(value<850){
      stop = false;
    }
  }
  for (uint8_t i = 0; i < SensorCount; i++){
    if(stop){
      output[i] = 1;
    }else if(index == i){
      output[i] = 1;
    }else{
      output[i] = 0;
    }
  }
  Serial.print("[");
  for (uint8_t i = 0; i < SensorCount; i++){
    Serial.print(output[i]);
    //Serial.print(sensorValues[i]);
    Serial.print(", ");
  }
  Serial.println("]");

  
}
