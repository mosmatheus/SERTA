//Configuracao Selftest
void setupSELFTEST() {

}

//Loop Self Test
void loopSELFTEST() {
  Dprint("Reset WTD - SELFTEST");
  timerWrite(timer, 0); //reseta o temporizador (alimenta o watchdog)
  int falhaTest = 0;
  int i;
  int j;
  if (flagSelfTest = true) {
    //Realiza leituras dos DHTs
    for (i = 1; i <= 4; i++) {
      for (j = 1; j <= 2; j++) {
        if (readDHT(i, j)) {
          Dprintln("Leituras do DHT ");
          Dprintln(i);
          Dprint( " realizada com sucesso");
        }
        else {
          Dprintln("Leituras do DHT ");
          Dprintln(i);
          Dprint( " realizada sem sucesso");
        }
      }
    }
    //Realiza as leituras dos ADCs
    for (i = 1; i <= 4; i++) {
      if (readADC(i)) {
        Dprintln("Leituras do ADC ");
        Dprintln(i);
        Dprint( " realizada com sucesso");
      }
      else {
        Dprintln("Leituras do ADC ");
        Dprintln(i);
        Dprint( " realizada sem sucesso");
      }
    }
    //Realiza as leituras dos Termopares
    for (i = 1; i <= 5; i++) {
      if (readTERMOPAR(i)) {
        Dprintln("Leituras do Termopar ");
        Dprintln(i);
        Dprint( " realizada com sucesso");
      }
      else {
        Dprintln("Leituras do Termopar ");
        Dprintln(i);
        Dprint( " realizada sem sucesso");
      }
    }

    //Verifica as medicoes
    if (formattedDate = "") {
      falhaTest = falhaTest + 1;
      Dprint("Falha ao carregar o Timestamp");
    }
    if (DHT1_T = 0) {
      falhaTest = falhaTest + 1;
      Dprint("Falha ao carregar o DHT1_T");
    }
    if (DHT2_T = 0) {
      falhaTest = falhaTest + 1;
      Dprint("Falha ao carregar o DHT2_T");
    }
    if (DHT3_T = 0) {
      falhaTest = falhaTest + 1;
      Dprint("Falha ao carregar o DHT3_T");
    }
    if (DHT4_T = 0) {
      falhaTest = falhaTest + 1;
      Dprint("Falha ao carregar o DHT4_T");
    }
    if (DHT1_U = 0) {
      falhaTest = falhaTest + 1;
      Dprint("Falha ao carregar o DHT1_U");
    }
    if (DHT2_U = 0) {
      falhaTest = falhaTest + 1;
      Dprint("Falha ao carregar o DHT2_U");
    }
    if (DHT3_U = 0) {
      falhaTest = falhaTest + 1;
      Dprint("Falha ao carregar o DHT3_U");
    }
    if (DHT4_U = 0) {
      falhaTest = falhaTest + 1;
      Dprint("Falha ao carregar o DHT4_U");
    }
    if (TERMOPAR1 = 0) {
      falhaTest = falhaTest + 1;
      Dprint("Falha ao carregar o TERMOPAR1");
    }
    if (TERMOPAR2 = 0) {
      falhaTest = falhaTest + 1;
      Dprint("Falha ao carregar o TERMOPAR2");
    }
    if (TERMOPAR3 = 0) {
      falhaTest = falhaTest + 1;
      Dprint("Falha ao carregar o TERMOPAR3");
    }
    if (TERMOPAR4 = 0) {
      falhaTest = falhaTest + 1;
      Dprint("Falha ao carregar o TERMOPAR4");
    }
    if (TERMOPAR5 = 0) {
      falhaTest = falhaTest + 1;
      Dprint("Falha ao carregar o TERMOPAR5");
    }
    if (TENSAO_BAIXA = 0) {
      falhaTest = falhaTest + 1;
      Dprint("Falha ao carregar a TENSAO_BAIXA");
    }
    if (CORRENTE_BAIXA = 0) {
      falhaTest = falhaTest + 1;
      Dprint("Falha ao carregar a CORRENTE_BAIXA");
    }
    if (TENSAO_ALTA = 0) {
      falhaTest = falhaTest + 1;
      Dprint("Falha ao carregar a TENSAO_ALTA");
    }
    if (CORRENTE_ALTA = 0) {
      falhaTest = falhaTest + 1;
      Dprint("Falha ao carregar a CORRENTE_ALTA");
    }
    if (falhaTest == 0) {
      Dprint("Self Test Sucesso!");
      flagLedSelf = true;
    }
    else {
      Dprint("Self Test Falhou");
      flagLedSelf = false;
    }
    flagSelfTest = false;
  }
}
