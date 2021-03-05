#include <Arduino.h>
#include "main_definitions.h"
#include <ArduinoOTA.h>

#define RW_MODE 1           //0: lectura, 1: escritura
#define EEPROM_ADDR_MSB     0x14
#define EEPROM_ADDR_LSB     0x00
#define EEPROM_DATA_LENGTH  160 //Entre 1 y 100

//Escriba aquí los bytes a grabar en EEPROM
uint8_t write_data_frame[] = {
0x09,0x4E,0x09,0x00,0x90,0x6D,0xFE,0xFF,0xFF,0x04,0x1F,0x00,0x0A,0x4C,0x9C,0x00 
0x5A,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x17,0x27,0xFB,0xFA,0x00,0x2D,0x2D
0x01,0x37,0x45,0x48,0xB4,0x00,0xCA,0xF8,0x0B,0x00,0xFE,0x97,0x96,0x00,0x00,0x00
0x14,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x40,0x00,0x00,0x00,0x00,0x00,0x40
0x00,0x17,0x27,0xFB,0xFA,0x00,0x2D,0x2D,0x01,0x37,0x45,0x48,0xB4,0x00,0xCA,0xF8
0x0B,0x00,0xFE,0x97,0x96,0x00,0x00,0x00,0x14,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
0xFF,0x00,0xFE,0x00,0x96,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x19,0x46,0x26,0x96,0xDC,0x05
0x01,0x05,0x26,0x0A,0x64,0x02,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};

uint8_t read_data_frame[] = {EEPROM_ADDR_MSB, EEPROM_ADDR_LSB, EEPROM_DATA_LENGTH};

WiFiClientSecure client;
WiFiClient espclient;

uint8_t serial_rx_buffer[RX_BUFFER_SIZE];
uint16_t serial_rx_buffer_idx;
RemoteDebug Debug;
uint32_t last_time = millis();
uint32_t read_start_time = millis();
uint8_t cmd = 0;
uint8_t read_bytes_from_eeprom[256] = {0};
uint8_t frame_to_send[100] = {0};
char telnet_message[600] = {'\0'};
uint8_t read_flag = 0;
int idx_read = 0;
int block_length = 0;
int block_number = 0;
uint32_t delay_to_send_read_cmd = 0;

void setup() {
  delay(3000);
  serial_mgr_init();
  wifi_init();
  Tel_debug_init();
  OTA_init(); 
  
  if(RW_MODE == 1){
    cmd = 0xBA;
    
    int num_blocks = EEPROM_DATA_LENGTH / 32;
    int remainder = EEPROM_DATA_LENGTH % 32;
    int eeprom_base_addr = (EEPROM_ADDR_MSB << 8) + EEPROM_ADDR_LSB;
    int addr_to_send = 0;

    //Enviamos bloques de 32
    for(int idx = 0; idx < num_blocks; idx++){
        memset(frame_to_send, 0x00, sizeof(frame_to_send));
        addr_to_send = eeprom_base_addr + idx*32;
        frame_to_send[0] = (addr_to_send >> 8) & 0xFF;
        frame_to_send[1] = addr_to_send & 0xFF;
        frame_to_send[2] = 32;
        for(int j=0; j < 32; j++)
          frame_to_send[3 + j] = write_data_frame[idx*32 + j];

        serial_mgr_pack(cmd, frame_to_send, 35);
        delay(DELAY_TO_WRITE);
    }

    //Enviamos residuo
    if(remainder > 0){
      memset(frame_to_send, 0x00, sizeof(frame_to_send));
      addr_to_send = eeprom_base_addr + num_blocks*32;
      frame_to_send[0] = (addr_to_send >> 8) & 0xFF;
      frame_to_send[1] = addr_to_send & 0xFF;
      frame_to_send[2] = remainder;
      for(int k=0; k < remainder; k++)
        frame_to_send[3 + k] = write_data_frame[num_blocks*32 + k];

      serial_mgr_pack(cmd, frame_to_send, 3 + remainder);
    }
    
  }
  else{
    read_flag = 1;
    read_start_time = millis();
    delay_to_send_read_cmd = 10000;
  }
}

