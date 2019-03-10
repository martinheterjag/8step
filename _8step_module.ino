#define NOT_PRESSED HIGH
#define WAS_PRESSED LOW

// Predefined functions
void HandleClock();
void StepIfTriggered();

// Global Variables
const int out1 = 6;
const int out2 = 7;
const int out3 = 8;
const int out4 = 9;
const int out5 = 10;
const int out6 = 11;
const int out7 = 12;
const int out8 = 13;

const int clk_in = 3;
const int clk_out = 5;

// Step sequencer variables
int current_step = out1;  // Always start on step one.

int last_button_state = NOT_PRESSED;   // the previous reading from the input pin
int button_state = NOT_PRESSED;
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 10;    // the debounce time; increase if the output flickers

// Clock generator variables
unsigned long clk_period = 1000;  // Period in milliseconds
unsigned long last_clock_cycle = 0;
int output_state = LOW;

void setup() {                
  // initialize the digital pin as an output.
  pinMode(out1, OUTPUT);
  pinMode(out2, OUTPUT);
  pinMode(out3, OUTPUT);
  pinMode(out4, OUTPUT);
  
  pinMode(out5, OUTPUT);
  pinMode(out6, OUTPUT);
  pinMode(out7, OUTPUT);
  pinMode(out8, OUTPUT);
  
  pinMode(clk_in, INPUT);
  digitalWrite(clk_in, HIGH);
  
  pinMode(clk_out, OUTPUT);

  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  HandleClock();
  StepIfTriggered();
}

void HandleClock() {
  clk_period = (analogRead(A0) + 100);
 /* Serial.print("millis = ");
  Serial.println(millis());
  Serial.print("last = ");
  Serial.println(last_clock_cycle);
  Serial.print("millis - last = ");
  Serial.println(millis() - last_clock_cycle);*/
  
  if(abs(millis() - last_clock_cycle) > clk_period / 2) {
    output_state = !output_state;   
    digitalWrite(clk_out, output_state);
    
    last_clock_cycle = millis();
  }
}

void StepIfTriggered() {
  int reading = digitalRead(clk_in);
  
  // If the switch changed, due to noise or pressing:
  if (reading != last_button_state) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  } 
  
  if ((millis() - lastDebounceTime) > debounceDelay) {
    Serial.println(reading);
    // Only trigger if there is a change since last time
    if (reading != button_state) {
      button_state = reading;
      Serial.println(reading); 
      if (button_state == WAS_PRESSED) {
        digitalWrite(current_step, LOW);
        
        current_step++;
        if (current_step > out8)
          current_step = out1;
      }
    }
  }
  digitalWrite(current_step, HIGH);
  
  last_button_state = reading; 
}
