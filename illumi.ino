/*
 * Illumi Sketch (Arduino)
 *
 * Copyright 2012 - TBIdeas.com
 * Distributed under the CC-BY-SA license.
 * http://creativecommons.org/licenses/by-sa/3.0/
 *
 */

// Edit your wifi settings in settings.h so that it is easy to pull/push
// on github without pushing those local settings.
#include "settings.h"
#include "Wifly.h"
#include "IllumiWiflyDelegate.h"
#include "ColorLED.h"

// Define buffer sizes
#define BUFFER_SIZE 100
#define OUTPUT_SIZE 300

int red, green, blue;

ColorLED led(RED, GREEN, BLUE, COMMON_ANODE);
IllumiWiflyDelegate wDelegate(led);

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(100); // timeout for reading on serial port

  int i;

  led.setColor(0, 0, 0);
  
  Wifly wifly(Serial, &wDelegate);
  wifly.initialize(SSID, WPA_PASSWORD);

  delay(1000);
  led.setColor(0,0,0);

  Serial.write("Illumi, Ready to Roll!\n");
}

void loop() {
  char buffer[BUFFER_SIZE];
  char output[OUTPUT_SIZE];
  
  if (Serial.available()) {
    int len = Serial.readBytes(buffer, BUFFER_SIZE);
    
    // Remove potential carriage return at the end of command.
    if (buffer[len - 1] == '\n') {
      len--;
    }
    // Apparently, those still exists too ...
    if (buffer[len - 1] == '\r') {
      len--;
    }
    buffer[len] = 0;

    bool gotCommand = false;

    if (strstr(buffer, "*OPEN*")) {
      Serial.write("\nC-Light! Ready to roll.\n");
      snprintf(output, OUTPUT_SIZE, "RGB%x%x%x\n", red, green, blue);
      Serial.write(output);
      gotCommand = true;
    }
    
    char *rgbCommand = strstr(buffer, "RGB");
    if (rgbCommand != NULL) {
      red = hexPairsToInt(rgbCommand + 3);
      green = hexPairsToInt(rgbCommand + 5);
      blue = hexPairsToInt(rgbCommand + 7);

      led.setColor(red, green, blue);
      
      snprintf(output, OUTPUT_SIZE, "RGB%02x%02x%02x\n", red, green, blue);   
      Serial.write(output);
      gotCommand = true;
    }
    
    if (!gotCommand)
    {
      snprintf(output, OUTPUT_SIZE, "Command not understood: >%s< (len=%i)\n", buffer, len);
      Serial.write(output);
    }
  } 
}

int hexPairsToInt(char *pair)
{
  return (hexCharToInt(pair[0]) << 4) + hexCharToInt(pair[1]);
}

int hexCharToInt(char c) {
    if (c >= '0' && c <= '9') 
      return c - '0';
    else if (c >= 'A' && c <= 'F') 
      return c - 'A' + 10;
    return 0;
}
