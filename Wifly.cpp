/*
 * Wifly.cpp: Wifly module library
 * Part of the Illumi project: http://getillumi.org/
 *
 * Copyright 2012 - TBIdeas.com
 * Distributed under the CC-BY-SA license.
 * http://creativecommons.org/licenses/by-sa/3.0/
 *
 */


/*

Manual wifi setup

set uart baud 115200
set w ssid YourSSID
set w p YourPassword
set w a 4
set b p 14484
set b i 2
set o d v00|Illumi$Proto
set c m 13
set i p 3
save
reboot
// reconnect in 115200
*/

#include "Wifly.h"

#define WIFLY_CMD_SIZE 100 // set wlan phrase = 
#define WIFLY_BUF_SIZE 100

Wifly::Wifly(HardwareSerial &s, WiflyDelegate *delegate) : ws(s), delegate(delegate)
{
  ssid = wepPassword = wpaPassphrase = NULL;
}

void Wifly::initializeWep(char *ssid, char *password)
{
  wepPassword = password;
  initialize(ssid);
}

void Wifly::initializeWpa(char *ssid, char *passphrase)
{
  wpaPassphrase = passphrase;
  initialize(ssid);
}

void Wifly::initialize(char *ssid) {
  unsigned int retries = 0;
  
  unsigned int speeds[] = { 57600, 9600 };
  byte speed = 0;

  this->ssid = ssid;
  
  while (1) {
    ws.begin(speeds[speed]);
    
    if (gotoCommandMode()) {
      if (speed != 0) {
        initComm(speeds[0]);
      }
      if (joinNetwork()) {
        return;
      }
    }
    speed = ++speed % 2;
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
  ws.write("\r\nexit\r\n");
  
  for (tries = 0; tries < 3; tries++) {
    ws.write("$$$");
    ws.flush();
    
    if (waitForString("CMD", 500))
      return true;
  }
  return false;
}

boolean Wifly::initComm(unsigned int speed)
{
  // FIXME - Save memory by avoiding one more snprintf here
  ws.write("set uart instant 57600\r\n");
  ws.flush();
  ws.end();
  ws.begin(57600);
}

boolean Wifly::joinNetwork()
{
  char command[WIFLY_CMD_SIZE];

  // Prepare the "set wlan ssid" command
  snprintf(command, WIFLY_CMD_SIZE, "set w s %s", ssid);
  // Send the "set wlan ssid" command and wait for "AOK" reply
  sendWiflyCommand(command, "AOK");

  // Set wlan mode WPA1+WPA2-PSK
  if (wpaPassphrase != NULL) {
    sendWiflyCommand("set w a 4", "AOK");

    // Prepare the "set wlan phrase" command
    snprintf(command, WIFLY_CMD_SIZE, "set w p %s", wpaPassphrase);
    // Send the "set wlan phrase" command and wait for "AOK" reply
    sendWiflyCommand(command, "AOK");
  }
  else if (wepPassword != NULL) {
    sendWiflyCommand("set w a 1", "AOK");
    
    snprintf(command, WIFLY_CMD_SIZE, "set w k %s", wepPassword);
    sendWiflyCommand(command, "AOK");
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
  
  // Send an IP packet each time a \n is given
  sendWiflyCommand("set c m 13", "AOK");
  
  // Enable TCP & UDP - set ip proto 3
  sendWiflyCommand("set i p 3", "AOK");
  
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
  ws.flush();
  ws.write(command);
  ws.write("\r\n");

  return waitForString(reply, timeout); 
}

boolean Wifly::waitForString(char *string,  unsigned long timeout) 
{
  int start = millis();
  char buffer[WIFLY_BUF_SIZE];
  
  while (millis() < start + timeout) {
    if (ws.available()) {
      int len = ws.readBytes(buffer, WIFLY_BUF_SIZE - 1);
      buffer[len] = 0;
      
      if (strstr(buffer, string))
        return true;
    }
  }
  return false;
}

int Wifly::exitWiflyCommand() {
  ws.write("exit\r\n");
  ws.flush();
}

