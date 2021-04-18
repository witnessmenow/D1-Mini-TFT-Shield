# D1-Mini-TFT-Shield

An open source shield for the ESP8266 D1 Mini with a ST7789 Colour TFT screen

![img](<https://cdn.tindiemedia.com/images/resize/9fYhuTP8_fl-3OeZqNDw4UVOPRc=/p/fit-in/774x516/filters:fill(fff)/i/528221/products/2020-11-12T16%3A35%3A22.633Z-IMG_20201112_160402.jpg>)

[Avilable to buy on my Tindie Store!](https://www.tindie.com/products/brianlough/d1-mini-tft-shield/) (Link may not be live yet)

## Help Support what I do!

My main contribution to the open source community to date has been my Arduino libraries for the ESP32 and ESP8266, but I recently been trying to contribute more certified open source hardware projects.

[If you enjoy my work, please consider becoming a Github sponsor!](https://github.com/sponsors/witnessmenow/)

## Features

The D1 Mini TFT Shield has the following features

- 240 x 240 ST7789 Colour TFT Display.
- Compact size due to SMD female headers.
- 2 push buttons
- All other available pins broken out to a header.

## Required Hardware

The D1 Mini TFT Shield requires a ESP8266 D1 Mini board. It is available as an add-on on the Tindie listing or the following:

- [Aliexpress\*](http://s.click.aliexpress.com/e/uzFUnIe)

**NOTE:** This is **NOT** compatible with the ESP32 board that is shaped like a D1 Mini! It will likely damage your ESP32. (The pads on the shield are duplicated on each side). It is possible to use stacking headers to connect it though.

\* = Affiliate

## Setup

The Shield will come assembled (other than the buttons). To use the shield, insert the D1 mini into the headers.

### Please note the orientation you insert it!

![img](<https://cdn.tindiemedia.com/images/resize/C6VKjNlau6LoIK_Dg67qAYbWLBc=/p/fit-in/774x516/filters:fill(fff)/i/528221/products/2020-11-12T16%3A35%3A22.633Z-IMG_20201112_160741.jpg>)

The notched corner of the shield should line up with the reset button of your D1 mini.

The USB of the D1 mini should be on the side of the shield where the display ribbon cable is (There is a "USB" mark on the PCB of the shield). The notched corner of the shield

## Software

I have been using this display with the [TFT_eSPI library by bodmer](https://github.com/Bodmer/TFT_eSPI). This can be installed through the library manager.

Once installed, you need to update the `User_Setup.h` file in the library folder to be one found in the config folder of this Repo.

You should then be able to try the examples that are in this repo.

Please note that using the buttons is a little strange as one is pulled up and the other is pulled down (internal boot pins of the ESP8266) and that D8 needs to configured after the `tft.init` call.

## Pins

#### Display

The display is powered from the D1 Mini 3.3V pin.

| ST7789     | D1 Mini | Note                                                                                  |
| ---------- | ------- | ------------------------------------------------------------------------------------- |
| COPI (SDA) | D7      |                                                                                       |
| DC         | D6      |                                                                                       |
| SCLK (SCL) | D5      |                                                                                       |
| RESET      | D0      |                                                                                       |
| CS         | GND     | This is connected to a 0 Ohm resistor at R4, so could be patched to a GPIO if needed. |
| Backlight  | D4      | This is Builtin LED pin of the ESP8266                                                |

#### Buttons

I wanted to avoid using the I2C pins for the buttons, so we are kind of in pin hell with the pins that are left for the buttons.

| D1 Mini | Note                                                                                                |
| ------- | --------------------------------------------------------------------------------------------------- |
| D8      | Pulled-Down, will be HIGH when pressed. Needs to be set as an input after `tft.init` (See examples) |
| D3      | Pulled-Up, will be LOW when pressed.                                                                |

#### Side Header Pins

The following pins are broken out to the pins at the side.

| D1 Mini  | Note                                                                           |
| -------- | ------------------------------------------------------------------------------ |
| A0       |                                                                                |
| D8       | Is also connected to a button                                                  |
| D3       | Is also connected to a button                                                  |
| D2 (SDA) | Not pulled up                                                                  |
| D1 (SCL) | Not pulled up                                                                  |
| RX       | [Can be used as a GPIO if needed](https://www.youtube.com/watch?v=uBvnvf35YJ8) |
| TX       | [Can be used as a GPIO if needed](https://www.youtube.com/watch?v=uBvnvf35YJ8) |
| 3.3V     |                                                                                |
| GND      |                                                                                |
| 5V       |                                                                                |

## Documentation License

Documentation licensed under the Creative Commons Attribution Share Alike 4.0 International License
