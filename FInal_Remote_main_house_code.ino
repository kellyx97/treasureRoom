//This Type Of Sensor Detects Motion And lows L.E.D And Start Buzzing,It Also Displays The That The "Motion is Detected" On
//An Lcd Screen
// Referenced from http://www.instructables.com/id/Home-Security-Alarm-or-a-Motion-Detector-Using-Ard/

#include <SoftwareSerial.h>
#include <Button.h>
#include "Wire.h"
#include "Adafruit_LiquidCrystal.h"


// Includes the Servo library
#include <Servo.h>
// Defines Tirg and Echo pins of the Ultrasonic Sensor
const int trigPin = 9;
const int echoPin = 11;
// Variables for the duration and the distance
long duration;
int distance;
Servo myServo; // Creates a servo object for controlling the servo motor

int activatedledPin = 13;                // choose the pin for the LED
const int greenLED = 1;           //***I changed it from Pin 9 to 1
int inputPin = 7;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status
int pinSpeaker = 10;           //Set up a speaker on a PWM pin (digital 9, 10, or 11)
unsigned long timee = 0;

// button stuff
const int button1 = 4;
const int button2 = 5;
const int button3 = 6;
const int button4 = 8;

int pValue;
String buf;
int tries = 0;
int count = 0;
int next = 0;
boolean lol = true;
boolean state = true;
String command = "";
String password = "1234";

Button code1 (button1, INPUT_PULLUP, 1500);
Button code2 (button2, INPUT_PULLUP, 1500);
Button code3 (button3, INPUT_PULLUP, 1500);
Button code4 (button4, INPUT_PULLUP, 1500);


const String alert = "motion trip";
SoftwareSerial xBee(2, 3); // (TX, RX) : pins on XBee adapter

//LiquidCrystal lcd(12, 11, 5, 4, 3, 2);                          // initialize the library with the numbers of the interface pins
Adafruit_LiquidCrystal lcd(0);


void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  myServo.attach(12); // Defines on which pin is the servo motor attached

  pinMode(activatedledPin, OUTPUT);      // declare LED as output  pinMode(inputPin, INPUT);     // declare sensor as input
  pinMode(greenLED, OUTPUT);
  pinMode(pinSpeaker, OUTPUT);
  Serial.begin(9600);

  lcd.begin(16, 2);
  lcd.setCursor(2, 0);                                            // Set LCD cursor position (column, row)
  lcd.print("System On");                                      // Print text to LCD
  lcd.setCursor(5, 1);                                            // Set LCD cursor position (column,row)                                            // Print text to LCD
  delay(1000); // wait                                      // Delay to read text
  // Delay to read text
  lcd.clear();                                                    // Clear LCD
  lcd.setCursor(0, 0);
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("Waiting For");
  lcd.setCursor(3, 1);
  lcd.print("Motion....");

  xBee.begin(9600);
  xBee.println("main on");
}

