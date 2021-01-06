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
int g_Brightness = 32;  // 0-255 brightness scale. do not go to big here, stay at 64 max;
int g_PowerLimit = 900; // 900mW Power Limit

// #include "marquee.h"
// #include "twinkle.h"
// #include "comet.h"
// #include "bounce.h"

#define ARRAYSIZE(x) (sizeof(x)/sizeof(x[0]))             // Counts number of elements in a statically defined array
#define TIMES_PER_SECOND(x) EVERY_N_MILLISECONDS(1000/x)  // Should not be run more than 1000ms


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

  set_max_power_indicator_LED(LED_BUILTIN);                               // Light the builtin LED if we power throttle
  FastLED.setMaxPowerInMilliWatts(g_PowerLimit);                          // Set the power limit, above which brightness will be throttled
}

void loop() {
  // put your main code here, to run repeatedly: 
  while (true)
  {

    EVERY_N_MILLISECONDS(20)
    {
      fadeToBlackBy(g_LEDs, NUM_LEDS, 64);
      int cometSize = 15;
      int iPos = beatsin16(32, 0, NUM_LEDS-cometSize);
      byte hue = beatsin8(10);  // 60 = once per second it'll cycle through the colors. 10 seems to bee chaning the color at each end of the strip
      // fill_solid(&g_LEDs[iPos], cometSize, CRGB::DarkViolet);
      for (int i = iPos; i < iPos + cometSize; i++)
        g_LEDs[i] = CHSV(hue, 255, 255); // CRGB::DarkViolet;
    }

    // Handle OLED drawing
    EVERY_N_MILLISECONDS(250)
    {
      g_OLED.clearBuffer();
      g_OLED.setCursor(0, g_lineHeight);
      g_OLED.printf("FPS: %u", FastLED.getFPS());
      g_OLED.setCursor(0, g_lineHeight * 2);
      g_OLED.printf("Power: %u mW", calculate_unscaled_power_mW(g_LEDs, NUM_LEDS));
      g_OLED.setCursor(0, g_lineHeight * 3);
      g_OLED.printf("Brite: %d", calculate_max_brightness_for_power_mW(g_Brightness, g_PowerLimit));
      g_OLED.sendBuffer();
    }

    FastLED.setBrightness(g_Brightness);  // Set the brightness scale
    FastLED.delay(10);                    // Show and delay
  }
}