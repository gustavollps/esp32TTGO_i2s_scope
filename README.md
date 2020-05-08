esp32TTGO_i2s_scope
====================
[DEMO VIDEO COMMING SOON]
Fell free to give me any sugestions, it is a simple work that supplied what I needed at the time and it has much to improve.
A simple 1Msps single channel scope with the TFT display on the ESP32-TTGO.

* Features v0.3
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

Tested and working on:
* PWM
* 80Khz sine wave
* 40hz sine wave
* Serial at 115200bps
* Electric Guitar Raw singal (from 82hz to 1.3khz, <100mV peak to peak)

**KEEP IN MIND TO PROTECT YOUR ADC PORT**

Version history
-----------------
* v0.1 simple bugged scope
* v0.2 Trigger added
* v0.3 Frequency calculation improved, trigger improved, overall improvements
