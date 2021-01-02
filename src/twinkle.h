#include <FastLED.h>

#define NUM_COLORS 5
static const CRGB TwinkleColors [NUM_COLORS] = 
{
    CRGB::Red,
    CRGB::Blue,
    CRGB::Purple,
    CRGB::Green,
    CRGB::Orange,
};

void DrawTwinkle()
{
    FastLED.clear(false);                   // Clear the strip, but don't push out the bits quite yet.

    for (int i = 0; i < NUM_LEDS / 4; i++)
    {
        g_LEDs[random(NUM_LEDS)] = TwinkleColors[random(NUM_COLORS)];
        FastLED.show(g_Brightness);
        delay(200);
    }
}