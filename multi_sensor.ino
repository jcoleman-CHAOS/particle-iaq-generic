/*
Project by:                               James Coleman
Dated:                                      04/011/2017
Purpose:    Encode and publish multiple sensor readings
*/

#define num_data_points 1 // The MAX number of data points you'll be publishing
#define num_phenom_types 1

/* INCLUDE Statements */
#include "application.h"  // Necessary for all I2C commands
/**/

/* USER INPUT */
String firmware = "template added";   // try to match this to a git commit - to track how a device is collecting data
String experiment = "CISBAT";  // used for filtering, and to differentiate concurrent experiments in same building
String location = "ChaosLab"; // probably a building, used for filtering
String label = "alpa";  // something unqiue!
int _delay = 5000; // how often you want device to publish
/**/

/* ENCODING */
int co2_index = 0;

String data[] = {
    "" // co2_index = 0
}; // "label:value"

String phenom_types[] = {
    "co2.ppm,1"
}; // "phemenon.unit,num_samples"

String string_holder = "";
String event_string = "";
String data_string = "";
/**/

/* CO2 SENSOR, I2C stuff */
int i = 0;
static const int address = 0x15;  // I2C Sensor address
static const int MOD_READ[] = {
0x04, /* Modbus function: Read Input Registers */
0x13, /* Starting Address: 5003, MSB first */
0x8B,
0x00, /* # of registers to read: 1, MSB first */
0x01,
0x46, /* CRC, LSB then MSB */
0x70
};
unsigned char buffer[6]; // this is to store the response
int ppm_co2; // This is to store our value
/**/

void setup() {
    // Hardware notes
    Particle.variable("firmware", firmware);

    // Experimental notes
    Particle.variable("experiment", experiment);
    Particle.variable("location", location);  // usually a building
    Particle.variable("label", label);  // something unqiue
    Particle.variable("rate", _delay);  // Publish frequency

    Serial.begin(9600);
    Wire.begin();
}

void loop() {
  Serial.println("\nStart");
  Particle.publish(event_string,data_string);
  Serial.println(event_string);
  Serial.println(data_string);
  Serial.println("End");
  Serial.println();

  /* CO2 Loop */
    // Send request to CO2 sensor
    Wire.beginTransmission(address);
    for(i = 0; i < 6; i++){
      Wire.write(MOD_READ[i]);
      }
    Wire.endTransmission();

    Wire.requestFrom(address, 6);
    if (Wire.available()){
      for (int i=0; i<7; i++){
        buffer[i] = Wire.read();
      }
    }

    ppm_co2 = buffer[2] * 256 + buffer[3];

    data[co2_index] = String(ppm_co2);

    Serial.print("PPM co2: ");
    Serial.println(ppm_co2);
  /**/

  /* ENCODE + PUBLISH */
  event_string = join(phenom_types, num_phenom_types);
  data_string = join(data, num_data_points);

  Particle.publish(event_string,data_string); // Publish
  /**/

  delay(_delay);
}

String join(String words[], int num_words){
  String all_together = "";
  for (int i=0; i < num_words; i++){
    all_together += (words[i] + " ");
  }
  return all_together;
}

void generate_string(){
    /*
    Max EVENT length is 63 characters,
    Max DATA length is 255 characters

    PSUEDO CODE:
    1. Pass this function an array
    2. Ensure that EVENT.length is less than 63 characters
    3. Ensure DATA.length is less than 255 characters
    4. Generate largest EVENT string possible
        4.0 Then create matching DATA String
    5. Call Particle.Publish
    6. Retain remaining EVENTS
    7. Reject new data
    7. Keep publishing buffer until it is empty
    8. Accept new data
    */


}
