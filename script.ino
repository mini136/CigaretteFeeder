#include <Wire.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

RTC_DS3231 rtc;
LiquidCrystal_I2C lcd(0x27, 16, 2); // Zkontroluj I2C adresu tvého displeje
Servo myServo;

const int buzzerPin = 8;  // Zapoj buzzer na D8
const int servoPin = 9;   // Servo na D9

DateTime lastActionTime;

void setup() {
  Wire.begin();
  rtc.begin();
  lcd.begin();
  lcd.backlight();

  myServo.attach(servoPin);
  pinMode(buzzerPin, OUTPUT);

  if (rtc.lostPower()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // nastav čas podle kompilace
  }

  lastActionTime = rtc.now(); // uložíme počáteční čas
}

void loop() {
  DateTime now = rtc.now();

  // Zobraz čas na LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Time:");
  lcd.setCursor(0, 1);
  lcd.print(now.hour());
  lcd.print(":");
  if (now.minute() < 10) lcd.print("0");
  lcd.print(now.minute());

  // Zkontroluj, jestli uplynulo 8 hodin
  TimeSpan elapsed = now - lastActionTime;
  if (elapsed.totalseconds() >= 8 * 60 * 60) {
    // Pípnutí
    tone(buzzerPin, 1000, 500); // pípne 500 ms

    // Servo pohyb tam a zpět
    myServo.write(90);
    delay(1000);
    myServo.write(0);

    // Ulož nový čas
    lastActionTime = now;
  }

  delay(1000); // čekej 1 sekundu
}
