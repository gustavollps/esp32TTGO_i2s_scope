#include <Loop.h>
#include <Arduino.h>
#include <driver/i2s.h>
#include <driver/adc.h>
#include <soc/syscon_reg.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include "WiFi.h"
#include <Wire.h>
#include <Button2.h>
#include "esp_adc_cal.h"

//#define DEBUG_SERIAL
//#define DEBUG_BUFF
#define DELAY 1000

// Width and height of sprite
#define WIDTH  135
#define HEIGHT 240

#define ADC_CHANNEL   ADC1_CHANNEL_5  // GPIO33
#define NUM_SAMPLES   1000            // number of samples
#define I2S_NUM         (0)
#define BUFF_SIZE 50000
#define B_MULT BUFF_SIZE/NUM_SAMPLES
#define BUTTON_1        35
#define BUTTON_2        0

TFT_eSPI    tft = TFT_eSPI();         // Declare object "tft"

TFT_eSprite spr = TFT_eSprite(&tft);  // Declare Sprite object "spr" with pointer to "tft" object

Loop adc(3);
Loop screen(10);

Button2 button_mode = Button2(BUTTON_1);
Button2 button_set = Button2(BUTTON_2);

esp_adc_cal_characteristics_t adc_chars;

float min_v = 20480;
float max_v = 24575;
float delta = max_v - min_v; //4095 = 12bits

float v_div = 825;
float s_div = 10;
float offset = 0;
float toffset = 0;
uint8_t current_filter = 0;

uint16_t i2s_buff[BUFF_SIZE];
bool stop = false;

void setup() {

  Serial.begin(115200);

  configure_i2s(1000000);

  setup_screen();

  button_mode.setClickHandler(click);
  button_mode.setLongClickHandler(click_long);
  button_set.setClickHandler(click);
  button_set.setLongClickHandler(click_long);

#ifdef DEBUG_BUF
  debug_buffer();
#endif
}

static const inline void ADC_Sampling() {
  load_buffer();
}

bool stop_change = false;

void loop() {
  menu_handler();
  if (adc.ok()) {
    if (!stop) {
      if (stop_change) {
        i2s_adc_enable(I2S_NUM_0);
        stop_change = false;
      }
      ADC_Sampling();
    }
    else {
      if (!stop_change) {
        i2s_adc_disable(I2S_NUM_0);
        i2s_zero_dma_buffer(I2S_NUM_0);
        stop_change = true;
      }
    }
  }
  if (screen.ok()) {
    update_screen();
  }
}