void loop() {
  ArduinoOTA.handle();

  if((read_flag) && ((millis() - read_start_time) >= delay_to_send_read_cmd)){
    cmd = 0xBB;
    delay_to_send_read_cmd = DELAY_TO_READ;
    int num_blocks = EEPROM_DATA_LENGTH / 32;
    int remainder = EEPROM_DATA_LENGTH % 32;
    int eeprom_base_addr = (EEPROM_ADDR_MSB << 8) + EEPROM_ADDR_LSB;
    int addr_to_send = 0;

    if(idx_read < num_blocks){
      //Leemos bloques de 32
      addr_to_send = eeprom_base_addr + idx_read*32;
 ]
 ]
 
    frame_to_send[0] = (addr_to_send >> 8) & 0xFF;
      frame_to_send[1] = addr_to_send & 0xFF;
      frame_to_send[2] = 32;
      idx_read++;
      block_number++;
    }
    else if(remainder > 0){
      //Leemos el residuo
      addr_to_send = eeprom_base_addr + num_blocks*32;
      frame_to_send[0] = (addr_to_send >> 8) & 0xFF;
      frame_to_send[1] = addr_to_send & 0xFF;
      frame_to_send[2] = remainder;
      block_number++;
      read_flag = 0;
    }
    else if(remainder == 0)
      read_flag = 0;

    block_length = frame_to_send[2];
    serial_mgr_pack(cmd, frame_to_send, 3);
    read_start_time = millis();
  }

  if(serial_mgr_get_frame(serial_rx_buffer, &serial_rx_buffer_idx)){
        uint8_t cmd = serial_rx_buffer[3];
        
        switch (cmd)
        {
            case 0x01:
            case 0x02:
              Clean_serial_buffer();
              break;
            case 0xA8:
              calibration_handler((char *)serial_rx_buffer, idx_read);
              break;    
            default:
                break;
        }
    Clean_serial_buffer();      
  }

  if((millis() -last_time) >= 2000){
    last_time = millis();
    
    Debug.println(telnet_message);
    Debug.handle();
  }

  Debug.handle();
}

void OTA_init(void){
    ArduinoOTA.onStart([]() {
        Serial.println("Start.");
    });
    ArduinoOTA.onEnd([]() {
        Serial.println("\nEnd.");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) {
        Serial.println("Auth Failed");
        } else if (error == OTA_BEGIN_ERROR) {
        Serial.println("Begin Failed");
        } else if (error == OTA_CONNECT_ERROR) {
        Serial.println("Connect Failed");
        } else if (error == OTA_RECEIVE_ERROR) {
        Serial.println("Receive Failed");
        } else if (error == OTA_END_ERROR) {
        Serial.println("End Failed");
        }
    });
    ArduinoOTA.begin();
}

void calibration_handler(char *data_buffer, int eeprom_index){
   int cursor_str = 0;

  for(int idx = 0; idx < block_length; idx++)
    read_bytes_from_eeprom[idx] = data_buffer[4 + idx];
  
  cursor_str = sprintf(telnet_message, "Bytes leidos. Bloque:%02d. Longitud:%02d: ", block_number, 
                      block_length);

  for(int idx = 0; idx < block_length; idx++){
    cursor_str += sprintf(telnet_message + cursor_str, "%02X ", read_bytes_from_eeprom[idx]);
  }

}

void Clean_serial_buffer(void){
     memset((void *)serial_rx_buffer, 0x00, sizeof(serial_rx_buffer));
     memset((void *)read_bytes_from_eeprom, 0x00, sizeof(read_bytes_from_eeprom));
     serial_rx_buffer_idx = 0;
}

