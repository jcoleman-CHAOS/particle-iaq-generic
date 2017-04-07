/*
Project by:                   James Coleman
Dated:                           04/06/2017
Purpose:   Concisely explain data structure
*/

/* USER INPUT */
String firmware = "first commit";   // try to match this to a git commit - to track how a device is collecting data
String experiment = "calibration";  // used for filtering, and to differentiate concurrent experiments in same building
String location = "Archlab"; // probably a building, used for filtering
String label = "";  // something unqiue!
int _delay = 5000; // how often you want device to publish

#define num_data_points 10 // The MAX number of data points you'll be publishing
/**/

/*  GENERAL STRUCTURE

  Particle.pulish() uses...
    - EVENTS as decoding key. (ex. "temp.c,3", "humidity,2",...)
    - DATA to hold fields and tags. (ex. "Manifold_hot:20.2224", "Manifold_cold1:18.1245",...)

*/

/*"label,temp.unit" <-- structure
/*String data[num_data_points];*/
String data[] = {
    "Manifold_hot:20.2224",
    "Manifold_cold1:18.1245",
    "Manifold_cold2:17.1234",
    "5':83",
    "10':72",
    "5':221",
    "10':268",
    "voc:50"
}; // this hold a test array

// "phemenon.unit,num_samples"
String events[] = {
    "temp.c,3",
    "humidity,2",
    "co2.ppm,2",
    "voc.ppm,1"
}

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
  Serial.println("Test");
  /*Particle.publish("array length", String(num_data_points));*/
  delay(_delay);
}

void get_num_chars(){
  /* Loops through an array finds the number of characters */

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
