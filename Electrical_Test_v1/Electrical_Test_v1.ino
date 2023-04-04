#define DEBUG
#ifdef DEBUG
#define Dprint(...)   Serial.print(__VA_ARGS__)
#define Dprintln(...) Serial.println(__VA_ARGS__)
#else
#define Dprint(...)
#define Dprintln(...)
#endif

//Bibliotecas
#include <WiFi.h>
#include <NTPClient.h>
#include <SPIFFS.h>
#include <FS.h>
#include <ArduinoOTA.h>
#include <WebServer.h>
#include <SD.h>
#include <SPI.h>
#include <max6675.h>
#include <DHT.h>
#include <Wire.h>
#include <ADS1115_WE.h>
#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>
#include <Preferences.h>
//#include <sqlard.h>

//Inicializacao Preferences (armazenamento configuracao na flash)
Preferences preferences;

//Estrutura InfluxDB - banco de dados cloud com Dashboard
// InfluxDB v2 server url, e.g. https://eu-central-1-1.aws.cloud2.influxdata.com (Use: InfluxDB UI -> Load Data -> Client Libraries)
#define INFLUXDB_URL "https://us-central1-1.gcp.cloud2.influxdata.com"
// InfluxDB v2 server or cloud API token (Use: InfluxDB UI -> Data -> API Tokens -> Generate API Token)
#define INFLUXDB_TOKEN "ajZA94dKkYG3vwdtfAtdcKPwbo1YUgD7eweMSpwRxidxui3zmFL_Cnj4sJOmVuEKO4_pIP4ejVxw1e5CHNRhYA=="
// InfluxDB v2 organization id (Use: InfluxDB UI -> User -> About -> Common Ids )
#define INFLUXDB_ORG "bruno@dangelomachado.com"
// InfluxDB v2 bucket name (Use: InfluxDB UI ->  Data -> Buckets)
#define INFLUXDB_BUCKET "Test Electrical"
// Set timezone string according to https://www.gnu.org/software/libc/manual/html_node/TZ-Variable.html
#define TZ_INFO "WART4WARST,J1/0,J365/25"
// InfluxDB client instance with preconfigured InfluxCloud certificate
InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);
// Data point
Point sensor("wifi_status");

//Páginas HTML Web
#include "index.h"
#include "configuracao.h"
#include "verificaOTA.h"
#include "serverIndex.h"
#include "Resultado_Ok.h"
#include "Resultado_Falha.h"
#include "calibracao.h"

//Controle do Temporizador do WTD - watchdog (nao deixa sistema travar)
hw_timer_t *timer = NULL;   //faz o controle do temporizador (interrupção por tempo)

//Estrutura NTP - sincronizacao relogio
WiFiUDP udp;
NTPClient timeClient(udp, "a.st1.ntp.br", -3 * 3600, 60000);/* Cria um objeto "NTP" com as configurações.utilizada no Brasil */
String formattedDate;
String dayStamp;
String timeStamp;

//Definições das Saídas
#define LED_Self_Test   32
#define LED_Running     33
#define Rele_Off        19

//Definiçoes das Entradas
#define WIFI_Bt         35
#define Self_Test_Bt    36
#define Start_Log_Bt    39
#define Stop_Log_Bt     34

//Definições HSPI -> SD
#define HSPI_CLK        14
#define HSPI_MISO       12
#define HSPI_MOSI       13

//Definições VSPI -> Termopares
#define VSPI_CLK        18
#define VSPI_MISO       23
//#define VSPI_MOSI       19  //nao sera utilizado

//Definições I2C
#define I2C_SCL         22
#define I2C_SDA         21

//Definições DHT
#define Data_DHT1       25
#define Data_DHT2       26
#define Data_DHT3       27
#define Data_DHT4       00

//Definições ChipSelects
#define CS_Termo1       05
#define CS_Termo2       17
#define CS_Termo3       16
#define CS_Termo4       04
#define CS_Termo5       02
#define CS_SD           15

