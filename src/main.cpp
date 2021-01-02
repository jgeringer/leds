#include <Arduino.h>
#include <U8g2lib.h>
#define FASTLED_INTERNAL
#include <FastLED.h>

#define OLED_CLOCK  15        // Pins for OLED display
#define OLED_DATA   4
#define OLED_RESET  16

#define NUM_LEDS    144        // FastLED definitions
#define LED_PIN     5

CRGB g_LEDs[NUM_LEDS] = {0};  // Frame buffer for FastLED

U8G2_SSD1306_128X64_NONAME_F_HW_I2C g_OLED(U8G2_R2, OLED_RESET, OLED_CLOCK, OLED_DATA);
// U8G2_SSD1306_128X64_NONAME_F_SW_I2C g_OLED(U8G2_R2, OLED_CLOCK, OLED_DATA, OLED_RESET);
int g_lineHeight = 0;
int g_Brightness = 2; // 0-255 brightness scale. do not go to big here, stay at 64 max;

// #include "marquee.h"
#include "twinkle.h"

// FramesPerSecond
//
// Tracks a weighted average to smooth out the values that it calcs as the simple reciprocal
// of the amount of time taken specified by the caller.  So 1/3 of a second is 3 fps, and it
// will take up to 10 frames or so to stabilize on that value.
 
double FramesPerSecond(double seconds)
{
  static double framesPerSecond; 
  framesPerSecond = (framesPerSecond * .9) + (1.0 / seconds * .1);
  return framesPerSecond;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(115200);
  while (!Serial) { }
  
  Serial.println("ESP32 Startup");
 
  g_OLED.begin();
  g_OLED.clear();
  g_OLED.setFont(u8g2_font_profont15_tf);
 
  g_lineHeight = g_OLED.getFontAscent() - g_OLED.getFontDescent();        // Descent is a negative number so we add it to the total

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(g_LEDs, NUM_LEDS);               // Add our LED strip to the FastLED library
  FastLED.setBrightness(64);
}

void loop() {
  // put your main code here, to run repeatedly:
  bool bLED = 0;
  double fps = 0;

  uint8_t initialHue = 0;
  const uint8_t deltaHue = 16;      // how fast the rainbow is going to scroll by
  const uint8_t hueDensity = 4;     // 255 colors in the color wheel, so go 4 at a time.

 
  for (;;)
  {
    bLED = !bLED;                                                         // Blink the LED off and on  
    digitalWrite(LED_BUILTIN, bLED);
 
    double dStart = millis() / 1000.0;                                    // Display a frame and calc how long it takes

    // Handle OLED drawing

    g_OLED.clearBuffer();
    g_OLED.setCursor(0, g_lineHeight);
    g_OLED.printf("FPS: %.1lf", fps);
    g_OLED.sendBuffer();

    // Handle LEDs
    // for (int i = 0; i < NUM_LEDS; i++) 
    //   g_LEDs[i] = CRGB::Red;
    // fill_solid(g_LEDs, NUM_LEDS, CRGB::Green);
    // fill_rainbow(g_LEDs, NUM_LEDS, initialHue += hueDensity, deltaHue);
    // DrawMarquee();
    DrawTwinkle();

    FastLED.setBrightness(g_Brightness);
    FastLED.show();

    double dEnd = millis() / 1000.0;
    fps = FramesPerSecond(dEnd - dStart);
  }
}