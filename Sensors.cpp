#include "Sensors.hpp"
 
bool bme_ok = true;

float bme_temp = 0;
float bme_pres = 0;
float bme_humd = 0;
float CO_data = 0;
float SPW_data = 0;
float INA_data = 0;

Adafruit_BME680 bme;

void sensorInitialization() {
  Serial.begin(115200);
  pinMode(D0, INPUT);
  pinMode(D1, INPUT);
  pinMode(D2, INPUT);
  Wire.begin(SDA_pin, SCL_pin);

  if(!bme.begin(0x77)){
    Serial.println("BME sensor not found!");
    bme_ok = false;
  } else {
    Serial.println("BME sensor started");
    bme.setTemperatureOversampling(BME680_OS_8X);
    bme.setHumidityOversampling(BME680_OS_2X);
    bme.setPressureOversampling(BME680_OS_4X);
    bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
    bme.setGasHeater(320, 150);
  }
}

float getTemp(){
  if(bme_ok){
    if(bme.performReading()){
      return bme_temp = bme.temperature;
    }
  }
}
float getPres(){
  if(bme_ok){
    if(bme.performReading()){
      return bme_pres = bme.pressure/100.00;
    }
  }
}
float getHumd(){
  if(bme_ok){
    if(bme.performReading()){
      return bme_humd = bme.humidity;
    }
  }
}

float getCO(){
  return CO_data = analogRead(CO_pin);
}

float getMicrophone(){
  return SPW_data = analogRead(SPW_pin);
}

float getCurrent(){
  return INA_data = analogRead(INA_pin);
}
