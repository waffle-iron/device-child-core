/*********************************************
* tiempodesiembra.es                         *
* child controller 0.0.2                     *
**********************************************/

/*********************************************
* Device setup                               *
**********************************************/

/* uuid - https://www.uuidgenerator.net/ */

/* Unique ID for this device */
const String DEVICE_UNIT_UUID   = {"8d1b208e-cadb-4767-b199-cda6cbed3b0d"};

/* Cryto KEY used to share message with the root device. */
const String DEVICE_LINK_CRYPTO = {"960e4ce7-53e2-48ac-b7bb-30ef00f2d20d"};

/* Enable or disable */                               /* Enable or disable   */
const boolean MOISTURE_SENSOR_ENABLED         = true; /* Moisture            */
const boolean TEMPERATURE_SENSOR_ENABLED      = true; /* Temperature         */

/* PIN setup */                                       /* PIN setup           */
const int MOISTURE_SENSOR_PIN                 = 0;    /* Moisture sensor     */
const int TEMPERATURE_SENSOR_PIN              = 1;    /* Temperature sensor  */
const int RF24_CE_PIN                         = 9;    /* nRF24L01 ce pin     */
const int RF24_CSN_PIN                        = 10;   /* nRF24L01 csn pin    */

/* Prints to Serial instead of using the nRF module */
const boolean DEBUG                           = false;

/*********************************************
*                                            *
*                                            *
*    Do NOT change anything from this point  *
*                                            *
*                                            *
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

/*********************************************
* Libraries                                  *
**********************************************/

/* Vars */
#include <printf.h>
#include <ArduinoJson.h>

/* Comm */
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>

/*********************************************
* System definitions                         *
**********************************************/

const char* VERSION = {"0.0.2"};

/* Loop */
const int LOOP_DELAY = 1 * 1000; // <seconds> * <ms>

/* Levels */
const String SENSOR_LEVELS[9] = {"lowl", "low", "lowg", "medl", "med", "medh", "higl", "hig", "higg"};

/* Comm */
int msg[1];
RF24 radio(RF24_CE_PIN, RF24_CSN_PIN);
const uint64_t pipe = 0xE8E8F0F0E1LL;

/*********************************************
* Functions                                  *
**********************************************/

/*
 * Returns the level for a given type and value
 */
String getLevel(char type[3], int value) {
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

String readSensors() {

  // Create JSON object
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  // Attach device ID
  root["d"] = DEVICE_UNIT_UUID;
  
  // Attach device version
  root["v"] = VERSION;
  
  if (MOISTURE_SENSOR_ENABLED) {
    JsonArray& mData = root.createNestedArray("moi");
    int val = analogRead(MOISTURE_SENSOR_PIN);
    mData.add(val);
    mData.add(getLevel("MOI", val));
  }
  
  if (TEMPERATURE_SENSOR_ENABLED) {
    JsonArray& tData = root.createNestedArray("tem");
    int val = analogRead(TEMPERATURE_SENSOR_PIN);
    tData.add(val);
    tData.add(getLevel("TEM", val));
  }

  String output;
  root.printTo(output);
  return output;
};

void setup()
{
  Serial.begin(9600); //initialize serial monitor
  Serial.println("Tiempo de siembra, child controller: " + String(VERSION));

  if (!DEBUG) {
    Serial.println("Radio begin");
    radio.begin();
    radio.openWritingPipe(pipe);  
  }
  
}

void loop()
{
  String theMessage = readSensors();
  
  if (DEBUG) {
    Serial.println(theMessage);
  }
  else {
    radio.powerUp();
    theMessage = String(theMessage + "~");

    int messageSize = theMessage.length();
    for (int i = 0; i < messageSize; i++) {
      int charToSend[1];
      charToSend[0] = theMessage.charAt(i);
      
      radio.write(charToSend,1);
      delay(10);
    }
    radio.powerDown();
  }

  delay(LOOP_DELAY);

}

