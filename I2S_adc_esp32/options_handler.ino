int voltage_division[6] = { //screen has 4 divisions, 31 pixels each (125 pixels of height)
  825, //fullscreen 3.3V peak-peak
  750,
  500,
  250,
  100,
  50
};


/*each sample represents 1us (1Msps),
   thus, the time division is the number
   of samples per screen division
*/
float time_division[9] = { //screen has 4 divisions, 60 pixel each (240 pixel of width)
  10,
  25,
  50,
  100,
  250,
  500,
  1000,
  2500,
  5000
};
//,   //1Mhz    35ms of data (of 50ms possible)
//  10000,  //100khz  70ms/500ms
//  25000,  //100khz  175ms/500ms of data
//  50000,  //100khz  350ms/500ms of data
//  100000  //50khz   700ms/1000ms of data
//};

void menu_handler() {
  button_mode.loop();
  button_set.loop();
}

void click_long(Button2& btn) {
  menu_action = true;
  if (btn == button_mode) {
    uint32_t pressed = btn.wasPressedFor();
    if (pressed > 1000) {
      opt = None;
      hide_menu();
      set_value = false;
    }

  }
  else {
    uint32_t pressed = btn.wasPressedFor();
    if (pressed > 1000) {
      if (set_value) {
        set_value = false;
      }
      else {
        switch (opt) {
          case Vdiv:
            v_div = 825;
            volts_index = 0;
            break;
            
          case Sdiv:
            s_div = 10;
            tscale_index = 0;
            break;
            
          case Offset:
            offset = 0;
            break;
            
          case TOffset:
            toffset = 0;
            break;
          case Filter:
            current_filter = 1;
            break;
            
          case None:
            info = !info;
            break;
            
          default:
            break;
        }
      }
    }
  }
}

void click(Button2& btn) {
  menu_action = true;

  if (set_value) {
    switch (opt) {
      case Vdiv:
        if (btn == button_set) {
          volts_index++;
          if (volts_index >= sizeof(voltage_division) / sizeof(*voltage_division)) {
            volts_index = 0;
          }
        }
        else {
          volts_index--;
          if (volts_index < 0) {
            volts_index = sizeof(voltage_division) / sizeof(*voltage_division) - 1;
          }
        }

        v_div = voltage_division[volts_index];
        break;

      case Sdiv:
        if (btn == button_mode) {
          tscale_index++;
          if (tscale_index >= sizeof(time_division) / sizeof(*time_division)) {
            tscale_index = 0;
          }
        }
        else {
          tscale_index--;
          if (tscale_index < 0) {
            tscale_index = sizeof(time_division) / sizeof(*time_division) - 1;
          }
        }

        s_div = time_division[tscale_index];
        break;

      case Offset:
        if (btn == button_mode){
          offset += 0.1 * (v_div * 4) / 3300;
        }        
        else{
          offset -= 0.1 * (v_div * 4) / 3300;
        }

        if (offset > 3.3)
          offset = 3.3;
        if (offset < -3.3)
          offset = -3.3;

        break;

      case TOffset:
        if (btn == button_mode)
          toffset += 0.1 * s_div;
        else
          toffset -= 0.1 * s_div;

        break;

      default:
        break;

    }
  }
  else {
    if (btn == button_mode) {
      opt++;
      if (opt > Single)
        opt = None;
      if (opt == None)
        hide_menu();
      else
        show_menu();

    }
    else if (btn == button_set) {
      switch (opt) {
        case Autoscale:
          auto_scale = !auto_scale;
          break;

        case Vdiv:
          set_value = true;
          break;

        case Sdiv:
          set_value = true;
          break;

        case Offset:
          set_value = true;
          break;

        case Stop:
          stop = !stop;
          break;

        case TOffset:
          set_value = true;
          break;

        case Single:
          single_trigger = true;
          break;

        case Reset:
          offset = 0;
          v_div = 825;
          s_div = 10;
          tscale_index = 0;
          volts_index = 0;
          break;

        case Probe:
          break;

        case Mode:
          digital_wave_option++;
          if (digital_wave_option > 2)
            digital_wave_option = 0;
          break;

        case Filter:
          current_filter++;
          if (current_filter > 3)
            current_filter = 0;
          break;

        default:
          break;

      }
    }
  }
}

void hide_menu() {
  menu = false;
}

void hide_all() {
  menu = false;
  info = false;
}

void show_menu() {
  menu = true;
}

String strings_vdiv() {
  return "";
}

String strings_sdiv() {
  return "";
}

String strings_offset() {
  return "";
}

String strings_toffset() {
  return "";
}

String strings_freq() {
  return "";
}

String strings_peak() {
  return "";
}

String strings_vmax() {
  return "";
}

String strings_vmin() {
  return "";
}

String strings_filter() {
  return "";
}
