/*
This is code to take an RC remote's throttle and steering pulses, translate them to what a tank drive robot needs to do, and re-output in the same RC pulse format (compatible with the Spark motor controllers).

See Repo: https://github.com/Team6321/RCremoteTranslator
*/
#include<TimerOne.h>  // Avaiable from http://www.arduino.cc/playground/Code/Timer1
#define THROTTLE_PIN 2
#define STEERING_PIN 3
#define LEFT_PIN 4
#define RIGHT_PIN 5

volatile int LeftCount;
volatile int RightCount;
volatile int Count;

int freqStep = 8; // This is the delay step in Microseconds
// It is calculated based on the period of the input pulse (1ms - 2 ms)
// and the number of speed steps you want (255). 


struct scalePulses {
  float Throttle; 
  float Steering;
};


void setup() {
  // set pin modes
  pinMode(THROTTLE_PIN, INPUT);
  pinMode(STEERING_PIN, INPUT);

  pinMode(LEFT_PIN, OUTPUT);
  pinMode(RIGHT_PIN, OUTPUT);
  
  // register timer
  Timer1.initialize(freqStep);                      // Initialize TimerOne library for the freq we need
  Timer1.attachInterrupt(setPulseOut, freqStep); // call a function (second param) every (first param) microseconds
  // Use the TimerOne Library to attach an interrupt
  // to the function we use to check to see if it is 
  // the right time to fire the triac.  This function 
  // will now run every freqStep in microseconds.     
  

  // register pulse out pin (hw interrupt)
  short interruptPin = digitalPinToInterrupt(THROTTLE_PIN);
  attachInterrupt(interruptPin, resetOutPulses, RISING);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  scalePulses scaledValues = readPulses();

  TranslateArcadeToTank(scaledValues.Steering, scaledValues.Throttle);
}

struct scalePulses readPulses() {
  float steeringDuration, throttleDuration;
  float scaledSteeringDuration, scaledThrottleDuration;
  
  steeringDuration = pulseIn(STEERING_PIN, HIGH);
  throttleDuration = pulseIn(THROTTLE_PIN, HIGH);

  scaledSteeringDuration = map(steeringDuration, 1000, 2000, -1, 1);
  scaledThrottleDuration = map(throttleDuration, 1000, 2000, -1, 1);

  struct scalePulses scaledValues = {scaledSteeringDuration, scaledThrottleDuration};

  return scaledValues;
}

void resetOutPulses(){
  Count = 0;
  digitalWrite(LEFT_PIN, HIGH);
  digitalWrite(RIGHT_PIN, HIGH);  
}

void TranslateArcadeToTank(float x, float y)
{
  float throttle = abs(y);
  float steering = abs(x);
  float left, right;

  if (x > 0)
  {
    left = throttle - steering;
    right = max(throttle,steering);
  }
  else
  {
    left = max(throttle,steering);
    right = throttle - steering;
  }

  if(y < 0){
    float temp = left;
    left = -right;
    right = -temp;
  }

  LeftCount = map(left, -1, 1, 1000, 2000) / freqStep;
  RightCount = map(right, -1, 1, 1000, 2000) / freqStep;

}

void setPulseOut(){

    if(LeftCount > Count) {
      digitalWrite(LEFT_PIN, LOW);
    }
    
    if(RightCount > Count) {
      digitalWrite(RIGHT_PIN, LOW);
    }

    ++Count; 
}
