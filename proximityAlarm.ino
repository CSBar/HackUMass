/*
  Inspectio

  Detects objects on the left and right of a person wearing the belt and signals them
  when objects are close by

  circuit:
  - 1, 8 ohm speaker on digital pin 8
  - #, ultrasonic sensors

  created 20 Oct 2019
  by Blue Wall Peeps

  https://github.com/CSBar/HackUMass19
*/
// program parameters
const int minDistance = 70; //farthest object distance that will trigger a beep
const int numOfSensors = 2; //number of functional sensors connected

typedef struct sensor {
  int trigPin;
  int echoPin;
} sensor;

// defines pin numbers
const sensor sensorList[numOfSensors] = {{8, 7}, {3,2}};
const int buzzer = 13;

enum direction {
  LEFT,
  RIGHT
};


/*
 * Has the specified sensor scan for nearby objects
 * @param ultrasonicSensor - specified sensor with valid trigPin and echoPin connected to arduino
 * @return distance (in cm) of the nearest object detected by the sensor
*/
int detectNearestObject(sensor ultrasonicSensor);

/*
 * Sends a signal to the speaker to play specified sounds
 * Discriminates left and right sensor signals and sounds more frequently when objects are closer to the sensor
 * @param direction - whether the sensor sending a signal is from the left or right side of the person
 * @param distance - distance (in cm) of the nearest object detected by the sensor
 * @return NOTHING
*/
void playBuzzerSound(int direction, int distance);


void setup() {
  for(int i=0; i<numOfSensors; i++) {
    pinMode(sensorList[i].trigPin, OUTPUT); // Sets each trigPin as an Output
    pinMode(sensorList[i].echoPin, INPUT); // Sets each echoPin as an Input
  }
  pinMode(buzzer, OUTPUT); // Sets the buzzer as an Output
  Serial.begin(9600); // Starts the serial communication
}

void loop() {
  for(int i=0; i<numOfSensors; i++) {
    int distance = detectNearestObject(sensorList[i]);
    if(distance < minDistance)
      playBuzzerSound(i % 2, distance);
  }
}


int detectNearestObject(sensor ultrasonicSensor) {
  // Clears the trigPin
  digitalWrite(ultrasonicSensor.trigPin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(ultrasonicSensor.trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultrasonicSensor.trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  long duration = pulseIn(ultrasonicSensor.echoPin, HIGH);

  // Calculating the distance
  int distance= duration*0.034/2;

  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);

  return distance;
}


void playBuzzerSound(int direction, int distance) {
  int delayTime = distance * 3;
  if(direction == LEFT) {
    tone(buzzer, 1000, 200);
    delay(delayTime);
    noTone(buzzer);
    delay(delayTime);
  }
  else {
    tone(buzzer, 1100, 100);
    delay(delayTime);
    noTone(buzzer);
    delay(delayTime);
  }
}
