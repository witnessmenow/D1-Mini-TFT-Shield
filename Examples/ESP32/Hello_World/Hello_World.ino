/*******************************************************************
    Simple "hello world" with some simple graphics for the shield

    For a more comprehensive graphics demo, check out this example
    https://github.com/Bodmer/TFT_eSPI/blob/master/examples/160%20x%20128/TFT_graphicstest_small/TFT_graphicstest_small.ino

    The buttons control turning on and off the backlight

    Parts:
    ----------
    D1 Mini ESP8266 * - http://s.click.aliexpress.com/e/uzFUnIe

    * = Affilate

    Support what I do!
    -----------------
    If you find what I do useful and would like to support me,
    please consider becoming a sponsor on Github
    https://github.com/sponsors/witnessmenow/


    Written by Brian Lough
    YouTube: https://www.youtube.com/brianlough
    Tindie: https://www.tindie.com/stores/brianlough/
    Twitter: https://twitter.com/witnessmenow
 *******************************************************************/

// ----------------------------
// Standard Libraries
// ----------------------------

#include <SPI.h>

// ----------------------------
// Additional Libraries - each one of these will need to be installed.
// ----------------------------

#include <TFT_eSPI.h>
// Graphics and font library for ST7789 driver chip

// Can be installed from the library manager (Search for "eSPI")
// https://github.com/Bodmer/TFT_eSPI

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

void setup(void) {

  Serial.begin(115200);
  tft.init();
  tft.setRotation(3); // Needs to be rotated 3 to look correct on the shield

  tft.fillScreen(TFT_BLACK);

  // D8 mode needs to be set after tft.init
  pinMode(5, INPUT); // is pulled down, will be high when pressed
  pinMode(17, INPUT); // is pulled low, will be low when pressed

  //D4 is the backlight
  pinMode(16, OUTPUT);

  //D4 defaults to LOW (Black Screen)
  //You will need to turn it on using
  //analogWrite or digitalWrite

  //PWM to ~half brightness
  //analogWrite(16, 500);
  digitalWrite(16, HIGH); //full brightness

  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setCursor (2, 2);
  tft.print("Hello World!"); // This uses the standard ADAFruit small font

  tft.setTextColor(TFT_BLUE, TFT_BLACK);
  tft.drawCentreString("Hello World!", 120, 10, 2); // Centered, size 2 font

  tft.setTextColor(TFT_PINK, TFT_BLACK);
  tft.drawRightString("Hello World!", 240, 30, 4); // Right orientated, size 4 font

  tft.fillRect(40, 70, 160, 160, TFT_ORANGE);
  tft.drawRect(40, 70, 160, 160, TFT_GREEN);

  tft.fillCircle(120, 150, 60, TFT_BLUE);
  tft.drawCircle(120, 150, 60, TFT_BLACK);
  tft.drawCircle(120, 150, 59, TFT_BLACK);

  tft.setTextColor(TFT_RED, TFT_BLUE);
  tft.drawCentreString("HI!", 120, 140, 4); // Centered, size 4 font
}

void loop() {

  // If D3 is LOW, button is pressed
  if (digitalRead(17) == LOW)
  {
    Serial.println("D3 Pressed");
    digitalWrite(16, HIGH);
    delay(100);
  }

  // If D8 is HIGH, button is pressed
  if (digitalRead(5) == HIGH)
  {
    Serial.println("D8 Pressed");
    digitalWrite(16, LOW);
    delay(100);
  }

}
