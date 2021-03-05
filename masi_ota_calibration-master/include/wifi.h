#ifndef WIFI_H
#define WIFI_H

#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager

#define NTP_SERVER_NAME   "time.nist.gov"
#define NTP_PACKET_SIZE   48
#define UDP_PORT        2390

extern uint64_t ms_since_epoch;
extern unsigned long offset_millis;
extern byte mac[6];

void wifi_init(void);
uint32_t Wifi_get_timestamp(void);
uint32_t Send_ntp_packet(IPAddress& address);
void wifi_sync_timestamp(void);

#endif