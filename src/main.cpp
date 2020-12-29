#include <Arduino.h>
#include <U8g2lib.h>

#define OLED_CLOCK  15    // Pins for OLED display
#define OLED_DATA   4
#define OLED_RESET  16

U8G2_SSD1306_128X64_NONAME_F_SW_I2C g_OLED(U8G2_R2, OLED_CLOCK, OLED_DATA, OLED_RESET);

int g_lineHeight = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);

  g_OLED.begin();
  g_OLED.clear();
  g_OLED.setFont(u8g2_font_profont15_tf);

  g_lineHeight = g_OLED.getFontAscent() - g_OLED.getFontDescent();  // Descent is a negative number so we add it to the total

  g_OLED.setCursor(0, g_lineHeight);                                // Plae cursor at bottom of first line of text
  g_OLED.print("Hello Hailey!");

  g_OLED.setCursor(0, g_lineHeight * 2);
  g_OLED.printf("Line Height: %d", g_lineHeight);
  g_OLED.setCursor(0, g_lineHeight * 3);
  g_OLED.print("Third line");

  g_OLED.sendBuffer();
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, 0);
  delay(500);
  digitalWrite(LED_BUILTIN, 1);
  delay(500);
}