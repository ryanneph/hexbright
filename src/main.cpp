#include <print_power.h>
#include "tactical.h"

// These next two lines must come after all other library #includes
#define BUILD_HACK
#include <hexbright.h>

hexbright hb;

void setup() {
    hb.init_hardware();
}

void loop() {
    loop_tactical();
}
