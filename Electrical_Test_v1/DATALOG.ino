//Configuraçao do Datalog
void setupDATALOG() {

}

//Loop do Datalog
void loopDATALOG() {
  //WATCHDOG
  Dprint("Reset WTD - DATALOG");

  //TESTES DE ESTABILIDADE DE TEMPERATURA

  //TESTE DE ABORTO DO TESTE
  
  //DATALOG
  timerWrite(timer, 0); //reseta o temporizador (alimenta o watchdog)
  int64_t microTempTime = TempTime * 60 * 1000;
  String sample = "";
  int i = 0;
  int j = 0;
  if (flagStartLog) {
    if (esp_timer_get_time() > readTimeBefore + microTempTime) {
      //Se o tempo passado for maior que o TemTime, o sistema logará
      readTimeBefore = esp_timer_get_time();

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

      //Concatena todas as medidas
      sample = formattedDate + ";" + TERMOPAR1 + ";" + TERMOPAR2 + ";" + TERMOPAR3 + ";" + TERMOPAR4 + ";" + TERMOPAR5 + ";" + DHT1_T\
               + ";" + DHT2_T + ";" + DHT3_T + ";" + DHT4_T + ";" + DHT1_U + ";" + DHT2_U + ";" + DHT3_U + ";" + DHT4_U + ";" + TENSAO_BAIXA\
               + ";" + CORRENTE_BAIXA + ";" + TENSAO_ALTA + ";" + CORRENTE_ALTA + '\n';

      flagSyncInflux = true;

      //Verificar se ja existe o arquivo corrente no SD
      if (SD.exists("/" + currentFileLog + ".txt")) {
        //Se exister apenas insere os novos dados
        Dprint("Arquivo ja existe, adicionando informacao");
        appendFile_SD(SD, currentFileLog.c_str(), sample.c_str());
      }
      else {
        //Senao, cria o arquivo e insere as informacoes
        Dprint("Arquivo inexistente, criadno arquivo e adicionando as informacoes");
        writeFile_SD(SD, currentFileLog.c_str(), sample.c_str());
      }
    }
  }
}
