#ifndef SENSORS_H
#define SENSORS_H

#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include <Arduino.h>
#include <Wire.h>

// Pins
#define CO_pin D0
#define SPW_pin D1
#define INA_pin D2
#define SDA_pin D4
#define SCL_pin D5

// Function declarations
void sensorInitialization();

float getTemp();
float getPres();
float getHumd();
float getCO();
float getMicrophone();
float getCurrent();

#endif