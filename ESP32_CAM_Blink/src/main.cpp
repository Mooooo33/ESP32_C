#include <Wire.h>
#include "SSD1306.h"

SSD1306 display(0x3c, 14, 15);

void setup() {
  display.init();

  display.setFont(ArialMT_Plain_24);
  display.drawString(0, 0, "Hello World");
  display.display();
}

void loop() {
 
}