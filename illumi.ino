/*
 * Illumi Sketch (Arduino)
 *
 * Copyright 2012 - TBIdeas.com
 * Distributed under the CC-BY-SA license.
 * http://creativecommons.org/licenses/by-sa/3.0/
 *
 */

#include <EEPROM.h>
// Edit your wifi settings in settings.h so that it is easy to pull/push
// on github without pushing those local settings.
#include "settings.h"
#include "Color.h"
#include "ColorLED.h"

// Define buffer sizes
#define BUFFER_SIZE 255

// If we have not received a command in the last 5 seconds, save the last command in the EEPROM
#define LASTCMD_WAIT_TIMER 5000

#define STARTUP_TIMER 2000

// The last command will take up at most 254 bytes. 
// The first byte is used to tell the length so the next available byte is @ 0x100
#define EEPROM_LAST_CMD 0x0000 

unsigned long startTime;
unsigned long targetTime;
Color startColor;
Color currentColor;
Color targetColor;

#ifndef LED_SHUTDOWN
ColorLED led(RED, GREEN, BLUE, COMMON_ANODE);
#else
ColorLED led(RED, GREEN, BLUE, COMMON_ANODE, LED_SHUTDOWN);
#endif

void setup() {
  int i;

  led.setColor(currentColor);
  
  Serial.setTimeout(1);
  Serial.begin(115200);
  
  Serial1.setTimeout(10);
  Serial1.begin(115200);
  
  char *lastCommand = readLastCommand();
  if (lastCommand != NULL) {
    Serial.write("Reloading last command:");
    Serial.write(lastCommand);
    
    processCommand(lastCommand);
    targetTime = millis() + STARTUP_TIMER;
  }
  
  // Use D2 as an instruction counter to measure the speed of transmission
  pinMode(2, OUTPUT);
}

byte counter = 0;

char buffer[BUFFER_SIZE];
unsigned long lastCommandTime = 0;
char *lastCommandPtr;

void loop() {
  updateLed();
  processWifiInterface();
  processDebugInterface();
}

void updateLed() {
  if (targetTime != 0 && millis() < targetTime) {
    currentColor = Color::interpolateColor(millis(), startTime, targetTime, startColor, targetColor);
  }
  else {
    memcpy(&currentColor, &targetColor, sizeof(Color));
  }
  led.setColor(currentColor.red, currentColor.green, currentColor.blue);
}

void processWifiInterface()
{
  if (Serial1.available()) {
    int len = Serial1.readBytes(buffer, BUFFER_SIZE);
    
    // Remove potential carriage return at the end of command.
    if (buffer[len - 1] == '\n') {
      len--;
    }
    // Apparently, those still exists too ...
    if (buffer[len - 1] == '\r') {
      len--;
    }
    buffer[len] = 0;

    char *rgbCommand = strstr(buffer, "RGB");
    if (rgbCommand != NULL) {
      Serial.write("CMD ");
      Serial.write(rgbCommand);
      Serial.write('\n');
      
      processCommand(rgbCommand);
      
      digitalWrite(2, counter++ % 2 == 0 ? HIGH : LOW);

      lastCommandTime = millis();
      lastCommandPtr = rgbCommand;
    }
    else {
      Serial.write("KO >");
      Serial.write((const uint8_t*)buffer, len);
      Serial.write('\n');
    }
  }
  if (lastCommandTime != 0 && millis() > lastCommandTime + LASTCMD_WAIT_TIMER) {
    Serial.write("Saving last command: ");
    Serial.write(lastCommandPtr);
    Serial.write('\n');
    saveLastCommand(lastCommandPtr);    
    lastCommandTime = 0;
  }
}

void processDebugInterface()
{
  char *last;
  
  if (Serial.available()) {
    char command = Serial.read();
    switch (command) {
      case '?':
        Serial.print("Up and running!\n");
        break;
      case 'L':
        Serial.print("Last Command (EEPROM):\n");
        last = readLastCommand();
        if (last != NULL)
          Serial.print(last);
        else
          Serial.print("NULL");
        Serial.print('\n');
        break;
      case 'l':
        if (lastCommandTime != 0) {
          Serial.print("Last Command Time = ");
          Serial.print(lastCommandTime);
          Serial.print(" And millis() is: ");
          Serial.print(millis());
          Serial.print("\nLast Command buffer points to:");
          Serial.print(lastCommandPtr);
          Serial.print('\n');
        }
        else {
          Serial.print("No last command.\n");
        }
        break;
      default:
        Serial.print("Unknown command: ");
        Serial.print(command);
        Serial.print("\n");
    }
  }
}

void processCommand(char *command)
{
  // The only command format supported right now is RGBXXXXXX\n
  targetColor.red   = hexPairsToInt(command + 3);
  targetColor.green = hexPairsToInt(command + 5);
  targetColor.blue  = hexPairsToInt(command + 7);
  
  startColor = currentColor;
  startTime = millis();
  // No animation for this command
  targetTime = millis();
}

void saveLastCommand(char *cmd)
{
  int i;
  size_t len = strlen(cmd);
  
  int address = EEPROM_LAST_CMD;
  if (len <= 0xFE) { // max len is 0xFF including 1 byte for length
    EEPROM.write(address++, (byte)len);
    
    for (i = 0; i < len; i++)
    {
      EEPROM.write(address++, *cmd);
      cmd++;
    } 
  }
}

char *readLastCommand()
{
  size_t len = EEPROM.read(EEPROM_LAST_CMD);

  // If len is 0 there is nothing to read, and if len is 0xFF then the EEPROM
  // has just never be written there.
  if (len != 0 || len != 0xFF) {
    int i;

    for (i = 0; i < len; i++) {
      buffer[i] = EEPROM.read(EEPROM_LAST_CMD + i + 1);
    }
    buffer[i] = 0;
    return buffer;
  }
  else {
    return NULL;
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
