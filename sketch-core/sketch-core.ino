/***********************************************
* tiempodesiembra.es
* child controller 0.0.1
**********************************************/

const char VERSION = {"0.0.1"};

/***********************************************
* Device setup
**********************************************/

/* Tiempo de siembra */
// uuid - https://www.uuidgenerator.net/
const char DEVICE_UNIT_UUID   = {"8d1b208e-cadb-4767-b199-cda6cbed3b0d"};
const char DEVICE_LINK_CRYPTO = {"960e4ce7-53e2-48ac-b7bb-30ef00f2d20d"};

/* Enable or disable */                               // Enable or disable
const boolean MOISTURE_SENSOR_ENABLED         = true; // Moisture
const boolean TEMPERATURE_SENSOR_ENABLED      = true; // Temperature

/* PIN setup */                                       // PIN setup
const int MOISTURE_SENSOR_PIN                 = 0;     // Moisture sensor
const int TEMPERATURE_SENSOR_PIN              = 1;     // Temperature sensor

const int RF_RX_PIN                           = 2;     // RF rx
const int RF_TX_PIN                           = 3;     // RF tx

/***********************************************
* Custom values
**********************************************/

/* Moisture */
const int MOISTURE_LEVELS[9] = {
  0, 1, 2, // low
  3, 4, 5, // med
  6, 7, 8  // high
 };

/* Temperature */
const int TEMPERATURE_LEVELS[9] = {
  0, 1, 2, // low
  3, 4, 5, // med
  6, 7, 8  // high
 };

/***********************************************
* 
* 
*    Do NOT change anything from this point
* 
* 
**********************************************/

/***********************************************
* Libraries
**********************************************/

#include <ArduinoJson.h>

/***********************************************
* System definitions
**********************************************/

/* Loop */
const int LOOP_DELAY = 1 * 1000; // <seconds> * <ms>

/* Levels */
const char SENSOR_LEVELS[9] = {'lowl', 'low', 'lowg', 'medl', 'med', 'medh', 'higl', 'hig', 'higg'};

/***********************************************
* Functions
**********************************************/

/*
 * Returns the level for a given type and value
 */
char getLevel(char type[3], int value) {
  int level[9];
  int biggerIndex = 0;
  
  if (type == "MOI") {
    level[9] = MOISTURE_LEVELS;
  };
  
  if (type == "TEM") {
    level[9] = TEMPERATURE_LEVELS;
  };

  for (int i = 0; i <= 8; i ++) {
     if (level[i] > value) {
      biggerIndex = i;
     }
  }

  return SENSOR_LEVELS[biggerIndex];
};

char* readSensors() {

  JsonObject& root = StaticJsonBuffer<200> jsonBuffer;
  
  if (MOISTURE_SENSOR_ENABLED) {
    root['MOI'] = getLevel(
      "MOI",
      analogRead(MOISTURE_SENSOR_PIN)
     );
  }
  
  if (TEMPERATURE_SENSOR_ENABLED) {
    root['TEM'] = getLevel(
      "MOI",
      analogRead(TEMPERATURE_SENSOR_PIN)
     );
  }

  return root;
};

void setup()
{
  Serial.begin(9600); //initialize serial monitor
  Serial.println("start");
}

void loop()
{
  // readSensors();
  delay(LOOP_DELAY);
}
