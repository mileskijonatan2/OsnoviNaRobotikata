#include <Servo.h>
#include <Stepper.h>

Servo servo1;  // Create a Servo object

#define echoPin 2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 3 //attach pin D3 Arduino to pin Trig of HC-SR04
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement
#define r 0.65 // radius - the distance between the rotating point and the ultrasonic sensor
#define rad 0.017 // one degree equals to radians
#define delayTime 200 // Adjust this delay to approximate 5 degrees
#define numSteps 72  // Number of 5-degree steps in 360 degrees (360/5=72)
#define stepsPerRevolution 2038
#define d 18 // 18cm distance between the ultrasonic sensor and the rotation shaft of the stepper mottor 

Stepper stepper = Stepper(stepsPerRevolution, 8, 10, 9, 11);

 
void setup() {
  servo1.attach(5); 
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  stepper.setSpeed(10);
  Serial.begin(9600);
}

float findMedian(float arr[], int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                int temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
    if (n % 2 != 0)
        return arr[n / 2];
    return (arr[(n - 1) / 2] + arr[n / 2]) / 2;
}

void loop() {
  for(int pos = 70; pos<=114; pos += 2){
    servo1.write(pos);
    delay(500);

    for(int i=1; i<=72; i++){ //72
      stepper.step(stepsPerRevolution / 72); // Move 5 degrees
      delay(100); // Delay between steps

      distance = 0;
      float values[5] = {};
        int local_distance = 0;
        for(int j=0; j<5; j++){
          //Measuring the distance 
          digitalWrite(trigPin, LOW);
          delayMicroseconds(2);
          // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
          digitalWrite(trigPin, HIGH);
          delayMicroseconds(10);
          digitalWrite(trigPin, LOW);
          // Reads the echoPin, returns the sound wave travel time in microseconds
          duration = pulseIn(echoPin, HIGH);
          delay(5);
          // Calculating the distance
          local_distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
          values[j] = local_distance;
          distance += local_distance;
        }  
      
      distance = findMedian(values, 5);

      // calculate coordinates
      float beta = 5.0 * i; // angle of stepper mottor in degrees
      float alpha = (pos-90);
      int sign = (pos-90)?-1:+1;

      /*float fi = (180 - alpha) / 2.0;
      float gamma = 90 - fi;
      float s = abs(2 * sin(alpha / 2 * rad) * r) * 2;
      float delta_x = abs(sin(gamma * rad) * s);*/

      float l = distance; // abs(cos(alpha*rad-7*rad*sign)*distance-delta_x);
      float z = sin(alpha*rad)*d;
      float x = d - l;
      float y = sin(beta*rad)*x;
      x = cos(beta*rad)*x;

      if(x<30&&y<30&&distance<50){
        String coordinates = String(x) + ", " + String(y) + ", " + String(z) + ", " + String(pos) + ", " + String(distance);
        Serial.println(coordinates.c_str());
      }
      
    }
  }

  while (1) { // The loop stops once 360 degrees is reached
    delay(1);
  }

}