//Modos SPI
SPIClass SPISD(HSPI);
SPIClass SPITermo(VSPI);

//Inicialização Termopares
MAX6675 Termo1(HSPI_CLK, CS_Termo1, HSPI_MOSI);
MAX6675 Termo2(HSPI_CLK, CS_Termo2, HSPI_MOSI);
MAX6675 Termo3(HSPI_CLK, CS_Termo3, HSPI_MOSI);
MAX6675 Termo4(HSPI_CLK, CS_Termo4, HSPI_MOSI);
MAX6675 Termo5(HSPI_CLK, CS_Termo5, HSPI_MOSI);

//Inicializacao DHT
#define DHT_TYPE DHT22
DHT DHT1(Data_DHT1, DHT_TYPE);
DHT DHT2(Data_DHT1, DHT_TYPE);
DHT DHT3(Data_DHT1, DHT_TYPE);
DHT DHT4(Data_DHT1, DHT_TYPE);

//Inicializacao dos ADC
ADS1115_WE ADC_Baixa = ADS1115_WE(0x48);
ADS1115_WE ADC_Alta = ADS1115_WE(0x49);

//Flag para indicar se foi salva uma nova configuração de rede
bool shouldSaveConfig = false;

//Porta do Servidor WEB
const uint32_t PORTA = 80; //A porta que será utilizada (padrão 80)

//Algumas informações do chip
uint32_t chipID; //um ID exclusivo do Chip...
String CHIP_ID; // montado para ser usado no HTML
String VERSION; //Exemplo de um controle de versão
String RunningTime;
String INFOS = VERSION + CHIP_ID + RunningTime;

//Sinalizador de autorização do OTA
boolean OTA_AUTORIZADO = false;

//Páginas HTML utilizadas no procedimento OTA
String raiz = "";
String configuracao = "";
String verificaOTA = "";
String serverIndex = "";
String Resultado_Ok = "";
String Resultado_Falha = "";
String calibracao = "";

//Configura o Web Server para disponibilizar as páginas
WebServer server(PORTA);

//Variáveis armazenadas SPIFFS/NVS
String ssid_WIFI = "";
String password_WIFI = "";
int TempTime = 10;
int TempMAX = 130;
bool StopRele = true;
//String CalibSensor = "";
//String ssid_WIFI_Path = "/ssid_WIFI.txt";
//String password_WIFI_Path = "/password_WIFI.txt";
//String TempTime_Path = "/TempTime.txt";
//String TempMAX_Path = "/TempMAX.txt";
//String StopRele_Path = "/StopRele.txt";
//String CalibSensor_Path = "/CalibSensor.txt";
float DHT1T_alfa = 1;
float DHT1U_alfa = 1;
float DHT1T_beta = 0;
float DHT1U_beta = 0;
float DHT2T_alfa = 1;
float DHT2U_alfa = 1;
float DHT2T_beta = 0;
float DHT2U_beta = 0;
float DHT3T_alfa = 1;
float DHT3U_alfa = 1;
float DHT3T_beta = 0;
float DHT3U_beta = 0;
float DHT4T_alfa = 1;
float DHT4U_alfa = 1;
float DHT4T_beta = 0;
float DHT4U_beta = 0;
float Termo1_alfa = 1;
float Termo1_beta = 0;
float Termo2_alfa = 1;
float Termo2_beta = 0;
float Termo3_alfa = 1;
float Termo3_beta = 0;
float Termo4_alfa = 1;
float Termo4_beta = 0;
float Termo5_alfa = 1;
float Termo5_beta = 0;
float TB1_alfa = 0.000185;
float TB1_beta = 0;
float TA1_alfa = 0.000185;
float TA1_beta = 0;
float CB1_alfa = 0.000185;
float CB1_beta = 0;
float CA1_alfa = 0.000185;
float CA1_beta = 0;

//Variaveis HTML
String sensor_html;
String x1_html = "";
String x2_html = "";
String y1_html = "";
String y2_html = "";
float alfa_atual = 0;
float beta_atual = 0;
float alfa_html = 0;
float beta_html = 0;

