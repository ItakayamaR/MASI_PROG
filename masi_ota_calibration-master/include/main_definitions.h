#ifndef MAIN_DEFINITIONS_H
#define MAIN_DEFINITIONS_H

#include "Arduino.h"
#include "wifi.h"
#include "serial_mgr.h"
#include "RemoteDebug.h"
#include "telnet.h"
#include "config.h"

#define DELAY_TO_WRITE 2000 //Units: ms
#define DELAY_TO_READ  3000 //Units: ms

// Use WiFiClientSecure class to create TLS connection
//WiFiClient client;
extern WiFiClientSecure client;
extern WiFiClient espClient;
extern RemoteDebug Debug;

void OTA_init(void);
void Clean_serial_buffer(void);
void calibration_handler(char *data_buffer, int eeprom_index);

#endif