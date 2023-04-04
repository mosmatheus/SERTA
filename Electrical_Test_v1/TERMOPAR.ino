//Configuracao Termopares
void setupTERMOPAR() {

}

//Loop Termopares
void loopTERMOPAR() {
  Dprint("Reset WTD - TERMOPAR");
  timerWrite(timer, 0); //reseta o temporizador (alimenta o watchdog)
}

//Rotina de Leitura dos Termpares
bool readTERMOPAR(int N) {
  switch (N) {
    case 1:
      TERMOPAR1 = Termo1_alfa * Termo1.readCelsius() + Termo1_beta;
      return true;
      break;
    case 2:
      TERMOPAR2 = Termo2_alfa * Termo1.readCelsius() + Termo2_beta;
      return true;
      break;
    case 3:
      TERMOPAR3 = Termo3_alfa * Termo1.readCelsius() + Termo3_beta;
      return true;
      break;
    case 4:
      TERMOPAR4 = Termo4_alfa * Termo1.readCelsius() + Termo4_beta;
      return true;
      break;
    case 5:
      TERMOPAR5 = Termo5_alfa * Termo1.readCelsius() + Termo5_beta;
      return true;
      break;
    default:
      return false;
  }
}
