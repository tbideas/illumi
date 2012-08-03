/*
 * Wifly.h: Wifly module library
 * Part of the Illumi project: http://getillumi.org/
 *
 * Copyright 2012 - TBIdeas.com
 * Distributed under the CC-BY-SA license.
 * http://creativecommons.org/licenses/by-sa/3.0/
 *
 */
 
#ifndef __WIFLY_H__
#define __WIFLY_H__

#include "Arduino.h"

// Status sent to the delegate
#define WIFLY_RESETTING 0
#define WIFLY_JOINING 1
#define WIFLY_JOINED 2

// Timeouts
#define CMD_TIMEOUT 1000
#define JOIN_TIMEOUT 10000

class WiflyDelegate
{
  public:
    /*
     * Called when a new step is reached in the initialization process or 
     * when a step is retried. The delegate can return FALSE to quit the 
     * initialization process.
     */
    virtual boolean initializationProgress(int status) = 0;
};

class Wifly
{
  public:
    Wifly(HardwareSerial &s, WiflyDelegate *delegate = 0);
    
    /*
     * Initializes the hardware and wait until the Wifly has connected to the given network.
     * 
     * If you are using WPA/WP2, give the shared-key used.
     * If the network is opened, do not give a password.
     */
    void initialize(char *ssid, char *password = 0);
  
  private:
    HardwareSerial serial;
    WiflyDelegate *delegate;
    
    boolean gotoCommandMode();
    boolean joinNetwork(char *ssid, char *password);
    int exitWiflyCommand();

    boolean waitForString(char *string,  unsigned long timeout);
    boolean sendWiflyCommand(char *command, char *reply, unsigned long timeout = CMD_TIMEOUT);
 };
 
 #endif
