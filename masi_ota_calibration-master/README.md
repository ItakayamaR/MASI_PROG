## Instrucciones para escribir y leer valores en la EEPROM:

Dentro del archivo main.cpp:<br/>
### Para escribir:<br/>
1. Cambiar RW_MODE a 1
2. Setear en EEPROM_ADDR_MSB y EEPROM_ADDR_LSB la dirección a partir de la cual se escribirán bytes en la EEPROM
3. Setear la cantidad de bytes en EEPROM_DATA_LENGTH
4. Poner en el archivo platformio.ini la dirección IP  del Masi(La PC debe estar en la misma red WiFi que Masi)
5. Compilar y programarlo por OTA.

### Para leer:
1. Cambiar RW_MODE a 0
2. Setear en EEPROM_ADDR_MSB y EEPROM_ADDR_LSB la dirección a partir de la cual se leerán bytes de la EEPROM
3. Setear la cantidad de bytes en EEPROM_DATA_LENGTH
4. Poner en el archivo platformio.ini la dirección IP  del Masi(La PC debe estar en la misma red WiFi que Masi)
5. Compilar y programarlo por OTA.
6. Abrir Putty y seleccionar la opción Telnet. Tipear la dirección IP del Masi y click en Open. Verá que en el terminal se imprimen los bytes leídos en hexadecimal cada  2 segundos.
