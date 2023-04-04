//Configuracao dos DHTs
void setupDHT() {
  DHT1.begin();
  DHT2.begin();
  DHT3.begin();
  DHT4.begin();
}

//Loop dos DHTs
void loopDHT() {
    Dprint("Reset WTD - DHT");
    timerWrite(timer, 0); //reseta o temporizador (alimenta o watchdog)
}

//Rotina de Leitura dos DHTs
bool readDHT(int N, int M) {   //N -> 1 a 4; M -> 1=Temperatura, 2=Umidade
  switch (N) {
    case 1:
      switch (M) {
        case 1:
          DHT1_T = DHT1T_alfa * DHT1.readTemperature() + DHT1T_beta;
          return true;
          break;
        case 2:
          DHT1_U = DHT1U_alfa * DHT1.readHumidity() + DHT1U_beta;
          return true;
          break;
        default:
          return false;
      }
    case 2:
      switch (M) {
        case 1:
          DHT2_T = DHT2T_alfa * DHT1.readTemperature() + DHT2T_beta;
          return true;
          break;
        case 2:
          DHT2_U = DHT2U_alfa * DHT1.readHumidity() + DHT2U_beta;
          return true;
          break;
        default:
          return false;
      }
    case 3:
      switch (M) {
        case 1:
          DHT3_T = DHT3T_alfa * DHT1.readTemperature() + DHT3T_beta;
          return true;
          break;
        case 2:
          DHT3_U = DHT3U_alfa * DHT1.readHumidity() + DHT3U_beta;
          return true;
          break;
        default:
          return false;
      }
    case 4:
      switch (M) {
        case 1:
          DHT4_T = DHT4T_alfa * DHT1.readTemperature() + DHT4T_beta;
          return true;
          break;
        case 2:
          DHT4_U = DHT4U_alfa * DHT1.readHumidity() + DHT4U_beta;
          return true;
          break;
        default:
          return false;
      }
    default:
      return false;
  }
}
