#include <print_power.h>

#define DEBUG DEBUG_LOOP

// These next two lines must come after all other library #includes
#define BUILD_HACK
#include <hexbright.h>

hexbright hb;

#define HOLD_TIME 150 // milliseconds before going to STROBE or MOMENTARY ON
#define OFF_TIME 1000 // milliseconds before going off on the next normal button press

#define BRIGHTNESS_COUNT 5
#define OFF (BRIGHTNESS_COUNT-1) // define const for off mode corresponding to OFF_LEVEL
int brightness[BRIGHTNESS_COUNT] = {100, 300, 600, MAX_LEVEL, OFF_LEVEL};
//int num_brightness = sizeof(one)/sizeof(int);
int current_brightness = OFF; // start on the last mode (off)

void setup() {
  hb.init_hardware(); 
}

void loop() {
  hb.update();
  
  //MOMENTARY MODE (FULL BRIGHTNESS UNTIL BUTTON RELEASED)
  if(hb.button_pressed_time()>HOLD_TIME && current_brightness==4) { //Button is held from OFF state
         if(hb.button_pressed()) {
          //held for over HOLD_TIME ms, go to momentary
          hb.set_light(CURRENT_LEVEL, MAX_LEVEL, NOW);
          
          while (hb.button_pressed()) { //keep momentary in control until button is released then return to power OFF state
            hb.update();
          }
          //Button is released and light should turn off
          hb.set_light(MAX_LEVEL, OFF_LEVEL, 0);
        }
        
  //PERSISTENT ON MODE (VARIOUS SETTINGS - LIGHT STAYS ON)
  } else {
      //SOLID ON MODE (MULTIPLE BRIGHTNESS SETTINGS, TURN OFF IF OFF_TIME PASSES BETWEEN BUTTON PRESSES)
      if(hb.button_just_released() && hb.button_pressed_time()<HOLD_TIME) { 
          // if held for less than HOLD_TIME before release, change regular modes
          static unsigned long on_time = millis();          
          if(current_brightness!=OFF && on_time+OFF_TIME<millis()) { //if it has been OFF_TIME seconds since last mode change, turn off
            current_brightness = OFF; //Set flashlight mode to off
            hb.set_light(CURRENT_LEVEL, brightness[current_brightness], 50);
            Serial.print("current_brightness: "); Serial.print(current_brightness); Serial.print("\n");
          } else { //Change light mode by one
            current_brightness = (current_brightness+1)%BRIGHTNESS_COUNT;
            hb.set_light(CURRENT_LEVEL, brightness[current_brightness], 50);
            on_time = millis();
            Serial.print("on_time value is: "); Serial.print(on_time); Serial.print("\n");
            Serial.print("current_brightness: "); Serial.print(current_brightness); Serial.print("\n");
          }  
          
      //TACTICAL STROBE MODE (FLASH AT MAX BRIGHTNESS IF BUTTON IS HELD)
      } else if (hb.button_pressed_time()>HOLD_TIME) {
          if(hb.button_pressed()) {
            // held for over HOLD_TIME ms, go to strobe
            static unsigned long flash_time = millis();
            if(flash_time+70<millis()) { // flash every 70 milliseconds
              flash_time = millis(); // reset flash_time
              hb.set_light(MAX_LEVEL, 0, 20); // and pulse (going from max to min over 20 milliseconds)
              // actually, because of the refresh rate, it's more like 'go from max brightness on high
              //  to max brightness on low to off.
            }
          } else { // we have been doing strobe, but the light is now released
              // after strobe, go to previous light level:
              hb.set_light(CURRENT_LEVEL, brightness[current_brightness], 50);
          }
      }
  }
  print_power();
}

