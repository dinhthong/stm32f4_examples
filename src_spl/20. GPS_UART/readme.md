# Brief
+ UART DMA project for reading and parsing GPS data.
+ STM32, UART3, PB10, PB11
# Common GPS module pin colors.
+ Red -> Vcc
+ Black -> Gnd
+ Green -> Tx connects PL2303 Rx.
+ Yellow -> Rx
+ NeoGPS Arduino. 
NMEAsimple
# Arduino Library for GPS.
## NeoGPS
https://github.com/SlashDevin/NeoGPS
Support both NMEA and UBX, but you have to know how to use.
SoftwareSerial .
GPS Green -> pin 8 Arduino for software serial.
## TinyGPS

# Configure the GPS module using u-center.
Version: u-center 8.29
View -> Configuration View.
## PRT
Configure BaudRate.
Send and Receive message format.
## RATE.
change u blox data rate. -> change `Measurement Period` 

-> Click Send
## NAV 5.

# NMEA important data sentences.
+ GPGGA - System fix data
+ GPGSA - DOP and active satellites
GPGLL - Geographic Latitude and Longitude

GPGST - Pseudo Range Error Statistics
GPGSV - Satellites in View
GPRMC - Recommended Minimum specific GPS/Transit data
GPVTG - Course over ground and Ground speed
GPZDA - UTC Time and Datw

$GPGSV, $GPGLL, $GPVTG, and $GPGSA

$GNRMC
$GPGGA NMEA Sentence
# GNSS
GNSS is broader term for GNSS system.
Most commonly referred GNSS system is GPS. Other than GPS, there are also Galileo, BeiDou, QZSS, SBAS,...
# Disable specific GPS data.
