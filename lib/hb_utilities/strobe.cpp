#include "strobe.h"

unsigned long next_strobe = STROBE_OFF;
unsigned long strobe_delay = 0;
unsigned long strobe_duration = 100;
unsigned long strobe_off_time = 0;
bool strobe_state = false;


///////////////STROBE CONTROL//////////////////

void set_strobe_delay(unsigned long delay) {
  strobe_delay = delay;
  next_strobe = micros()+strobe_delay;
  if (strobe_delay == STROBE_OFF){
	  strobe_off_time == STROBE_OFF;
  }
  //strobe_off_time = next_strobe + strobe_duration;
}

void set_strobe_duration(unsigned long duration) {
  strobe_duration = duration;
}

void set_strobe_duty_cycle(float dutypercent) {
	if (dutypercent < 0) dutypercent = 0;
	else if (dutypercent > 100) dutypercent = 100;

	strobe_duration = strobe_delay * (dutypercent / 100);
}

void set_strobe_fpm(unsigned int fpm) {
  set_strobe_delay(60000000/fpm);
}

unsigned int get_strobe_fpm() {
  return 60000000 / (strobe_delay/8*8);
}

unsigned int get_strobe_error() {
  // 90000000 because we have an error of 3*8 microseconds; 1.5 above, 1.5 below
  return 90000000 / ((strobe_delay/8)*8) - 90000000 / ((strobe_delay/8+1)*8);
}


