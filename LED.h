
class LED {
  public:
    void setup();
    void clear();
    void setBrightness(int brightness);
    void show();
    int size();
};

void LED::setup(){
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
}

int LED::size(){
}
