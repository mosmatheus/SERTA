//Configuracao o Web Server (Webpages)
void setupWEB() {

  //Trata as conexões de clientes WEB
  if (1) {
    //atende uma solicitação para a raiz
    server.on("/", HTTP_GET, []() //atende uma solicitação para a raiz
    {
      Dprintln("Acessando Raiz");
      server.sendHeader("Connection", "close");
      raiz = MAIN_page;
      server.send(200, "text/html", raiz);
    });

    //atende uma solicitação para configuração
    server.on("/config", HTTP_GET, []() //atende uma solicitação para a config
    {
      Dprintln("Acessando Configuração");
      server.sendHeader("Connection", "close");
      configuracao = CONFIG_page;
      server.send(200, "text/html", configuracao);
    });

    //atende uma solicitação para reboot
    server.on("/reboot", HTTP_GET, []() //atende uma solicitação para a config
    {
      Dprintln("Rebooting...");
      server.sendHeader("Connection", "close");
      server.send(200, "text/html", raiz);
      delay(1000);
      ESP.restart();
    });

    //atende uma solicitação para checar os dados da configuracao
    server.on("/checaconfig", HTTP_POST, []() //atende uma solicitação para a config
    {
      Dprintln("Checando Configurações:"); //somente para debug
      Dprintln("SSID WIFI: " + String(server.arg("ssid_WIFI"))); //somente para debug
      Dprintln("Password WIFI: " + String(server.arg("password_WIFI"))); //somente para debug
      Dprintln("Tempo de Leitura: " + String(server.arg("TempTime"))); //somente para debug
      Dprintln("Temperatura Máxima: " + String(server.arg("TempMAX"))); //somente para debug
      Dprintln("Relé atua no Stop?: " + String(server.arg("StopRele"))); //somente para debug
      ssid_WIFI = server.arg("ssid_WIFI");
      preferences.putString("ssid_WIFI", ssid_WIFI);
      //writeFile_SPIFFS(String(ssid_WIFI), ssid_WIFI_Path);
      password_WIFI = server.arg("password_WIFI");
      preferences.putString("password_WIFI", password_WIFI);
      //writeFile_SPIFFS(String(password_WIFI), password_WIFI_Path);
      TempTime = server.arg("TempTime").toInt();
      preferences.putInt("TempTime", TempTime);
      //writeFile_SPIFFS(String(TempTime), TempTime_Path);
      TempMAX = server.arg("TempMAX").toInt();
      preferences.putInt("TempMAX", TempMAX);
      //writeFile_SPIFFS(String(TempMAX), TempMAX_Path);
      StopRele = server.arg("StopRele");
      preferences.putBool("StopRele",StopRele);
      //writeFile_SPIFFS(String(StopRele), StopRele_Path);
      preferences.end();
      server.sendHeader("Connection", "close");
      server.send(200, "text/html", raiz);
    });

    server.begin(); //inicia o servidor

    Dprintln("WEBPage Iniciada - " + INFOS); //envia as informações armazenadas em INFOS, para debug

    //Envia ara a serial o IP atual do ESP
    Dprint("Servidor em: ");
    Dprintln( WiFi.localIP().toString() + ":" + PORTA);
  }
  else
  {
    //avisa se não onseguir conectar no WiFi
    Dprintln("Falha ao conectar ao WiFi.");
  }
}

//Loop do Web Server (Webpage de Configuração)
void loopWEB() {
  Dprint("Reset WTD - WEB");
  timerWrite(timer, 0); //reseta o temporizador (alimenta o watchdog)

  //Dprintln("WEBPage em Loop");
  server.handleClient();

  //Informacoes atualizadas
  chipID = (uint32_t)(ESP.getEfuseMac() >> 32); //um ID exclusivo do Chip...
  CHIP_ID = "<p> Chip ID: " + String(chipID) + "</p>"; // montado para ser usado no HTML
  VERSION = "<p> Versão: 1.0 </p>"; //Exemplo de um controle de versão
  RunningTime = "<p> Tempo Funcionando: " + time_Alive() + "</p>";
  INFOS = VERSION + CHIP_ID + RunningTime;

  //Páginas HTML utilizadas
  //raiz = "<!DOCTYPE html><html><head><title>MBD Ensaio Temperatura</title><meta charset='utf-8'><style>html { font-family: Helvetica; display: inline-block;""margin: 0px auto; text-align: center;} .button { background-color: #4CAF50; border: none; color: white; padding: 5px 30px;text-decoration: none; font-size: 15px; margin: 2px; cursor: pointer;}</style></head><body><h1>MBD Ensaio Temperatura</h1>" + INFOS + "<p><a href='/verificaOTA'><button class='button'>Configurações Internas</button></a></p><p><a href='/verificaOTA'><button class='button'>Calibração dos Sensores</button></a></p><p><a href='/calibracao'><button class='button'>Atualização OTA</button></a></p></p><p><a href='/reboot'><button class='button'>Reinicia o Sistema</button></a></p></body></html>";
  //configuracao = "<!DOCTYPE html><html><head><title>Configuração MBD Ensaio Temperatura</title><meta charset='utf-8'><style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;} .input[type=button], input[type=submit], input[type=reset] { background-color: #4CAF50; border: none; color: white; padding: 5px 30px;text-decoration: none; font-size: 15px; margin: 2px; cursor: pointer;}</style></head><body><h1>Configuração MBD Ensaio Temperatura</h1>" + INFOS + "<form method='POST' action='/checaconfig 'enctype='multipart/form-data'><p><label>Tempo Leitura Temperatura: </label><input type='text' name='ssid_WIFI' value=" + String(TempTime) + "></p><p><label>SSID WIFI: </label><input type='text' name='ssid_WIFI' value=" + String(ssid_WIFI) + "></p><p><label>Password WIFI: </label><input type='password' name='password_WIFI' value=" + String(password_WIFI) + "></p><p><label>Temperatura Máxima para Desligamento: </label><input type='text' name='ssid_WIFI' value=" + String(TempMAX) + "></p><p><label>Relé atua no Stop Test?: </label><input type='checkbox' name='stoprele' value=" + String(StopRele) + "></p><p><input type='submit' value='Ok'></p></form></body></html>";
}
