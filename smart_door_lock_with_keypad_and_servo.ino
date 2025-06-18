#include <Keypad.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Setting up LCD 
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Setup Servo
Servo myServo;

// Setup Keypad
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {2, 3, 4, 5}; 
byte colPins[COLS] = {6, 7, 8, 9}; 

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


const int servoPin = 10;
const int buzzerPin = 11;

String secretCode = "1379";
String enteredCode = "";

void setup() {
  // Start LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("ENTER CODE:");

  // Servo setup
  myServo.attach(servoPin);
  myServo.write(0); // Locked

  // Buzzer setup
  pinMode(buzzerPin, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    Serial.print("Key: ");
    Serial.println(key);

    if (key == '#') {
      if (enteredCode == secretCode) {
        lcd.clear();
        lcd.print("UNLOCKED!");
        myServo.write(90); // Open
        delay(3000);
        myServo.write(0);  // Lock again
        lcd.clear();
        lcd.print("ENTER CODE:");
      } else {
        lcd.clear();
        lcd.print("WRONG!");
        tone(buzzerPin, 1000, 1000);
        delay(1000);
        lcd.clear();
        lcd.print("ENTER CODE:");
      }
      enteredCode = ""; // Reset code
    } 
    else if (key == '*') {
      enteredCode = "";
      lcd.clear();
      lcd.print("CLEARED");
      delay(500);
      lcd.clear();
      lcd.print("ENTER CODE:");
    } 
    else {
      enteredCode += key;
      lcd.setCursor(0, 1);
      lcd.print("Code: ");
      for (unsigned int i = 0; i < enteredCode.length(); i++) lcd.print("*");
    }
  }
}
