/*
This is code to take an RC remote's throttle and steering pulses, translate them to what a tank drive robot needs to do, and re-output in the same RC pulse format (compatible with the Spark motor controllers).

See Repo: https://github.com/Team6321/RCremoteTranslator
*/
#include<TimerOne.h>  // Avaiable from http://www.arduino.cc/playground/Code/Timer1
#define THROTTLE_PIN 2
#define STEERING_PIN 3
#define Left_PIN 4
#define Right_PIN 5

volatile int Left;
volatile int Right;
volatile int Left_Count;
volatile int Right_Count;

int freqStep = 8; // This is the delay step in Microseconds
// It is calculated based on the period of the input pulse (1ms - 2 ms)
// and the number of speed steps you want (255). 


struct scalePulses {
  float Left; 
  float Right;
};


void setup() {
  // set pin modes
  pinMode(THROTTLE_PIN, INPUT);
  pinMode(STEERING_PIN, INPUT);

  pinMode(Left_PIN, OUTPUT);
  pinMode(Right_PIN, OUTPUT);
  
  // register timer
  

  // register pulse out pin (hw interrupt)
}

void loop() {
  // put your main code here, to run repeatedly:
  
  scalePulses scaledValues = setPulseOut();

  TranslateArcadeToTank(scaledValues.Left, scaledValues.Right);
}

struct scalePulses setPulseOut() {
  float steeringDuration, throttleDuration;
  float scaledSteeringDuration, scaledThrottleDuration;
  
  steeringDuration = pulseIn(STEERING_PIN, HIGH);
  throttleDuration = pulseIn(THROTTLE_PIN, HIGH);

  scaledSteeringDuration = map(steeringDuration, 1000, 2000, -1, 1);
  scaledThrottleDuration = map(throttleDuration, 1000, 2000, -1, 1);

  struct scalePulses scaledValues = {scaledSteeringDuration, scaledThrottleDuration};

  return scaledValues;
}

void TranslateArcadeToTank(int x, int y)
{
  float throttle = abs(y);
  float steering = abs(x);

  if (x > 0)
  {
    Left = throttle - steering;
    Right = max(throttle,steering);
  }
  else
  {
    Left = max(throttle,steering);
    Right = throttle - steering;
  }

  float temp = Left;
  Left = (y < 0)? -1 * Right : Left;
  Right = (y < 0)? -1 * Right : Right;
}
