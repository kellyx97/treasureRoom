// Requesting/Including libraries required for running the code
#include "Wire.h"
#include "Adafruit_LiquidCrystal.h"
#include <Button.h>
#include <SoftwareSerial.h>

/* xbee = TX = 2(yellow), RX = 3(orange)
 *  
 * LCD:
 * 5V to Arduino 5V pin
 * GND to Arduino GND pin
 * CLK to Analog #5 (green)
 * DAT to Analog #4 (yellow)
 */
 
// Activating the LCD display
Adafruit_LiquidCrystal lcd(0);

// Assigning variable names to pins
const int buzzer = 5;
const int button1 = 4;
const int button2 = 6;
const int button3 = 7;
const int button4 = 8;
const int greenLED = 9;
const int redLED = 10;
const int pResistor = A0; 

int pValue;   // Photoresistor
String buf;   // Buffer to seperate passcodes
int tries = 0;   // Counts the amount of buttons clicked
int count = 0;   // Counts number of complete passcode tries
int next = 0;   // Counts 5th button press to clear message 
boolean repeat = true;   // Stop repeating of "Intruder" message
boolean state = true;   // System state (on or off) | true = system off   false = system on
String password = "1234"; // Password for the system
boolean laserTrip = false; // Laser status

// Activating the four button inputs for the entry code system
Button code1 (button1, INPUT_PULLUP, 1500);
Button code2 (button2, INPUT_PULLUP, 1500);
Button code3 (button3, INPUT_PULLUP, 1500);
Button code4 (button4, INPUT_PULLUP, 1500);

// Activating the xBee radio communication module
SoftwareSerial xBee(2, 3);

void setup() {
  // Opening serial communication ports
  Serial.begin(9600);
  xBee.begin(9600);

  // Activating the LCD display
  lcd.begin(16,2);
  lcd.print("System On");

  // For setting up pins for input/output
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(pResistor, INPUT);

  // Setting the initial status light on startup
  digitalWrite(redLED, HIGH);
}

void loop() {
  checkMessageReceived();
  keycodeEntry();
  laserReading(analogRead(pResistor));
}

// Method that handles communication with the command center as well as re-setting the system when receiving the command to
void checkMessageReceived () {
  if (xBee.available()) {
    String Msystem = xBee.readString();
    Serial.println(Msystem);
    if (Msystem == "treasure clear") {
      resetSystem();
      lcd.print("System On");
      state = true;
      repeat = true;
      laserTrip = false;
      digitalWrite(greenLED, LOW);
      digitalWrite(redLED, HIGH);
    }
  }
}

// Method that contains the keycode entry algorithm
void keycodeEntry() {
  // For checking if keycode entry buttons are pressed
  int code1Action = code1.checkButtonAction();
  int code2Action = code2.checkButtonAction();
  int code3Action = code3.checkButtonAction();
  int code4Action = code4.checkButtonAction();

  // For changing variables if keykode entry buttons are pressed
  if (code1Action == Button::CLICKED) {
    tone(buzzer, 3322, 20);
    buf += "1";
    tries += 1;
    next += 1;
  }
  if (code2Action == Button::CLICKED) {
    tone(buzzer, 3322, 20);
    buf += "2";
    tries += 1;
    next += 1;
  }
  if (code3Action == Button::CLICKED) {
    tone(buzzer, 3322, 20);
    buf += "3";
    tries += 1;
    next += 1;
  }
  if (code4Action == Button::CLICKED) {
    tone(buzzer, 3322, 20);
    buf += "4";
    tries += 1;
    next += 1;
  }

  // For applying the "entry denied" message to the LCD
  if (tries == 4 && count < 3) {
    lcd.setCursor(0,1);
    lcd.print("Entry denied");
    tone(buzzer, 4699, 20);
    tries = 0;
    if (state == true){
      count += 1;
    }
  } 

  // For removing the "entry denied" message from the LCD on the next button press
  if (next == 5) {
    if (state == true) {
      next = 1;
      lcd.setCursor(0,1);
      lcd.print("                ");
    }
  } 

  // For triggering the alarm after either exceeding the number of password attempts or interrupting the laser
  if (count >= 3) {
    if (repeat == true) {
      lcd.clear();
      lcd.print("Intruder");
      if (laserTrip == false) {
        lcd.setCursor(0,1);
        lcd.print("Passcode Wrong");
        xBee.println("treasure password");
      } else {
        lcd.setCursor(0,1);
        lcd.print("Laser Trip");
        xBee.println("laser trip"); 
      }
      repeat = false;
      //reset repeat to true when command center sends message back
    }
    tone(buzzer, 4699);
  }

  // For correct password entry - both activates and deactivates the system
  if (buf.endsWith(password) && state == true && count < 3) {
    resetSystem();
    lcd.print("System Off");
    digitalWrite(greenLED, HIGH);
    digitalWrite(redLED, LOW);
    state = false;
    xBee.println("treasure off");
  } else if (buf.endsWith(password) && state == false) {
    resetSystem();
    lcd.print("System On");
    digitalWrite(greenLED, LOW);
    digitalWrite(redLED, HIGH);
    state = true;
    xBee.println("treasure on");
  }
}

// Method for resetting important variables that deal with the keycode entry algorithm
void resetSystem() {
  noTone(buzzer);
  count = 0;
  lcd.clear();
  buf += "END";
  tries = 0;
  next = 0;
}

// Method for checking to ensure the laser is not obstructed (which would indicate an intruder)
int laserReading(int pValue) {
  int var = map(pValue, 0, 1024, 0, 255);
  if (var < 50 && state == true){
    laserTrip = true;
    count = 3;
    Serial.println("Laser break");
    Serial.println(var);
  } else{
    Serial.println("Laser fine");
    Serial.println(var);
  }
}
