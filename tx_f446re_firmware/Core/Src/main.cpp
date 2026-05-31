#include "rcc_config.h"
#include "delay.h"

int main(void) {

    SystemClock_Config();
    Delay::init();

    while(1) {

        Delay::ms(1000);
    }
}
