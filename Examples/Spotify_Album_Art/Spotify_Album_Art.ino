/*******************************************************************
    Displays Album Art on a ST7789

    This example could easily be adapted to any Adafruit GFX
    based screen.

    NOTE: You need to get a Refresh token to use this example
    Use the getRefreshToken example to get it.

    Parts:
    D1 Mini ESP8266 * - http://s.click.aliexpress.com/e/uzFUnIe
    ST7789 TFT Screen

 *  * = Affilate

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

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#define FS_NO_GLOBALS
#include <FS.h>
//#include <LittleFS.h>

#include <SPI.h>

// ----------------------------
// Additional Libraries - each one of these will need to be installed.
// ----------------------------

#include <TFT_eSPI.h>
// Graphics and font library for ST7789 driver chip

// Can be installed from the library manager (Search for "eSPI")
// https://github.com/Bodmer/TFT_eSPI

#include <ArduinoSpotify.h>
// Library for connecting to the Spotify API

// Install from Github
// https://github.com/witnessmenow/arduino-spotify-api

#include <ArduinoJson.h>
// Library used for parsing Json from the API responses

// Search for "Arduino Json" in the Arduino Library manager
// https://github.com/bblanchon/ArduinoJson

#include <TJpg_Decoder.h>
// Library for decoding Jpegs from the API responses

// Search for "tjpg" in the Arduino Library manager
// https://github.com/Bodmer/TJpg_Decoder

//------- Replace the following! ------

char ssid[] = "SSID";         // your network SSID (name)
char password[] = "password"; // your network password

char clientId[] = "56t4373258u3405u43u543"; // Your client ID of your spotify APP
char clientSecret[] = "56t4373258u3405u43u543"; // Your client Secret of your spotify APP (Do Not share this!)

// Country code, including this is advisable
#define SPOTIFY_MARKET "IE"

#define SPOTIFY_REFRESH_TOKEN "AAAAAAAAAABBBBBBBBBBBCCCCCCCCCCCDDDDDDDDDDD"


//------- ---------------------- ------

// file name for where to save the image.
#define ALBUM_ART "/album.jpg"

// so we can compare and not download the same image if we already have it.
String lastAlbumArtUrl;

WiFiClientSecure client;
ArduinoSpotify spotify(client, clientId, clientSecret, SPOTIFY_REFRESH_TOKEN);

// You might want to make this much smaller, so it will update responsively

unsigned long delayBetweenRequests = 30000; // Time between requests (30 seconds)
unsigned long requestDueTime;               //time when request due

TFT_eSPI tft = TFT_eSPI();



// This next function will be called during decoding of the jpeg file to
// render each block to the Display.  If you use a different display
// you will need to adapt this function to suit.
bool displayOutput(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap)
{
  // Stop further decoding as image is running off bottom of screen
  if ( y >= tft.height() ) return 0;

  // This function will clip the image block rendering automatically at the TFT boundaries
  tft.pushImage(x, y, w, h, bitmap);

  // Return 1 to decode next block
  return 1;
}

void setup() {

  Serial.begin(115200);

  // Initialise LittleFS, if this fails try .begin(true)
  // NOTE: I believe this formats it though it will erase everything on
  // LittleFS already! In this example that is not a problem.
  // I have found once I used the true flag once, I could use it
  // without the true flag after that.

  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS initialisation failed!");
    while (1) yield(); // Stay here twiddling thumbs waiting
  }
  Serial.println("\r\nInitialisation done.");

  // Initialise the TFT
  tft.begin();
  tft.setRotation(3); // Needs to be rotated 3 to look correct on the shield
  tft.setTextColor(0xFFFF, 0x0000);
  tft.fillScreen(TFT_BLACK);
  tft.setSwapBytes(true); // We need to swap the colour bytes (endianess)

  // The jpeg image can be scaled by a factor of 1, 2, 4, or 8
  TJpgDec.setJpgScale(2);

  TJpgDec.setCallback(displayOutput);

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);

  // Only avaible in ESP8266 V2.5 RC1 and above
  //client.setFingerprint(SPOTIFY_FINGERPRINT);
  client.setInsecure();

  // If you want to enable some extra debugging
  // uncomment the "#define SPOTIFY_DEBUG" in ArduinoSpotify.h
  delay(100);
  Serial.println("Refreshing Access Tokens");
  Serial.print("Free Heap: ");
  Serial.println(ESP.getFreeHeap());

  Serial.println("Refreshing Access Tokens");
  if (!spotify.refreshAccessToken()) {
    Serial.println("Failed to get access tokens");
  }

}
int displayImage(char *albumArtUrl) {

  // In this example I reuse the same filename
  // over and over, maybe saving the art using
  // the album URI as the name would be better
  // as you could save having to download them each
  // time, but this seems to work fine.
  if (SPIFFS.exists(ALBUM_ART) == true) {
    Serial.println("Removing existing image");
    SPIFFS.remove(ALBUM_ART);
  }

  fs::File f = SPIFFS.open(ALBUM_ART, "w+");
  if (!f) {
    Serial.println("file open failed");
    return -1;
  }

  bool gotImage = spotify.getImage(albumArtUrl, &f);

  // Make sure to close the file!
  f.close();

  if (gotImage) {
    Serial.print("Got Image");
    delay(1);
    return TJpgDec.drawFsJpg(45, 10, ALBUM_ART);
  } else {
    return -2;
  }
}

//void printCurrentlyPlayingToSerial(CurrentlyPlaying currentlyPlaying)
//{
//  if (!currentlyPlaying.error)
//  {
//    Serial.println("--------- Currently Playing ---------");
//
//
//    Serial.print("Is Playing: ");
//    if (currentlyPlaying.isPlaying)
//    {
//      Serial.println("Yes");
//    } else {
//      Serial.println("No");
//    }
//
//    Serial.print("Track: ");
//    Serial.println(currentlyPlaying.trackName);
//    Serial.print("Track URI: ");
//    Serial.println(currentlyPlaying.trackUri);
//    Serial.println();
//
//    Serial.print("Artist: ");
//    Serial.println(currentlyPlaying.firstArtistName);
//    Serial.print("Artist URI: ");
//    Serial.println(currentlyPlaying.firstArtistUri);
//    Serial.println();
//
//    Serial.print("Album: ");
//    Serial.println(currentlyPlaying.albumName);
//    Serial.print("Album URI: ");
//    Serial.println(currentlyPlaying.albumUri);
//    Serial.println();
//
//    // will be in order of widest to narrowest
//    // currentlyPlaying.numImages is the number of images that
//    // are stored
//    for (int i = 0; i < currentlyPlaying.numImages; i++) {
//      Serial.println("------------------------");
//      Serial.print("Album Image: ");
//      Serial.println(currentlyPlaying.albumImages[i].url);
//      Serial.print("Dimensions: ");
//      Serial.print(currentlyPlaying.albumImages[i].width);
//      Serial.print(" x ");
//      Serial.print(currentlyPlaying.albumImages[i].height);
//      Serial.println();
//    }
//
//    Serial.println("------------------------");
//  }
//}

void displayCurrentlyPlayingOnScreen(CurrentlyPlaying currentlyPlaying)
{
  //Clearing Text Area
  tft.fillRect(0, 160, 240, 80, TFT_BLACK);
  tft.setTextColor(TFT_LIGHTGREY, TFT_BLACK);
  tft.drawCentreString(currentlyPlaying.trackName, 120, 170, 2);
  tft.drawCentreString(currentlyPlaying.firstArtistName, 120, 188, 2);
  tft.drawCentreString(currentlyPlaying.albumName, 120, 206, 2);
  //    Serial.println(currentlyPlaying.trackName);
  //    Serial.print("Track URI: ");
  //    Serial.println(currentlyPlaying.trackUri);
  //    Serial.println();
  //
  //    Serial.print("Artist: ");
  //    Serial.println(currentlyPlaying.firstArtistName);
  //    Serial.print("Artist URI: ");
  //    Serial.println(currentlyPlaying.firstArtistUri);
  //    Serial.println();
  //
  //    Serial.print("Album: ");
  //    Serial.println(currentlyPlaying.albumName);
  //    Serial.print("Album URI: ");
  //    Serial.println(currentlyPlaying.albumUri);
  //    Serial.println();
  //
  //    // will be in order of widest to narrowest
  //    // currentlyPlaying.numImages is the number of images that
  //    // are stored
  //    for (int i = 0; i < currentlyPlaying.numImages; i++) {
  //      Serial.println("------------------------");
  //      Serial.print("Album Image: ");
  //      Serial.println(currentlyPlaying.albumImages[i].url);
  //      Serial.print("Dimensions: ");
  //      Serial.print(currentlyPlaying.albumImages[i].width);
  //      Serial.print(" x ");
  //      Serial.print(currentlyPlaying.albumImages[i].height);
  //      Serial.println();
  //    }
  //
  //    Serial.println("------------------------");
}

void loop() {
  if (millis() > requestDueTime)
  {
    Serial.print("Free Heap: ");
    Serial.println(ESP.getFreeHeap());

    Serial.println("getting currently playing song:");
    // Market can be excluded if you want e.g. spotify.getCurrentlyPlaying()
    CurrentlyPlaying currentlyPlaying = spotify.getCurrentlyPlaying(SPOTIFY_MARKET);
    if (!currentlyPlaying.error)
    {
      //printCurrentlyPlayingToSerial(currentlyPlaying);
      displayCurrentlyPlayingOnScreen(currentlyPlaying);

      // Smallest (narrowest) image will always be last.
      SpotifyImage smallestImage = currentlyPlaying.albumImages[1];
      String newAlbum = String(smallestImage.url);
      if (newAlbum != lastAlbumArtUrl) {
        Serial.println("Updating Art");
        int displayImageResult = displayImage(smallestImage.url);
        if (displayImageResult == 0) {
          lastAlbumArtUrl = newAlbum;
        } else {
          Serial.print("failed to display image: ");
          Serial.println(displayImageResult);
        }
      }


    }

    requestDueTime = millis() + delayBetweenRequests;
  }

}
