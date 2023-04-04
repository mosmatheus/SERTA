//Setup SPIFFS
void setupSPIFFS() {
  /*Dprintln("Abrindo sistema de arquivos.");
  openFS_SPIFFS();
  Dprintln("Carregando variáveis de Sistema.");

  //Arquivo ssid_WIFI
  if (SPIFFS.exists(ssid_WIFI_Path)) {
    ssid_WIFI = readFile_SPIFFS(ssid_WIFI_Path);
    Dprintln("SSID WIFI: " + String(ssid_WIFI));
  }
  else {
    writeFile_SPIFFS("", ssid_WIFI_Path);
    Dprintln("Arquivo " + ssid_WIFI_Path + " criado com vazio");
  }

  //Arquivo password_WIFI
  if (SPIFFS.exists(password_WIFI_Path)) {
    password_WIFI = readFile_SPIFFS(password_WIFI_Path);
    Dprintln("Password WIFI: " + String(password_WIFI));
  }
  else {
    writeFile_SPIFFS("", password_WIFI_Path);
    Dprintln("Arquivo " + password_WIFI_Path + " criado com vazio");
  }

  //Arquivo Tempo de Leitura
  if (SPIFFS.exists(TempTime_Path)) {
    TempTime = readFile_SPIFFS(TempTime_Path);
    Dprintln("Tempo de Leitura: " + String(TempTime));
  }
  else {
    writeFile_SPIFFS("10", TempTime_Path);
    Dprintln("Arquivo " + TempTime_Path + " criado com 10 minutos");
  }

  //Arquivo Temperatura Maxima
  if (SPIFFS.exists(TempMAX_Path)) {
    TempMAX = readFile_SPIFFS(TempMAX_Path);
    Dprintln("Temperatura Máxima: " + String(TempMAX));
  }
  else {
    writeFile_SPIFFS("130", TempMAX_Path);
    Dprintln("Arquivo " + TempMAX_Path + " criado com 130 graus");
  }

  //Arquivo atuação Rele no Stop Test
  if (SPIFFS.exists(StopRele_Path)) {
    StopRele = readFile_SPIFFS(StopRele_Path);
    Dprintln("Stop Relé: " + String(StopRele));
  }
  else {
    writeFile_SPIFFS("1", StopRele_Path);
    Dprintln("Arquivo " + StopRele_Path + " criado com ATIVO");
  }

  //Arquivo Calibracao
  if (SPIFFS.exists(CalibSensor_Path)) {
    CalibSensor = readFile_SPIFFS(CalibSensor_Path);
    Dprintln("Calibracao Sensores: " + String(CalibSensor));
  }
  else {
    writeFile_SPIFFS("1", CalibSensor_Path);
    Dprintln("Arquivo " + CalibSensor_Path + " criado com calibracoes padrao");
  }*/
}

//Loop SPIFFS
void loopSPIFFS() {
    Dprint("Reset WTD - SPIFFS");
    timerWrite(timer, 0); //reseta o temporizador (alimenta o watchdog)
}

//Grava arquivo
void writeFile_SPIFFS(String state, String path) {
  //Abre o arquivo para escrita ("w" write)
  //Sobreescreve o conteúdo do arquivo
  File rFile = SPIFFS.open(path, "w+");
  if (!rFile) {
    Dprintln("Erro ao abrir arquivo: " + path);
  } else {
    rFile.println(state);
    Dprint("gravou estado: ");
    Dprintln(state);
  }
  rFile.close();
}

//Le o arquivo
String readFile_SPIFFS(String path) {
  File rFile = SPIFFS.open(path, "r");
  if (!rFile) {
    Dprintln("Erro ao abrir arquivo: " + path);
  }
  String content = rFile.readStringUntil('\r'); //desconsidera '\r\n'
  Dprint("Leitura do Dado: " + path + " - ");
  Dprintln(content);
  rFile.close();
  return content;
}

//Abre sistema de arquivo
void openFS_SPIFFS(void) {
  //Abre o sistema de arquivos
  if (!SPIFFS.begin(true)) {
    Dprintln("Erro ao abrir o sistema de arquivos!");
  } else {
    Dprintln("Sistema de arquivos aberto com sucesso!");
  }
}
