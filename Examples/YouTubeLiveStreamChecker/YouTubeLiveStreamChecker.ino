/*******************************************************************
    Checks when a list of youtubers are live

    Checks every 5 seconds if one of your channels being watched
    is live. 5 seconds later it will check the next one.

    NOTE 1: This is almost certainly against YouTube ToS, so use at your own risk
    (Although there is nothing in the request that identifies you, they may limit or ban
    your IP address)

    NOTE 2: This could potentially use a decent amount of data as it makes
    lots of requests. (every 5 seconds, all day, that's + 17k requests)

    Parts:
    ----------
    D1 Mini ESP8266 * - http://s.click.aliexpress.com/e/uzFUnIe

      = Affilate

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
#define ARDUINOJSON_DECODE_UNICODE 1
// ----------------------------
// Standard Libraries
// ----------------------------

#include <SPI.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

// ----------------------------
// Additional Libraries - each one of these will need to be installed.
// ----------------------------

#include <TFT_eSPI.h>
// Graphics and font library for ST7789 driver chip

// Can be installed from the library manager (Search for "eSPI")
// https://github.com/Bodmer/TFT_eSPI

#include <YouTubeLiveStream.h>
// An arduino library for interacting with YouTube live streams.
// This is a work in progress library so may be updated/changed.

// Needs to be installed from Github
// https://github.com/witnessmenow/youtube-livestream-arduino

#include <ArduinoJson.h>
// Library used for parsing Json from the API responses

// Search for "Arduino Json" in the Arduino Library manager
// https://github.com/bblanchon/ArduinoJson

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

struct YTChannelDetails
{
  const char *id;
  const char *name;
  bool live;
};

//------- Replace the following! ------

char ssid[] = "SSID";         // your network SSID (name)
char password[] = "password"; // your network password


// Update this to match the number of channels you are following.
#define NUM_CHANNELS 9

// {"channel_ID", "name to appear", "current status"}
//
// Easiest way to get the channel ID is to click on a video,
// then click on their channel name from the video page,
// channel ID will be in the URL then.
//
// 16 (maybe 17) characters is the max that will fit for "name to appear"
//
// Current status should start as false, it will get updated if they are live.

YTChannelDetails channels[NUM_CHANNELS] = {
  {"UCUW49KGPezggFi0PGyDvcvg", "Zack Freedman", false}, // https://www.youtube.com/channel/UCUW49KGPezggFi0PGyDvcvg
  {"UCp_5PO66faM4dBFbFFBdPSQ", "Bitluni", false}, // https://www.youtube.com/channel/UCp_5PO66faM4dBFbFFBdPSQ
  {"UC8rQKO2XhPnvhnyV1eALa6g", "Bitluni's Trash", false}, // https://www.youtube.com/channel/UC8rQKO2XhPnvhnyV1eALa6g
  {"UCu94OHbBYVUXLYPh4NBu10w", "Unexpected Maker", false}, // https://www.youtube.com/channel/UCu94OHbBYVUXLYPh4NBu10w
  {"UCezJOfu7OtqGzd5xrP3q6WA", "Brian Lough", false}, // https://www.youtube.com/channel/UCezJOfu7OtqGzd5xrP3q6WA
  {"UCQmACVkilzq39wH9WW3jmyA", "Lough & Load", false}, // https://www.youtube.com/channel/UCQmACVkilzq39wH9WW3jmyA
  {"UCv7UOhZ2XuPwm9SN5oJsCjA", "Intermit.Tech", false}, // https://www.youtube.com/channel/UCv7UOhZ2XuPwm9SN5oJsCjA
  {"UCllpBTH26_dAl5tYl7vA1TQ", "Defpom", false}, // https://www.youtube.com/channel/UCllpBTH26_dAl5tYl7vA1TQ
  {"UC3yasGCIt1rmKnZ8PukocSw", "Simple Elect", false} // Simple Electronics - https://www.youtube.com/channel/UC3yasGCIt1rmKnZ8PukocSw
};


//------- ---------------------- ------

WiFiClientSecure client;
YouTubeLiveStream ytVideo(client, NULL); //"scrapeIsChannelLive" doesn't require a key.

unsigned long delayBetweenRequests = 5000; // Time between requests (5 seconds)
unsigned long requestDueTime;               //time when request due

int channelIndex = 0;

void setup(void) {

  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //client.setFingerprint(SLACK_FINGERPRINT);
  client.setInsecure();

  tft.init();
  tft.setRotation(3); // Needs to be rotated 3 to look correct on the shield

  tft.fillScreen(TFT_BLACK);

  // D8 mode needs to be set after tft.init
  pinMode(D8, INPUT); // is pulled down, will be high when pressed
  pinMode(D3, INPUT); // is pulled low, will be low when pressed

  //D4 is the backlight
  pinMode(D4, OUTPUT);

  //D4 defaults to LOW (Black Screen)
  //You will need to turn it on using
  //analogWrite or digitalWrite

  //PWM to ~half brightness
  analogWrite(D4, 500);
  //digitalWrite(D4, HIGH); //full brightness

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.drawCentreString("Live Now:", 120, 5, 4);
}

int screenChange = false;

void loop() {

  if (millis() > requestDueTime)
  {
    if (ytVideo.scrapeIsChannelLive(channels[channelIndex].id)) {
      if (!channels[channelIndex].live)
      {
        channels[channelIndex].live = true;
        screenChange = true;
      }
      Serial.print(channels[channelIndex].name);
      Serial.println(" is live");
      requestDueTime = millis() + delayBetweenRequests;
    } else {
      if (channels[channelIndex].live)
      {
        channels[channelIndex].live = false;
        screenChange = true;
      }
      Serial.print(channels[channelIndex].name);
      Serial.println(" is NOT live");
      requestDueTime = millis() + 5000;
    }
    channelIndex++;
    if (channelIndex >= NUM_CHANNELS)
    {
      channelIndex = 0;
    }
  }

  if (screenChange) {
    screenChange = false;
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.drawCentreString("Live Now:", 120, 5, 4);
    int yOffset = 30;
    for (int i = 0; i < NUM_CHANNELS; i++) {
      if (channels[i].live) {
        tft.drawCentreString(channels[i].name, 120, yOffset, 4);
        yOffset += 30;
      }
    }
  }

  // If D3 is LOW, button is pressed
  if (digitalRead(D3) == LOW)
  {
    Serial.println("D3 Pressed");
    digitalWrite(D4, HIGH);
    delay(100);
  }

  // If D8 is HIGH, button is pressed
  if (digitalRead(D8) == HIGH)
  {
    Serial.println("D8 Pressed");
    digitalWrite(D4, LOW);
    delay(100);
  }

}
