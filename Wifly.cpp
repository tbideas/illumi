/*
 * Wifly.cpp: Wifly module library
 * Part of the Illumi project: http://getillumi.org/
 *
 * Copyright 2012 - TBIdeas.com
 * Distributed under the CC-BY-SA license.
 * http://creativecommons.org/licenses/by-sa/3.0/
 *
 */

#include "Wifly.h"

#define WIFLY_CMD_SIZE 100 // set wlan phrase = 
#define WIFLY_BUF_SIZE 100

Wifly::Wifly(HardwareSerial &s, WiflyDelegate *delegate) : serial(s), delegate(delegate)
{
}

void Wifly::initialize(char *ssid, char *password) {
  unsigned int retries = 0;
  
  while (1) {
    if (gotoCommandMode()) {
      if (joinNetwork(ssid, password)) {
        return;
      }
    }
    retries++;
  }
}

/* Try three times to go into command mode. Give up after x tries. */
boolean Wifly::gotoCommandMode()
{
  byte tries;
  
  if (delegate)
    delegate->initializationProgress(WIFLY_RESETTING);

  // Make sure we are not in command mode
  Serial.write("\r\nexit\r\n");
  
  for (tries = 0; tries < 3; tries++) {
    Serial.write("$$$");
    Serial.flush();
    
    if (waitForString("CMD", 500))
      return true;
  }
  return false;
}

boolean Wifly::joinNetwork(char *ssid, char *password)
{
  char command[WIFLY_CMD_SIZE];

  // Prepare the "set wlan ssid" command
  snprintf(command, WIFLY_CMD_SIZE, "set w s %s", ssid);
  // Send the "set wlan ssid" command and wait for "AOK" reply
  sendWiflyCommand(command, "AOK");

  // Set wlan mode WPA1+WPA2-PSK
  if (password != NULL) {
    sendWiflyCommand("set w a 4", "AOK");

    // Prepare the "set wlan phrase" command
    snprintf(command, WIFLY_CMD_SIZE, "set w p %s", password);
    // Send the "set wlan phrase" command and wait for "AOK" reply
    sendWiflyCommand(command, "AOK");

    // Note: WEP not supported but you should not be using WEP anyway.
    // If you really have to:
    // sendWiflyCommand("set wlan auth 1", "AOK");
    // sendWiflyCommand("set wlan key 112233445566778899AABBCCDD", "AOK");
  }
  // Or open network.
  else {
    sendWiflyCommand("set w a 0", "AOK");
  }
  
  // Do not autojoin (we will send the join command)
  sendWiflyCommand("set w j 0", "AOK");

  // Broadcast on port 14484
  sendWiflyCommand("set b p 14484", "AOK");
  sendWiflyCommand("set b i 2", "AOK");
  sendWiflyCommand("set o d v00|Illumi$Proto", "AOK");
  
  if (delegate)
    delegate->initializationProgress(WIFLY_JOINING);
  
  // Join now the network we just configured and wait for an IP address from DHCP
  if (sendWiflyCommand("join", "IP=", 10000)) {
    exitWiflyCommand();        
    if (delegate)
      delegate->initializationProgress(WIFLY_JOINED);
    return true;
  }
  
  return false;
}

/* 
 * Wifly must be in command mode.
 * Command does not need to be \n terminated.
 */
boolean Wifly::sendWiflyCommand(char *command, char *reply, unsigned long timeout) {
  Serial.flush();
  Serial.write(command);
  Serial.write("\r\n");

  return waitForString(reply, timeout); 
}

boolean Wifly::waitForString(char *string,  unsigned long timeout) 
{
  int start = millis();
  char buffer[WIFLY_BUF_SIZE];
  
  while (millis() < start + timeout) {
    if (Serial.available()) {
      int len = Serial.readBytes(buffer, WIFLY_BUF_SIZE - 1);
      buffer[len] = 0;
      
      if (strstr(buffer, string))
        return true;
    }
  }
  return false;
}

int Wifly::exitWiflyCommand() {
  Serial.write("exit\r\n");
  Serial.flush();
}

