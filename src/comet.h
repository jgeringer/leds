#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>

extern CRGB g_LEDs[];

void DrawComet()
{
    // FastLED.clear(false);

    const byte fadeAmt = 64;        // Fraction of 256 to fade pixel by if its chosen to be faded this pass
    const int cometSize = 5;        // Size of the comet in pixels
    const int deltaHue = 0;         // How far to step the cycling hue each draw call (0 = keep at red, 4 = cycle through rainbow every edge)
    static double cometSpeed = 1;   // How far to advance the coment every frame
    static byte hue = HUE_RED;      // Current color
    static int iDirection = 1;      // Current direction (-1 or +1)
    static double iPos = 0.0;       // Current comet position on strip

    hue += deltaHue;                // Update the comet color
    iPos += iDirection * cometSpeed;// Update the comet position

    // Flip the comet direction when it hits either end
    if (iPos == (NUM_LEDS - cometSize) || iPos == 0)
        iDirection *= -1;

    // Draw the comet at its current position
    for (int i = 0; i < cometSize; i++)
        g_LEDs[(int)iPos + 1].setHue(hue);

    // Fade the LEDs one step
    for (int j = 0; j < NUM_LEDS; j++)
        if (random(2) == 1)
            g_LEDs[j] = g_LEDs[j].fadeToBlackBy(fadeAmt);

    delay(20);
}