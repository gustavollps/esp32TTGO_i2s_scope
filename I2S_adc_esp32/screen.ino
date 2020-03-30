void setup_screen() {
  // Initialise the TFT registers
  tft.init();
  tft.setRotation(1);

  // Optionally set colour depth to 8 or 16 bits, default is 16 if not spedified
  // spr.setColorDepth(8);

  // Create a sprite of defined size
  spr.createSprite(HEIGHT, WIDTH);

  // Clear the TFT screen to blue
  tft.fillScreen(TFT_BLACK);
}

int data[240] = {0};

float to_scale(float reading) {
  float temp = WIDTH -
               (
                 (
                   (
                     ((reading - min_v) / delta)
                     + (offset / 3.3)
                   )
                   * 3300 /
                   (v_div * 4)
                 )
               )
               * (WIDTH - 1)
               - 1;
  return temp;
}

float to_voltage(float reading) {
  return  (reading - min_v) / delta * 3.3;
}

void update_screen(uint16_t *i2s_buff) {
  // Fill the whole sprite with black (Sprite is in memory so not visible yet)
  spr.fillSprite(TFT_BLACK);


  float mean = 0;
  mean = mean_calc(i2s_buff);

  float max_v, min_v;
  peak_peak(i2s_buff, BUFF_SIZE, &max_v, &min_v);
  float peak = to_voltage(max_v) - to_voltage(min_v);

  float freq = 0;
  float period = 0;
  uint32_t trigger0 = 0;
  uint32_t trigger1 = 0;

  trigger_freq_calc(i2s_buff, mean, &freq, &period, &trigger0, &trigger1);

  String frequency = "";
  if (freq < 1000)
    frequency = String(freq) + "hz";
  else if (freq < 100000)
    frequency = String(freq / 1000) + "khz";
  else
    frequency = "----";

  String s_mean = "";
  if (mean > 1.0)
    s_mean = "Avg: " + String(mean) + "V";
  else
    s_mean = "Avg: " + String(mean * 1000.0) + "mV";

  String str_filter = "";
  if (current_filter == 0)
    str_filter = "None";
  else if (current_filter == 1)
    str_filter = "Pixel";
  else if (current_filter == 2)
    str_filter = "Mean-5";
  else if (current_filter == 3)
    str_filter = "Lpass9";

  String str_stop = "";
  if (!stop)
    str_stop = "RUNNING";
  else
    str_stop = "STOPPED";


  draw_grid();

  if (auto_scale) {
    auto_scale = false;
    v_div = 1000.0 * to_voltage(max_v) / 4.0;
    s_div = period / 3.5;
    if (s_div > 5000)
      s_div = 5000;

  }
  draw_channel1(trigger0, trigger1, i2s_buff);

  int shift = 150;
  if (menu) {

    spr.fillRect(shift, 0, 240, 135, TFT_BLACK);
    spr.drawRect(shift, 0, 240, 135, TFT_WHITE);
    spr.fillRect(shift + 1, 3 + 10 * (opt - 1), 100, 11, TFT_RED);

    spr.drawString("AUTOSCALE",  shift + 5, 5);
    spr.drawString(String(int(v_div)) + "mV/div",  shift + 5, 15);
    spr.drawString(String(int(s_div)) + "uS/div",  shift + 5, 25);
    spr.drawString("Offset: " + String(offset) + "V",  shift + 5, 35);
    spr.drawString("T-Off: " + String((uint32_t)toffset) + "uS",  shift + 5, 45);
    spr.drawString("Filter: " + str_filter, shift + 5, 55);
    spr.drawString(str_stop, shift + 5, 65);
    spr.drawString("----RESET----", shift + 5, 75);

    spr.drawString("Vmax: " + String(to_voltage(max_v)) + "V",  shift + 5, 85);
    spr.drawString("Vmin: " + String(to_voltage(min_v)) + "V",  shift + 5, 95);
    spr.drawString("P-P: " + String(peak) + "V",  shift + 5, 105);
    spr.drawString(s_mean,  shift + 5, 115);
    spr.drawString(frequency,  shift + 5, 125);
  }
  //  else {
  //    spr.fillRect(shift, 0, 240 - shift, 30, TFT_BLACK);
  //    spr.drawRect(shift, 0, 240 - shift, 30, TFT_WHITE);
  //  }


  //push the drawed sprite to the screen
  spr.pushSprite(0, 0);

#ifdef DEBUG_SERIAL
  for (int i = 0; i < NUM_SAMPLES; i++) {
    Serial.println(float(i2s_buff[i] - min_v) / delta * 3.3);
  }
#endif
  //Serial.println("ALIVE!");
  yield(); // Stop watchdog reset
}

void draw_grid() {
  //axis drawing
  //spr.drawLine(10, 0, 10, 240, TFT_WHITE);
  spr.drawLine( 0, 67, 240, 67, TFT_WHITE); //center line
  for (int i = 0; i < 24; i++) {
    spr.drawPixel(i * 10, 33, TFT_WHITE);
    spr.drawPixel(i * 10, 101, TFT_WHITE);
  }
  for (int i = 0; i < 135; i += 10) {
    for (int j = 0; j < 240; j += 34) {
      spr.drawPixel(j, i, TFT_WHITE);
    }
  }
}

void draw_channel1(uint32_t trigger0, uint32_t trigger1, uint16_t *i2s_buff) {
  //screen wave drawing
  data[0] = to_scale(i2s_buff[trigger0]);
  low_pass filter(0.99);
  mean_filter mfilter(5);
  mfilter.init(i2s_buff[trigger0]);
  filter._value = i2s_buff[trigger0];
  float data_per_pixel = (s_div / 34.0) / (sample_rate / 1000);

  //  uint32_t cursor = (trigger1-trigger0)/data_per_pixel;
  //  spr.drawLine(cursor, 0, cursor, 135, TFT_RED);

  uint32_t index_offset = (uint32_t)(toffset / data_per_pixel);
  trigger0 += index_offset;
  uint32_t total_data_points = (int)(240.0 * data_per_pixel);
  uint32_t old_index = trigger0;
  float n_data = 0, o_data = to_scale(i2s_buff[trigger0]);
  for (uint32_t i = 1; i < 240; i++) {
    uint32_t index = trigger0 + (uint32_t)((i + 1) * data_per_pixel);
    if (index < BUFF_SIZE) {
      if (full_pix && s_div > 34 && current_filter == 0) {
        uint32_t max_val = i2s_buff[old_index];
        uint32_t min_val = i2s_buff[old_index];
        for (int j = old_index; j < index; j++) {
          //draw lines for all this data points on pixel i
          if (i2s_buff[j] > max_val)
            max_val = i2s_buff[j];
          else if (i2s_buff[j] < min_val)
            min_val = i2s_buff[j];

        }
        spr.drawLine(i, to_scale(min_val), i, to_scale(max_val), TFT_BLUE);
      }
      else {        
        if (current_filter == 2)
          n_data = to_scale(mfilter.filter((float)i2s_buff[index]));
        else if (current_filter == 3)
          n_data = to_scale(filter.filter((float)i2s_buff[index]));
        else
          n_data = to_scale(i2s_buff[index]);
          
        spr.drawLine(i - 1, o_data, i, n_data, TFT_BLUE);
        o_data = n_data;
      }

    }
    else {
      break;
    }
    old_index = index;
  }
}