//Variáveis Globais dos Sensores
float DHT1_T = 0;
float DHT2_T = 0;
float DHT3_T = 0;
float DHT4_T = 0;
float DHT1_U = 0;
float DHT2_U = 0;
float DHT3_U = 0;
float DHT4_U = 0;
float TERMOPAR1 = 0;
float TERMOPAR2 = 0;
float TERMOPAR3 = 0;
float TERMOPAR4 = 0;
float TERMOPAR5 = 0;
float TENSAO_BAIXA = 0;
float TENSAO_ALTA = 0;
float CORRENTE_BAIXA = 0;
float CORRENTE_ALTA = 0;

//Flags Globais
bool flagSelfTest = false;
bool flagStartLog = false;
bool flagLedSelf = false;
bool flagLedRun = false;
bool flagRele = false;
bool flagCalibSensor = false;
bool flagSyncInflux = false;
bool flagm1ready = false;
bool flagm2ready = false;

//Variaveis Globais Gerais
int64_t readTimeBefore = esp_timer_get_time();
String currentFileLog = "";

//Prototipação das Funções
void setupADC(void);
void loopADC(void);
bool readADC(int N);

void setupCALIBRACAO(void);
void loopCALIBRACAO(void);
void calculaCoeficientes(float X1, float X2, float Y1, float Y2);
float leSensor(String sensor);

void setupDATALOG(void);
void loopDATALOG(void);

void setupDHT(void);
void loopDHT(void);
bool readDHT(int N, int M);

void setupHARDWARE(void);
void loopHARDWARE(void);

void setupINFLUXDB(void);
void loopINFLUXDB(void);

void setupOTA_IDE(void);
void loopOTA_IDE(void);

void setupOTA_WEB(void);
void loopOTA_WEB(void);

void setupPREFERENCES(void);
void loopPREFERENCES(void);

void setupSD(void);
void loopSD(void);
void listDir_SD(fs::FS &fs, const char * dirname, uint8_t levels);
void createDir_SD(fs::FS &fs, const char * path);
void removeDir_SD(fs::FS &fs, const char * path);
void readFile_SD(fs::FS &fs, const char * path);
void writeFile_SD(fs::FS &fs, const char * path, const char * message);
void appendFile_SD(fs::FS &fs, const char * path, const char * message);
void renameFile_SD(fs::FS &fs, const char * path1, const char * path2);
void deleteFile_SD(fs::FS &fs, const char * path);
void testFileIO_SD(fs::FS &fs, const char * path);

void setupSELFTEST(void);
void loopSELFTEST(void);

void setupSERIAL(void);
void loopSERIAL(void);

void setupSPIFFS(void);
void loopSPIFFS(void);
void writeFile_SPIFFS(String state, String path);
String readFile_SPIFFS(String path);
void openFS_SPIFSS(void);

void setupTERMOPAR(void);
void loopTERMOPAR(void);
bool readTERMOPAR(int N); void listDir(fs::FS &fs, const char * dirname, uint8_t levels);

void setupTIME(void);
void loopTIME(void);
String time_Alive(void);

void setupWEB(void);
void loopWEB(void);

void setupWIFI(void);
void loopWIFI(void);

void setupWTD(void);
void loopWTD(void);
void IRAM_ATTR resetModule(void);