void loop() {

  // look to see if any complete incoming messages are ready
  checkMessageReceived();


  // button check
  int code1Action = code1.checkButtonAction();
  int code2Action = code2.checkButtonAction();
  int code3Action = code3.checkButtonAction();
  int code4Action = code4.checkButtonAction();

  if (code1Action == Button::CLICKED) {  //chnaged Button::CLICKED to isPressed
    tone(pinSpeaker, 3322, 20);
    buf += "1";
    tries += 1;
    next += 1;
  }
  if (code2Action == Button::CLICKED) {
    tone(pinSpeaker, 3322, 20);
    buf += "2";
    tries += 1;
    next += 1;
  }
  if (code3Action == Button::CLICKED) {
    tone(pinSpeaker, 3322, 20);
    buf += "3";
    tries += 1;
    next += 1;
  }
  if (code4Action == Button::CLICKED) {
    tone(pinSpeaker, 3322, 20);
    buf += "4";
    tries += 1;
    next += 1;
  }

  if (tries == 4 && count < 3) {         //if tried more than 4 times, ENTRY DENIED
    lcd.setCursor(0, 1);
    lcd.print("Entry denied");
    tone(pinSpeaker, 4699, 20);
    tries = 0;
    if (state == true) {
      count += 1;
    }
  }

  if (next == 5) {
    next = 1;
    lcd.setCursor(0, 1);
    lcd.print("            ");
  }

  if (count >= 3) {
    if (lol == true) {
      lcd.clear();
      lcd.print("Passcode wrong 3 times");
      lol = false;
      xBee.println("main password");

    }
    tone(pinSpeaker, 4699);
  }

  // if correct password

  if (buf.endsWith(password) && state == true && count < 3) {
    noTone(pinSpeaker);
    count = 0;
    lcd.clear();
    lcd.print("System Off");
    digitalWrite(greenLED, HIGH);
    digitalWrite(activatedledPin, LOW);
    buf += "END";
    tries = 0;
    next = 0;
    state = false;
    xBee.println("main off");
  } else if (buf.endsWith(password) && state == false) {
    noTone(pinSpeaker);
    count = 0;
    lcd.clear();
    lcd.print("System On");
    digitalWrite(greenLED, LOW);
    digitalWrite(activatedledPin, HIGH);
    buf += "END";
    tries = 0;
    next = 0;
    state = true;
    xBee.println("main on");
  }

  //detect motion code

  if (state = true) {             // if password is not pressed
    val = digitalRead(inputPin);  // read input value
    if (val == HIGH) {            // check if the input is HIGH
      digitalWrite(activatedledPin, HIGH);  // turn LED ON
      playTone(3000, 300);
      delay(150);
      if (pirState == LOW) {

        // we have just turned on

        Serial.println("Motion detected!");   //motion detected write into serial monitor
        Serial.print("Time: ");
        timee = millis() / 1000;
        Serial.println(timee);

        // send to xBee
        String msg = alert;
        Serial.println(msg);
        xBee.println(msg);

        lcd.clear() ;
        lcd.setCursor(0, 0);                                           // Set LCD cursor position (column 0, row 0)
        lcd.print("Motion Detected!");

        // We only want to print on the output change, not state
        pirState = HIGH;

        sweep();
      }
    } else {
      digitalWrite(activatedledPin, LOW); // turn LED OFF
      playTone(0, 0);
      //delay(300);

      if (pirState == HIGH) {
        // we have just turned of
        lcd.clear() ;
        lcd.setCursor(3, 0);
        lcd.print("Waiting For");
        lcd.setCursor(3, 1);
        lcd.print("Motion....");      // We only want to print on the output change, not state
        pirState = LOW;
        myServo.write(15);
      }
    }
  }
}

// duration in mSecs, frequency in hertz
void playTone(long duration, int freq) {
  duration *= 1000;
  int period = (1.0 / freq) * 10000;  //originally 100000 changed to 1000 faster sending
  long elapsed_time = 0;
  while (elapsed_time < duration) {
    digitalWrite(pinSpeaker, HIGH);
    delayMicroseconds(period / 2);
    digitalWrite(pinSpeaker, LOW);
    delayMicroseconds(period / 2);
    elapsed_time += (period);
  }
}


// Function for calculating the distance measured by the Ultrasonic sensor
int calculateDistance() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); // Reads the echoPin, returns the sound wave travel time in microseconds
  distance = duration * 0.034 / 2;
  return distance;
}
void sweep() {
  for (int i = 15; i <= 165; i++) {
    myServo.write(i);
    delay(20);
    distance = calculateDistance();// Calls a function for calculating the distance measured by the Ultrasonic sensor for each degree

    Serial.print(i); // Sends the current degree into the Serial Port
    Serial.print(","); // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
    Serial.print(distance); // Sends the distance value into the Serial Port
    Serial.print("."); // Sends addition character right next to the previous value needed later in the Processing IDE for indexing
  }
  // Repeats the previous lines from 165 to 15 degrees
  for (int i = 165; i > 15; i--) {
    myServo.write(i);
    delay(20);
    distance = calculateDistance();
    Serial.print(i);
    Serial.print(",");
    Serial.print(distance);
    Serial.print(".");
  }
}

void checkMessageReceived () {

  if (xBee.available()) {
    String Msystem = xBee.readString();
    Serial.println(Msystem);

    if (Msystem == "main clear") {
      Serial.println(Msystem);
      pirState = LOW;
      val = LOW; s
      noTone(pinSpeaker);
      int tries = 0;
      count = 0;
      next = 0;
      lol = true;
      state = true;
      lcd.clear();
      lcd.print("System On");
    }

  }

}

