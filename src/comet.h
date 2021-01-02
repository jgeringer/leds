#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>

extern CRGB g_LEDs[];

void DrawComet()
{
    const byte fadeAmt = 128;       // Fraction of 256 to fade pixel by if its chosen to be faded this pass
    const int cometSize = 5;        // Size of the comet in pixels
    const int deltaHue = 4;         // How far to step the cycling hue each draw call

    static byte hue = HUE_RED;      // Current color
    static int iDirection = 1;      // Current direction (-1 or +1)
    static int iPos = 0;            // Current comet position on strip

    hue += deltaHue;                // Update the comet color
    iPos += iDirection;             // Update the comet position

    // Flip the comet direction when it hits either end
    if (iPos == (NUM_LEDS - cometSize) || iPos == 0)
        iDirection *= -1;

    for (int i = 0; i < cometSize; i++)
        g_LEDs[iPos + 1].setHue(hue);
}