# esp32TTGO_i2s_scope
A simple 1Msps single channel scope with the TFT display on the ESP32-TTGO.

* Features v0.1
  * 1Msps max
  * 50000 16bits buffer (50ms of data at 1Msps)
  * Scale from 10us/div to 5ms/div at 1Msps
  * Using 2 onboard buttons for control
  * Frequency calculations (20hz resolution at 1Msps)
  * Simple mean filter ON/OFF
  * Max, min, average and Peak-Peak voltage

* TODO
  * Code commenting and structuring (not very reader-friendly right now)
  * Trigger features
  * Time offset
  * Fix unfunctional low-pass filter
  * Scale from 10ms/div to 50ms/div at 100ksps
  * Scale from 100ms/div to 5ms/div at 1Msps
  * External circuit/voltage divider to read negative/higher voltages
  * Probe configuration menu
  * Cursors
  * 2nd channel (probably lowering the frequency considerably)  

![Sine](https://github.com/gustavollps/esp32TTGO_i2s_scope/blob/master/imgs/sine%20wave.jpeg)
![Sine](https://github.com/gustavollps/esp32TTGO_i2s_scope/blob/master/imgs/sine%20zoomed.jpeg)
![Sine](https://github.com/gustavollps/esp32TTGO_i2s_scope/blob/master/imgs/pwm.jpeg)
