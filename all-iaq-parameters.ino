/*
Project by:                               James Coleman
Dated:                                      04/011/2017
Purpose:    Encode and publish multiple sensor readings
*/

// I HAVE DISABLED DHT STUFF!

#define num_data_points 3 // The MAX number of data points you'll be publishing
#define num_phenom_types 3

#define voc_pin A0
#define no2_pin A1
#define hcho_pin A2
// #define DHTPINA A3
// #define DHTTYPE DHT22

#define Vc 4.95
#define R0 34.00 // the resistance of the HCHO circuit in ambient conditions

/* INCLUDE Statements */
#include "application.h"  // Necessary for all I2C commands
// #include "DHT.h"
/**/

/* USER INPUT */
String firmware = "template added";   // try to match this to a git commit - to track how a device is collecting data
String experiment = "CISBAT";  // used for filtering, and to differentiate concurrent experiments in same building
String location = "ChaosLab_1"; // probably a building, used for filtering
String label = "alpa";  // something unqiue!
int _delay = 2500; // how often you want device to publish
/**/

/* ENCODING */
int co2_index = 0;
String co2_label = "co2_test";

int voc_index = 1;
String voc_label = "voc_test";

int no2_index = 2;
String no2_label = "no2_test";

int hcho_index = 3;
String hcho_label = "hcho_test";

// int temp_index = 4;
// String temp_label = "temp_test";

// int humid_index = 5;
// String humid_label = "humid_test";
/**/

/* READINGS */
int co2_reading;
float voc_reading;
float no2_reading;
float hcho_reading;
float hcho_Rs;
// float humid_reading;    // humidity
// float temp_reading;    // temperature c
/**/

// DHT dhta(DHTPINA, DHTTYPE); // create your DHT object

String data[] = {
    "", // co2_index  = 0
    "", // voc_index  = 1
    "", // no2_index  = n/a
    "" // hcho_index = 2
    // "", // temp_index = 4
    // "" // humid_index = 5
}; // "label:value"

String phenom_types[] = {
    "co2.ppm,1",
    "voc.ppm,1",
    "no2.ppm,1",
    "hcho.ppm,1"
    // "temp.c,1",
    // "humid,1"
}; // "phemenon.unit,num_samples"


/* Helper Stuff */
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
    // dhta.begin();
}

void loop() {
  Serial.println("\nStart");
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

    co2_reading = buffer[2] * 256 + buffer[3];

    data[co2_index] = co2_label + ":" + String(co2_reading);
  /**/

  /* VOC Loop */
  voc_reading = analogRead(voc_pin);
  voc_reading = ((voc_reading - 3) / 4095 * 500);
  data[voc_index] = voc_label + ":" + String(voc_reading);
  /**/

  /* NO2 Loop */
  no2_reading = analogRead(no2_pin);
  no2_reading = 9860/(4095/no2_reading - 1);
  data[no2_index] = no2_label + ":" + String(no2_reading);
  /**/

  /* HCHO Loop*/
  hcho_reading=analogRead(hcho_pin);
  hcho_Rs=(4095/hcho_reading)-1;
  hcho_reading=pow(10.0,((log10(hcho_Rs/R0)-0.0827)/(-0.4807)));
  data[hcho_index] = hcho_label + ":" + String(hcho_reading);
  /* */

//   /* DHT22 Loop */
//   delay(1000);
//   temp_reading = dhta.readTemperature(false);  // Read Temperature
//   data[temp_index] = temp_label + ":" + String(temp_reading);
//   delay(100);
//   humid_reading = dhta.readHumidity(); // Read Humidity
//   data[humid_index] = humid_label + ":" + String(humid_reading);
//   delay(1000);
//   /**/

//   if (hcho_reading > 85) {
//       NVIC_SystemReset();
//   }

  /* ENCODE + PUBLISH */
  event_string = join(phenom_types, num_phenom_types);
  data_string = join(data, num_data_points);

  Serial.println(event_string);
  Serial.println(data_string);
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
