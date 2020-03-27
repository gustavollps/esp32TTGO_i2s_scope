enum Option {
  None,
  Vdiv,
  Sdiv,
  Offset,
  Filter,
  Stop,
  Reset,
  Probe,
  UpdateF,
  Cursor1,
  Cursor2
};

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
float time_division[13] = { //screen has 4 divisions, 60 pixel each (240 pixel of width)
  10,
  25,
  50,
  100,
  250,
  500,
  1000,
  2500,
  5000,   //1Mhz    35ms of data (of 50ms possible)
  10000,  //100khz  70ms/500ms
  25000,  //100khz  175ms/500ms of data
  50000,  //100khz  350ms/500ms of data
  100000  //50khz   700ms/1000ms of data
};

uint8_t volts_index = 0;

uint8_t tscale_index = 0;

uint8_t opt = None;

bool menu = false;

float sample_rate = 1000; //in ksps --> 1000 = 1Msps

void menu_handler() {
  /* set V/div
     set S/div
     set offset
  */
  button_mode.loop();
  button_set.loop();
}

void click(Button2& btn) {

  if (btn == button_mode) {
    opt++;
    if (opt > Reset)
      opt = None;
    if (opt == None)
      hide_menu();
    else
      show_menu();

  }
  else if (btn == button_set) {
    switch (opt) {
      case None:
        //Change channel color?
        break;

      case Vdiv:
        show_menu();
        volts_index++;
        if (volts_index >= sizeof(voltage_division) / sizeof(*voltage_division))
          volts_index = 0;

        v_div = voltage_division[volts_index];
        break;

      case Sdiv:
        show_menu();
        tscale_index++;
        if (tscale_index >= sizeof(time_division) / sizeof(*time_division))
          tscale_index = 0;
        
        s_div = time_division[tscale_index];

//TODO Crashing WHY        
//        if(s_div > 5000 && s_div < 50000 && sample_rate != 100){
//          sample_rate = 100;
//          set_sample_rate(sample_rate*1000);          
//        }
//        else if(s_div >= 50000 && sample_rate!=50){
//          sample_rate = 50;
//          set_sample_rate(sample_rate*1000); 
//        }
//        else if(s_div <= 5000 && sample_rate != 1000){
//          sample_rate = 1000;
//          set_sample_rate(sample_rate*1000);
//        }
        break;

      case Offset:
        offset += 0.1 * (v_div * 4) / 3300;
        if (offset > 3.3)
          offset = 0;
        break;

      case Reset:
        offset = 0;
        v_div = 825;
        s_div = 50;
        tscale_index = 0;
        volts_index = 0;
        break;

      case Probe:
        break;

      case Stop:
        stop = !stop;
        break;

      case Filter:
        current_filter++;
        if (current_filter > 2)
          current_filter = 0;

        break;

      case Cursor1:
        break;

      case Cursor2:
        break;

      default:
        break;

    }
  }
}

void hide_menu() {
  menu = false;
}

void show_menu() {
  menu = true;
}
