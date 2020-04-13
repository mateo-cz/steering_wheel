
//Steering wheel and throttle.


#include <Joystick.h> //Download Joystic library from here: https://github.com/MHeironimus/ArduinoJoystickLibrary

//Note: it's not working when I'm using: JOYSTICK_TYPE_MULTI_AXIS. It's not recognized as Gaming Device
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_JOYSTICK,
0,0, //how many buttons
true, true, false, //only X axis steering and y axis throtle
false, false, false, //no Rx, Ry i Rz axis
false, false, 
false, false, false); 


const int potVcc = 9; //using pin 9 to provide power to potentiometer
//------------------------------------------
//Note: not every pin is usable for interrupts!
//You need to connect the signal wires to pins which are usable for interrupts: Micro, Leonardo, other 32u4-based: 0, 1, 2, 3, 7
const int outA = 2; //white cable
const int outB = 3; //yellow cable
volatile long encoder = 0; // volatile is using RAM this is because we will use interrupts
long encoder_save = 0;


void setup() {

Serial.begin(9600); //you can disable serial. it's just to check if it's working
  
//potentiometer
  pinMode(potVcc, OUTPUT);
  digitalWrite(potVcc, HIGH); //power up potentiometer
  Joystick.setYAxisRange(0, 1023);
//--------------------- 
  
Joystick.begin();
Joystick.setXAxisRange(-1600, 1600);
pinMode(outA, INPUT_PULLUP); //Encoder pin
pinMode(outB, INPUT_PULLUP); //Encoder pin
attachInterrupt(digitalPinToInterrupt(outA),outAChange,CHANGE); //CHANGE to trigger the interrupt whenever the pin changes value
attachInterrupt(digitalPinToInterrupt(outB),outBChange,CHANGE);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (encoder_save != encoder) { // only print if new value
    encoder_save = encoder; 
    Joystick.setXAxis(encoder);
    Serial.println(encoder);
  }
  delay(10);
  //potentiometer throttle----------- 
  int ThrottlePot = analogRead(A0);
  Joystick.setYAxis(ThrottlePot);
}


void outAChange() {
  // when outA changes, outA==outB means negative direction
  encoder += digitalRead(outA) == digitalRead(outB) ? 1 : -1; 
}

void outBChange() {
  // when outB changes, outA==outB means positive direction
  encoder += digitalRead(outA) == digitalRead(outB) ? -1 : 1;
}
