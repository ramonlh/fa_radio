
#pragma once

// tasks
#define task_ds18b20_size 2048
#define task_ds18b20_priority 1
#define task_ds18b20_delay 10000

#define task_ads1115_size 2048
#define task_ads1115_priority 1
#define task_ads1115_delay 1000

// extensions
#define SERIAL1
#define SERIAL2
//#define WIFIBLE
#define TCPSERVER
#define UDPSERVER
#define NTPCLIENT
#define WEBSOCKETS
#define FTPSERVER
#define NTPCLIENT
#define DS18B20   // checked
#define ADS1X15   // checked
#define SERVO
#define WEBSERVER
//#define REMOTEDEBUG
#define ESPCLIENT
#define TCPCLIENT
#define PUBSUB
#define TPA2016
#define TFT

// pines
#define W0            4     // sondas DS18B20   // EA4GZI
#define RXD2          39    // UART2            // EA4GZI    39=VN
#define TXD2          22    // UART2            // EA4GZI
#define ENC_A         34    // INPUT
#define ENC_B         35    // INPUT
#define FBUTTON       36    // INPUT     36=VP
#define PTT           15    // INPUT
#define ANALOG_KEYER  33    // INPUT ANALOG
#define TX_RX         23   // Relay
#define CW_TONE       16   // OUTPUT PWM
#define TX_LPF_A      25   // OUTPUT Relay, azul
#define TX_LPF_B      26   // OUTPUT Relay, verde
#define TX_LPF_C      27   // OUTPUT Relay, marrón
#define CW_KEY        2    // OUTPUT
#define SDA           18
#define SCL           19

// ADC ports ADS1115
#define VFORp 0
#define VREFp 1
#define SMETERp 2

// I2C addresses
#define I2CMETER_ADDR     0x58  //S-Meter Address

// servidores/puertos
//****************** PORTS **************************
#define FTPPort    21
#define TCPPort   8084
#define UDPPortS  8085
#define WSPort    8086
#define UDPPortF  8087
#define WEBPort   8088 
#define webportdefault 8088

#define HTTP_CODE_OK 200

// radio
#define MAX_PAGES      13
#define MAX_BANDS      10  
#define maxTemp         8       // número máximo de sondas ds18B20
#define VFO_A          16
#define VFO_B          20
#define maxMem        100
#define LOWEST_FREQ_DIAL  (3000l)
#define HIGHEST_FREQ_DIAL (60000000l)
#define TX_SSB          0
#define TX_CW           1

#define crlf "\n"

const char watermarkref[16]="uBitx1234567890";  // código de verificación uBitx

const float degtoradf=0.017453;        // 3.1416/180;  coef. para convertir a radianes 

const String vacio = "";
const char wifimodetext[4][7]={"OFF","STA","AP","AP+STA"};
const char connmodetext[5][10]={"OFF","IP as STA","IP as AP","Serial-IP","Serial"};
const char serial2modetext[2][8]={"Debug","Manager"};

char timezonetext[39][7]={"-12","-11","-10","-9:30","-9","-8","-7","-6","-5:30","-5",
                          "-4","-3:30","-3","-2","-1","+0","+1","+2","+3",
                          "+3:30","+4","+4:30","+5","+5:30","+5:45","+6",
                          "+6:30","+7","+8","+8:45","+9","+9:30","+10",
                          "+10:30","+11","+12","+12:45","+13","+14"};

long timezoneval[39]={-43200,-39600,-36000,-34200,-32400,-28800,-25200,-21600,-19800,-18000,
                      -14400,-12600,-10800,-7200,-3600,0,3600,7200,10800,12600,14400,
                       16200,18000,19800,20700,21600,23400,25200,28800,31500,32400,34200,
                       36000,37800,39600,43200,45900,46800,50400};                          

//*******************************************************
//  Commands TCP conection    // NO USAR 10 NI 13 
#define tcpACK 0
#define tcpsplitOn 3
#define tcp_is_USB 4
#define tcpcwMode 5
#define tcpritOn 8
#define tcpvfoActive 19
#define tcpisusbA 20
#define tcpisusbB 21
#define tcpcwmodeA 22
#define tcpcwmodeB 23
#define tcpbanddn 40    // band dn
#define tcpbandup 41    // band up
#define tcpfreqdn 42    // freq dn
#define tcpfrequp 43    // freq up
#define tcpCallSign 44
#define tcpfrequency 45
#define tcpfrequencyA 46
#define tcpfrequencyB 47
#define tcptemp1 48     // temp 1
#define tcptemp2 49     // temp 2
#define tcptemp3 50     // temp 3
#define tcptunestep 51    // define step
#define tcpwifi 52        // set WiFi
#define tcpreset 53       // reset remoto
#define tcpframe 54       // framemode
#define tcpminsmeter 55   // valor mínimo smeter
#define tcpmaxsmeter 56   // valor máximo smeter
#define tcpscanst 64
#define tcpkeylock 65
#define tcpattlevel 66
#define tcpifShiftVal 67
#define tcpvtotvalue 68
#define tcpitotvalue 69

#define tcpMEM 126
#define tcpALL 127
