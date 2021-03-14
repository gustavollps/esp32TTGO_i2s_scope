esp32TTGO_i2s_scope
====================
DEMO VIDEO at https://youtu.be/4NNk67PA57Y

Feel free to give me any suggestions. It is a simple work that supplied what I needed at the time and has much room for improvements.

# Project Description

A simple 1Msps single-channel scope with the TFT display on the ESP32-TTGO.(https://bit.ly/2WbaXDi)

#### Features v0.3
  * Single-channel
  * 1Msps
  * 50000 @ 16bits buffer (50ms of data at 1Msps)
  * Scale from 10us/div to 5ms/div at 1Msps
  * Using 2 onboard buttons for VERY slow control (dreaming of a rotational encoder)
  * Frequency calculations (20hz min due to buffer size)
  * Simple mean filter ON/OFF
  * Max, min, average and Peak-Peak voltage
  * Time and voltage offset
  * Analog, Digital/Data Mode
  * Single TRIGGER
  * AUTOSCALE

#### Tested and working on:
* PWM
* 80Khz sine wave
* 40hz sine wave
* Serial at 115200bps
* Electric Guitar Raw signal (from 82hz to 1.3khz, <100mV peak to peak)

**KEEP IN MIND TO PROTECT YOUR ADC PORT**

# Building

First of all, you should have your IDE with the ESP32 support installed.
Add the Button2 library and the TFT_eSPI library (By Bodmer) to the IDE, then edit the library to work with the correct display as described below:

In the file `TFT_eSPI/User_Setup_Select.h` change the line:

`#include <User_Setup.h>` to: `//#include <User_Setup.h>`

and 

`//#include <User_Setups/Setup25_TTGO_T_Display.h>` to `#include <User_Setups/Setup25_TTGO_T_Display.h>`

The board is the ESP32 Wrover Module.
With this, you should be able to build it successfully.


# Button Operation

Firstly, you should change the defines INSIDE the button2.h library to:
```
#ifndef DEBOUNCE_MS
  #define DEBOUNCE_MS 20
#endif
#ifndef LONGCLICK_MS
  #define LONGCLICK_MS 500
#endif
``` 
Defining it in the main .ino file did nothing at all for me, probably due to the Arduino IDE thing with the .ino files organization. This is closer to the parameters used (that I sadly lost) in the DEMO video.

The DEMO video shows better the working flow.

- Upper button
  - At the initial screen 
    - Short click - open menu
  - On menu
    - Short click - next option
    - Long click - Hide menu
  - In menu with an option selected
    - Short click - Increase the selected option's value (`+` symbol appearing at the right upper border of the display)
    - Long click - Hide menu

- Lower button
  - At the initial screen
    - Long click - hide all graphics except the grid
  - In menu
    - Short click - select highlighted option or enter the value-changing state for the option, if available
    - Long click - reset highlighted option to the default value
  - In menu with an option selected
    - Short click - Decrease the selected option's value (`-` symbol appearing at the right upper border of the display)
    - Long click - exit value-changing state for the highlighted option
  
# Version history

* v0.1 simple bugged scope
* v0.2 Trigger added
* v0.3 Frequency calculation improved, trigger improved, overall improvements

![scope](https://github.com/gustavollps/esp32TTGO_i2s_scope/blob/master/imgs/P_20200508_000602_vHDR_On.jpg)
