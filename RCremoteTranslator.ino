/*
This is code to take an RC remote's throttle and steering pulses, translate them to what a tank drive robot needs to do, and re-output in the same RC pulse format (compatible with the Spark motor controllers).

See Repo: https://github.com/Team6321/RCremoteTranslator
*/
#define THROTTLE_PIN 2
#define STEERING_PIN 3
#define LEFT_PIN 4
#define RIGHT_PIN 5

volatile int LeftCount;
volatile int RightCount;

struct scalePulses {
  float Throttle; 
  float Steering;
};

/* This method is run once on  boot and is responsible for:
* Setting up input and output pins
* Initializing the timer interrupt for setting the pulses out
* Registering for a hardware interrupt to trigger start of pulse out.
*/
void setup() {
  // set pin modes
  pinMode(THROTTLE_PIN, INPUT);
  pinMode(STEERING_PIN, INPUT);

  pinMode(LEFT_PIN, OUTPUT);
  pinMode(RIGHT_PIN, OUTPUT);
  
  


  Serial.begin(9600);
}

/*
Main code goes here, in our case, that means read the pulses coming from the remote, scale, and store them.
*/
void loop() {
 
  scalePulses scaledValues = readPulses();

  TranslateArcadeToTank(scaledValues.Steering, scaledValues.Throttle);
}

/* read the pulses coming from the remote. 
*   A pulse width of 1000 means full backwards, 
*   A pulse width of 1500 means disable, and 
*   A pulse width of 2000 means full forward
*/
struct scalePulses readPulses() {
  int steeringDuration, throttleDuration;
  float scaledSteeringDuration, scaledThrottleDuration;
  
  steeringDuration = pulseIn(STEERING_PIN, HIGH, 25000);
  throttleDuration = pulseIn(THROTTLE_PIN, HIGH, 25000);

  scaledSteeringDuration = map(steeringDuration, 1000, 2000, -1, 1);
  scaledThrottleDuration = map(throttleDuration, 1000, 2000, -1, 1);

  // in the event of a timeout, set values to 0
  if(steeringDuration == 0) {
    scaledSteeringDuration = 0;
  }
  if(throttleDuration == 0) {
    scaledThrottleDuration = 0;
  }
  
  struct scalePulses scaledValues = {scaledSteeringDuration, scaledThrottleDuration};

  // TODO: disable the following lines once this is tested.
  char buffer[100]; // you have to be aware of how long your data can be
                 // not forgetting unprintable and null term chars
  sprintf(buffer,"Raw: %i,\t%i\t\t", steeringDuration, throttleDuration);
  Serial.print(buffer);
  
  // sprintf does not support floats . . .
  Serial.print("Scaled: ");
  Serial.print(scaledValues.Steering);
  Serial.print(",\t");
  Serial.print(scaledValues.Throttle);

  return scaledValues;
}



/*
* Translated from FRC code library
*/
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
  
  /* TODO: enable this once read-in is debugged
  Serial.print("Values: ");
  Serial.print(left);
  Serial.print(",\t");
  Serial.print(right);
  Serial.print("\t\t");
  Serial.print("Scaled: ");
  Serial.print(LeftCount);
  Serial.print(",\t");
  Serial.print(Rightcount);
  */
}

void pulseOut(int pin, unsigned long duration, bool level=HIGH) {
  unsigned long delayTime = duration;
  unsigned long longDelay = 0;
  if (duration > 16000)
  {
    // delayMicro can only go up ~16k, so use millisecond delay for beyond that.
    longDelay = duration / 1000;
    delayTime = duration - (longDelay * 1000);
    Serial.println(delayTime);
  }
  digitalWrite(pin, level);
  delay(longDelay);
  delayMicroseconds(delayTime);
  digitalWrite(pin, !level);
}
