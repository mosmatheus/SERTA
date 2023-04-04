//Configuração dos Pinos
void setupHARDWARE() {
  Dprintln("Pinos Inicializados...");
  pinMode(LED_Self_Test, OUTPUT);
  pinMode(LED_Running, OUTPUT);
  pinMode(Rele_Off, OUTPUT);
  pinMode(WIFI_Bt, INPUT);
  pinMode(Self_Test_Bt, INPUT);
  pinMode(Start_Log_Bt, INPUT);
  pinMode(Stop_Log_Bt, INPUT);
  digitalWrite(LED_Self_Test, LOW);
  digitalWrite(LED_Running, LOW);
}

//Controle do Hardware
void loopHARDWARE() {
  
  Dprint("Reset WTD - HARDWARE");
  timerWrite(timer, 0); //reseta o temporizador (alimenta o watchdog)

  //Trata Botao de Self Test
  if (digitalRead(Self_Test_Bt) == HIGH) {
    flagSelfTest = true;
    flagLedSelf = false;
  }

  //Trata Botao de Start do Datalog
  if (digitalRead(Start_Log_Bt) == HIGH || flagStartLog == false) {
    flagStartLog = true;
    currentFileLog = "/" + formattedDate + ".txt";
  }

  //Trata o Botao do Stop do Datalog
  if (digitalRead(Stop_Log_Bt) == HIGH) {
    flagStartLog = false;
    currentFileLog = "";
  }

  //Trata o Botao do Wifi (ressetar as configuracoes e ir para o AP
  if ( digitalRead(WIFI_Bt) == HIGH ) {
    Dprintln("Botao WIFI Pressionado... pressione por 5 segundos");
    delay(5000);                        //Pressionar por 5 segundos
    if (digitalRead(WIFI_Bt) == HIGH ) {
      Dprintln("Botao WIFI pressionado. Config Wifi ressetadas e Acessando Modo AP");
      preferences.putString("ssid_WIFI","");
      preferences.putString("password_WIFI","");
      preferences.end();
      //writeFile_SPIFFS("", ssid_WIFI_Path);
      //writeFile_SPIFFS("", password_WIFI_Path);
      esp_restart();
    }
  }

  //Trata o Led de Self Test
  if (flagLedSelf = true) {
    digitalWrite(LED_Self_Test, HIGH);
  }
  else {
    digitalWrite(LED_Self_Test, LOW);
  }

  //Trata o Led de Running Datalog
  if (flagLedRun = true) {
    digitalWrite(LED_Running, HIGH);
  }
  else {
    digitalWrite(LED_Running, LOW);
  }

  //Trata o Relé Off
  if (flagRele = true) {
    digitalWrite(Rele_Off, HIGH);
  }
  else {
    digitalWrite(Rele_Off, LOW);
  }
}