//Função de setup
void setup() {

  //Setup WTD
  Dprint("********** Setup WTD **********");
  setupWTD();
  //delay(1000);

  //Setup Serial
  Dprint("********** Setup Serial **********");
  setupSERIAL();
  //delay(1000);

  //Setup Preferences
  Dprint("********** Setup Preferences **********");
  setupPREFERENCES();
  //delay(1000);

  //Configuração do hardware (pinos)
  Dprint("********** Setup HARDWARE **********");
  setupHARDWARE();
  //delay(1000);

  //Setup SPIFFS
  //Dprint("********** Setup SPIFFS **********");
  //setupSPIFFS();
  //delay(1000);

  //Setup Wifi
  Dprint("********** Setup WIFI **********");
  setupWIFI();
  //delay(1000);

  //Carrega Time
  Dprint("********** Setup TIME **********");
  setupTIME();
  //delay(1000);

  //Carrega Webpage
  Dprint("********** Setup WEB **********");
  setupWEB();
  //delay(1000);

  //Setup OTA_IDE
  Dprint("********** Setup OTA_IDE **********");
  setupOTA_IDE();
  //delay(1000);

  //Setup OTA_WEB
  Dprint("********** Setup OTA_WEB **********");
  setupOTA_WEB();
  //delay(1000);

  //Carrega SD
  Dprint("********** Setup SD **********");
  setupSD();
  //delay(1000);

  //Carrega TERMOPAR
  Dprint("********** Setup TERMOAPR **********");
  setupTERMOPAR();
  //delay(1000);

  //Carrega DHR
  Dprint("********** Setup DHT **********");
  setupDHT();
  //delay(1000);

  //Carrega ADC
  Dprint("********** Setup ADC **********");
  setupADC();
  //delay(1000);

  //Carrega Datalog
  Dprint("********** Setup DATALOG **********");
  setupDATALOG();
  //delay(1000);

  //Carrega Self Test
  Dprint("********** Setup SELFTEST **********");
  setupSELFTEST();
  //delay(1000);

  //Carrega Calibracao
  Dprint("********** Setup CALIBRACAO **********");
  setupCALIBRACAO();
  //delay(1000);

  //Carrega InfluxDB
  Dprint("********** Setup INFLUXDB **********");
  setupINFLUXDB();
  //delay(1000);
}

//Loop principal
void loop() {

  //Loop Watch
  Dprint("********** Loop WTD **********");
  loopWTD();
  //delay(1000);

  //Loop Serial
  Dprint("********** Loop Serial **********");
  loopSERIAL();
  //delay(1000);

  //Loop Preferences
  Dprint("********** Loop Preferences **********");
  loopPREFERENCES();
  //delay(1000);

  //Loop Hardware
  Dprint("********** Loop Hardware **********");
  loopHARDWARE();
  //delay(1000);

  //Carrega Wifi
  Dprint("********** Loop Wifi **********");
  loopWIFI();
  //delay(1000);

  //Carrega Time
  Dprint("********** Loop Time **********");
  loopTIME();
  //delay(1000);

  //Carrega Webpage
  Dprint("********** Loop WEB **********");
  loopWEB();
  //delay(1000);

  //Loop SPIFFS
  //Dprint("********** Loop SPIFFS **********");
  //loopSPIFFS();
  //delay(1000);

  //Loop OTA_IDE
  Dprint("********** Loop OTA_IDE **********");
  loopOTA_IDE();
  //delay(1000);

  //Loop OTA_WEB
  Dprint("********** Loop OTA_WEB **********");
  loopOTA_WEB();
  //delay(1000);

  //Carrega SD
  Dprint("********** Loop SD **********");
  loopSD();
  //delay(1000);

  //Carrega TERMOPAR
  Dprint("********** Loop TERMOPAR **********");
  loopTERMOPAR();
  //delay(1000);

  //Carrega DHT
  Dprint("********** Loop DHT **********");
  loopDHT();
  //delay(1000);

  //Carrega ADC
  Dprint("********** Loop ADC **********");
  loopADC();
  //delay(1000);

  //Carrega Datalog
  Dprint("********** Loop DATALOG **********");
  loopDATALOG();
  //delay(1000);

  //Carrega Self Test
  Dprint("********** Loop SELFTEST **********");
  loopSELFTEST();
  //delay(1000);

  //Carrega Calibracao
  Dprint("********** Loop CALIBRACAO **********");
  loopCALIBRACAO();
  //delay(1000);

  //Carrega InfluxDB
  Dprint("********** Loop INFLUXDB **********");
  loopINFLUXDB();
  //delay(1000);
}
