
#include <iostream>
#include <string>
#include "FastLED/pixeltypes.h"

class LED {
  public:
    void setup(CRGB l[], int size);
    void clear();
    void setBrightness(int brightness);
    void show();
    int size();
  private:
    CRGB *leds;
    int led_count;
};

void LED::setup(CRGB l[], int size){
  leds = l;
  led_count = size;
//    if (user_settings.led_type == strip_APA102) {
//      FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(leds, user_settings.led_count);
//    }
//    else {
//      FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, user_settings.led_count);
//    }
//
//    FastLED.setBrightness(user_settings.led_brightness);
//    FastLED.setDither(1);
}

void LED::clear(){
}

void LED::setBrightness(int brightness){
}

void LED::show(){
  std::string line;
  line.append("|");
  for(int i=0;i < led_count; i++) {
    if(leds[i] > CRGB(0, 0, 0)) {
      line.append("x");
    } else {
      line.append("-");
    }
  }
  line.append("|");
  std::cout << line << '\r' << std::flush;
}

int LED::size(){
}
