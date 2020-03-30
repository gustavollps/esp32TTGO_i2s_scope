void peak_peak(uint16_t *vector, uint32_t len, float * max_value, float * min_value) {
  max_value[0] = vector[0];
  min_value[0] = vector[0];
  mean_filter filter(5);
  filter.init(vector[0]);

  for (uint32_t i = 1; i < len; i++) {
    float value = filter.filter((float)vector[i]);
    if (value > max_value[0])
      max_value[0] = value;
    if (value < min_value[0])
      min_value[0] = value;
  }
}

float mean_calc(uint16_t *i2s_buffer) {
  //data mean calculation
  float mean = 0;
  for (int i = 0; i < BUFF_SIZE; i++) {
    mean += i2s_buffer[i];
  }
  mean /= float(BUFF_SIZE);
  mean = to_voltage(mean);
  return mean;
}


void trigger_freq_calc(uint16_t *i2s_buffer, float mean, float *pt_freq, float *pt_period, uint32_t *pt_trigger0, uint32_t *pt_trigger1) {
  float freq = 0;
  float period = 0;
  bool signal_side = false;
  uint32_t trigger_count = 0;
  uint32_t trigger_num = 10;
  uint32_t trigger_temp[trigger_num] = {0};
  uint32_t trigger_index = 0;

  //get initial signal relative to the mean
  if (to_voltage(i2s_buffer[0]) > mean) {
    signal_side = true;
  }

  //waveform repetitions calculation + get triggers time
  for (uint32_t i = 1 ; i < BUFF_SIZE; i++) {
    if (signal_side && to_voltage(i2s_buffer[i]) < mean - (mean - to_voltage(min_v)) * 0.2) {
      signal_side = false;
    }
    else if (!signal_side && to_voltage(i2s_buffer[i]) > mean + (to_voltage(max_v) - mean) * 0.2) {
      freq++;
      if (trigger_count < trigger_num) {
        trigger_temp[trigger_count] = i;
        trigger_count++;
      }
      signal_side = true;
    }
  }


  //frequency calculation
  if (trigger_count < 2) {
    trigger_temp[0] = 0;
    trigger_index = 0;
    freq = 0;
    period = 0;
  }
  else {

    //simple frequency calculation fair enough for frequencies over 2khz
    freq = freq * 1000 / 50;
    period = (float)(sample_rate * 1000.0) / freq; //us

    //from 2000 to 80 hz -> uses mean of the periods for precision
    if (freq < 2000 && freq > 80) {
      period = 0;
      for (uint32_t i = 1; i < trigger_count; i++) {
        period += trigger_temp[i] - trigger_temp[i - 1];
      }
      period /= (trigger_count - 1);
      freq = sample_rate * 1000 / period;
    }

    //under 80hz, single period for frequency calculation
    else if (trigger_count > 1 && freq <= 80) {
      period = trigger_temp[1] - trigger_temp[0];
      freq = sample_rate * 1000 / period;
    }
  }
  
  //setting triggers offset and getting second trigger for debug cursor on drawn_channel1
  /*
     The trigger function uses a rise porcentage (5%) obove the mean, thus,
     the real waveform starting point is some datapoints back.
     The resulting trigger gets a negative offset of 5% of the calculated period
  */
  uint32_t trigger2 = 0;
  if (trigger_temp[0] - period * 0.05 > 0 && trigger_count > 1) {
    trigger_index = trigger_temp[0] - period * 0.05;
    trigger2 = trigger_temp[1] - period * 0.05;
  }
  else if (trigger_count > 2) {
    trigger_index = trigger_temp[1] - period * 0.05;
    if (trigger_count > 2)
      trigger2 = trigger_temp[2] - period * 0.05;
  }



  pt_trigger0[0] = trigger_index;
  pt_trigger1[0] = trigger2;
  pt_freq[0] = freq;
  pt_period[0] = period;

}
