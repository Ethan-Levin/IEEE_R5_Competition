#include "ThingSpeak.h"
#include "Sensors.hpp"
#include <WiFi.h>

#define MAX_CHANNELS 6
#define UPDATE_DELAY 20000

// originally had it be linked lists now just static and add to sum
typedef struct{
  float sum;
  int count;
} sensor_accumulator_t;

typedef struct{
  sensor_accumulator_t temp;
  sensor_accumulator_t humidity;
  sensor_accumulator_t pressure;
  sensor_accumulator_t co;
  sensor_accumulator_t microphone;
  sensor_accumulator_t current;
  float avg_data[MAX_CHANNELS];
} sensor_data_t;


/* Wifi Setup */
const char* ssid = "CapstoneWifi";
const char* password = "RuleNumber9";
/* ThingSpeak Setup */
WiFiClient client;  
unsigned long channelID = 3317303;
const char* writeAPIKey = "JUOGXUJGD4K3F52Q";
/* Time */
unsigned long currTime = millis();
unsigned long prevTime = millis();
/* Define a global sensor data struct*/
sensor_data_t sensor_data;

void resetSensorData(){
  for(int i =0; i<MAX_CHANNELS; i++){
    sensor_data.avg_data[i]=0;
  }
  sensor_data.temp = {0, 0};
  sensor_data.humidity = {0, 0};
  sensor_data.pressure = {0, 0};
  sensor_data.co = {0, 0};
  sensor_data.microphone = {0, 0};
  sensor_data.current = {0, 0};
}

void ThingSpeakSendData(){
  for(int i =0; i<MAX_CHANNELS; i++){
    ThingSpeak.setField(i+1, sensor_data.avg_data[i]);//array starts at 0 but channel starts at 1
  }

    int response = ThingSpeak.writeFields(channelID, writeAPIKey);

    if (response == 200) {
      Serial.println("Data sent!");
    } 
    else {
      Serial.print("Error: ");
      Serial.println(response);
    }
  
}

float computeAverage(sensor_accumulator_t *sensor){
  if(sensor->count == 0){
    return 0; //prevents divide by 0 error;
  }
  return sensor->sum / sensor->count; //return the average
}

void sensorUpdateAverage(){
  sensor_data.avg_data[0] = computeAverage(&sensor_data.temp);
  sensor_data.avg_data[1] = computeAverage(&sensor_data.humidity);
  sensor_data.avg_data[2] = computeAverage(&sensor_data.pressure);
  sensor_data.avg_data[3] = computeAverage(&sensor_data.co);
  sensor_data.avg_data[4] = computeAverage(&sensor_data.microphone);
  sensor_data.avg_data[5] = computeAverage(&sensor_data.current);
}

void printAverageSensorData(){
  Serial.print("Temp: "); Serial.print(sensor_data.avg_data[0]); Serial.print(" C over "); Serial.print(sensor_data.temp.count); Serial.println(" samples.");
  Serial.print("Humidity: "); Serial.print(sensor_data.avg_data[1]); Serial.print(" rH over "); Serial.print(sensor_data.humidity.count); Serial.println(" samples.");
  Serial.print("Pressure: "); Serial.print(sensor_data.avg_data[2]); Serial.print(" hPA over "); Serial.print(sensor_data.pressure.count); Serial.println(" samples.");
  Serial.print("CO: "); Serial.print(sensor_data.avg_data[3]); Serial.print(" ppM over "); Serial.print(sensor_data.co.count); Serial.println(" samples.");
  Serial.print("Microphone: "); Serial.print(sensor_data.avg_data[4]); Serial.print(" mV over "); Serial.print(sensor_data.microphone.count); Serial.println(" samples.");
  Serial.print("Current: "); Serial.print(sensor_data.avg_data[5]); Serial.print(" mA over "); Serial.print(sensor_data.current.count); Serial.println(" samples.");
}

void getSensorData(){
  // Temperature
  sensor_data.temp.sum += getTemp();
  sensor_data.temp.count++;

  // Humidity
  sensor_data.humidity.sum += getHumd();
  sensor_data.humidity.count++;

  // Pressure
  sensor_data.pressure.sum += getPres();
  sensor_data.pressure.count++;

  // CO
  sensor_data.co.sum += getCO();
  sensor_data.co.count++;

  // Microphone
  sensor_data.microphone.sum += getMicrophone();
  sensor_data.microphone.count++;

  // Current
  sensor_data.current.sum += getCurrent();
  sensor_data.current.count++;
}


void setup() {
  Serial.begin(115200);

  /* Setup Wifi*/
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  /* Wait for Wifi connection */
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected!");

  ThingSpeak.begin(client);

  sensorInitialization();
  /* Reset sensor data just to be safe and ensure no null initialization*/
  resetSensorData();
}

void loop() {
  // put your main code here, to run repeatedly:
  currTime = millis();
  if(currTime - prevTime > UPDATE_DELAY){
    sensorUpdateAverage();
    ThingSpeakSendData();
    printAverageSensorData();
    resetSensorData();
    prevTime = currTime;
  }
  else{
    getSensorData();
  }
}
