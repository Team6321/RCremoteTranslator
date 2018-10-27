/*
This is code to take an RC remote's throttle and steering pulses, translate them to what a tank drive robot needs to do, and re-output in the same RC pulse format (compatible with the Spark motor controllers).

See Repo: https://github.com/Team6321/RCremoteTranslator
*/
#include<TimerOne.h>  // Avaiable from http://www.arduino.cc/playground/Code/Timer1
#define THROTTLE_PIN 2
#define STEERING_PIN 3
#define LEFT_PIN 4
#define RIGHT_PIN 5

volatile int Left;
volatile int Right;
volatile int Left_Count;
volatile int Right_Count;

int freqStep = 8; // This is the delay step in Microseconds
// It is calculated based on the period of the input pulse (1ms - 2 ms)
// and the number of speed steps you want (255). 

void setup() {
  // set pin modes
  pinMode(THROTTLE_PIN, INPUT);
  pinMode(STEERING_PIN, INPUT);

  pinMode(LEFT_PIN, OUTPUT);
  pinMode(RIGHT_PIN, OUTPUT);
  
  // register timer
  

  // register pulse out pin (hw interrupt)
}

void loop() {
  // put your main code here, to run repeatedly:

}

void setPulseOut() {
  
}

